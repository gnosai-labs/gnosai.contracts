#include <gnos.stake/gnos.stake.hpp>

#include <tuple>

using namespace eosio;

namespace {

static constexpr name ntoken_contract = "gnos.ntoken"_n;

struct [[eosio::table("tokenstats")]] ntoken_stat_probe {
    flon::nasset supply;
    flon::nasset max_supply;
    std::string token_uri;
    name ipowner;
    name notary;
    name issuer;
    time_point_sec issued_at;
    time_point_sec notarized_at;
    bool paused = false;

    uint64_t primary_key() const {
        return supply.symbol.nid;
    }

    EOSLIB_SERIALIZE(ntoken_stat_probe, (supply)(max_supply)(token_uri)(ipowner)(notary)(issuer)(issued_at)(notarized_at)(paused))
};

using ntoken_stat_probe_table = multi_index<"tokenstats"_n, ntoken_stat_probe>;

const ntoken_stat_probe& require_ntoken_stat(uint64_t nft_id, ntoken_stat_probe_table& stats) {
    auto itr = stats.find(nft_id);
    check(itr != stats.end(), "nft does not exist");
    return *itr;
}

name get_nft_controller(const ntoken_stat_probe& stat) {
    return stat.ipowner.value == 0 ? stat.issuer : stat.ipowner;
}

} // namespace

gnos::stake_plan_table::const_iterator gnosstake::require_plan(uint64_t plan_id, gnos::stake_plan_table& plans) const {
    auto itr = plans.find(plan_id);
    check(itr != plans.end(), "plan does not exist");
    return itr;
}

gnos::stake_position_table::const_iterator gnosstake::require_position(name staker, uint64_t plan_id, gnos::stake_position_table& positions) const {
    auto staker_idx = positions.get_index<"bystaker"_n>();
    auto pos_itr = staker_idx.lower_bound(staker.value);
    for (; pos_itr != staker_idx.end() && pos_itr->staker == staker; ++pos_itr) {
        if (pos_itr->plan_id == plan_id) {
            return positions.find(pos_itr->position_id);
        }
    }
    check(false, "stake position does not exist");
    return positions.end();
}

asset gnosstake::compute_claimable(const gnos::stake_plan_t& plan, const gnos::stake_position_t& position) const {
    const uint64_t delta_rps = plan.reward_per_share - position.reward_per_share_paid;
    const int64_t accrued_amount = static_cast<int64_t>((static_cast<__int128_t>(position.staked.amount) * delta_rps) / reward_precision);
    asset accrued{accrued_amount, plan.total_reward.symbol};
    return position.pending_reward + accrued;
}

void gnosstake::settle_position(const gnos::stake_plan_t& plan, gnos::stake_position_t& position) const {
    position.pending_reward = compute_claimable(plan, position);
    position.reward_per_share_paid = plan.reward_per_share;
}

uint64_t gnosstake::parse_plan_memo(const std::string& memo, const char* prefix) const {
    const std::string prefix_str(prefix);
    if (memo.size() <= prefix_str.size() || memo.compare(0, prefix_str.size(), prefix_str) != 0) {
        return 0;
    }
    uint64_t value = 0;
    for (size_t i = prefix_str.size(); i < memo.size(); ++i) {
        const char c = memo[i];
        check(c >= '0' && c <= '9', "invalid memo");
        value = value * 10 + static_cast<uint64_t>(c - '0');
    }
    return value;
}

void gnosstake::createplan(uint64_t nft_id, name stake_token_contract, symbol stake_symbol, name reward_token_contract, symbol reward_symbol) {
    ntoken_stat_probe_table nft_stats(ntoken_contract, ntoken_contract.value);
    const auto& nft_stat = require_ntoken_stat(nft_id, nft_stats);
    require_auth(get_nft_controller(nft_stat));
    check(stake_token_contract.value != 0, "stake token contract is required");
    check(reward_token_contract.value != 0, "reward token contract is required");
    check(stake_symbol.is_valid(), "invalid stake symbol");
    check(reward_symbol.is_valid(), "invalid reward symbol");

    gnos::stake_plan_table plans(get_self(), get_self().value);
    auto nft_idx = plans.get_index<"bynft"_n>();
    check(nft_idx.find(nft_id) == nft_idx.end(), "plan already exists for nft");

    const uint64_t plan_id = gnos::next_id(plans.available_primary_key());
    plans.emplace(get_nft_controller(nft_stat), [&](auto& row) {
        row.plan_id = plan_id;
        row.nft_id = nft_id;
        row.stake_token_contract = stake_token_contract;
        row.reward_token_contract = reward_token_contract;
        row.total_staked = asset{0, stake_symbol};
        row.total_reward = asset{0, reward_symbol};
        row.claimed_reward = asset{0, reward_symbol};
        row.undistributed_reward = asset{0, reward_symbol};
        row.active = true;
    });
}

void gnosstake::apply_stake(name staker, uint64_t plan_id, const asset& quantity) {
    check(quantity.is_valid(), "invalid quantity");
    check(quantity.amount > 0, "stake quantity must be positive");

    gnos::stake_plan_table plans(get_self(), get_self().value);
    auto plan_itr = require_plan(plan_id, plans);
    check(plan_itr->active, "plan is inactive");
    check(quantity.symbol == plan_itr->total_staked.symbol, "symbol mismatch");

    gnos::stake_position_table positions(get_self(), get_self().value);
    uint64_t found_primary = 0;
    auto staker_idx = positions.get_index<"bystaker"_n>();
    auto pos_itr = staker_idx.lower_bound(staker.value);
    for (; pos_itr != staker_idx.end() && pos_itr->staker == staker; ++pos_itr) {
        if (pos_itr->plan_id == plan_id) {
            found_primary = pos_itr->position_id;
            break;
        }
    }

    const bool had_zero_stake = (plan_itr->total_staked.amount == 0);

    if (found_primary == 0) {
        const uint64_t position_id = gnos::next_id(positions.available_primary_key());
        positions.emplace(staker, [&](auto& row) {
            row.position_id = position_id;
            row.plan_id = plan_id;
            row.staker = staker;
            row.staked = quantity;
            row.reward_per_share_paid = plan_itr->reward_per_share;
            row.pending_reward = asset{0, plan_itr->total_reward.symbol};
        });
    } else {
        auto primary_itr = positions.find(found_primary);
        positions.modify(primary_itr, same_payer, [&](auto& row) {
            settle_position(*plan_itr, row);
            row.staked += quantity;
        });
    }

    plans.modify(plan_itr, same_payer, [&](auto& row) {
        row.total_staked += quantity;
        if (had_zero_stake && row.undistributed_reward.amount > 0) {
            row.reward_per_share += static_cast<uint64_t>(
                (static_cast<__int128_t>(row.undistributed_reward.amount) * reward_precision) / row.total_staked.amount
            );
            row.undistributed_reward.amount = 0;
        }
    });
}

void gnosstake::stake(name staker, uint64_t plan_id, asset quantity) {
    require_auth(get_self());
    apply_stake(staker, plan_id, quantity);
}

void gnosstake::apply_fundreward(uint64_t plan_id, const asset& quantity) {
    gnos::stake_plan_table plans(get_self(), get_self().value);
    auto plan_itr = require_plan(plan_id, plans);

    ntoken_stat_probe_table nft_stats(ntoken_contract, ntoken_contract.value);
    require_ntoken_stat(plan_itr->nft_id, nft_stats);

    check(quantity.is_valid(), "invalid quantity");
    check(quantity.amount > 0, "reward quantity must be positive");
    check(quantity.symbol == plan_itr->total_reward.symbol, "symbol mismatch");

    plans.modify(plan_itr, same_payer, [&](auto& row) {
        row.total_reward += quantity;
        if (row.total_staked.amount > 0) {
            row.reward_per_share += static_cast<uint64_t>((static_cast<__int128_t>(quantity.amount) * reward_precision) / row.total_staked.amount);
        } else {
            row.undistributed_reward += quantity;
        }
    });
}

void gnosstake::fundreward(uint64_t plan_id, asset quantity) {
    require_auth(get_self());
    apply_fundreward(plan_id, quantity);
}

void gnosstake::unstake(name staker, uint64_t plan_id, asset quantity) {
    require_auth(staker);
    check(quantity.is_valid(), "invalid quantity");
    check(quantity.amount > 0, "unstake quantity must be positive");

    gnos::stake_plan_table plans(get_self(), get_self().value);
    auto plan_itr = require_plan(plan_id, plans);
    check(quantity.symbol == plan_itr->total_staked.symbol, "symbol mismatch");

    gnos::stake_position_table positions(get_self(), get_self().value);
    auto pos_itr = require_position(staker, plan_id, positions);
    check(pos_itr->staked.amount >= quantity.amount, "insufficient staked balance");

    auto mutable_pos = positions.find(pos_itr->position_id);
    positions.modify(mutable_pos, same_payer, [&](auto& row) {
        settle_position(*plan_itr, row);
        row.staked -= quantity;
    });

    plans.modify(plan_itr, same_payer, [&](auto& row) {
        row.total_staked -= quantity;
    });

    action(
        permission_level{get_self(), "active"_n},
        plan_itr->stake_token_contract,
        "transfer"_n,
        std::make_tuple(get_self(), staker, quantity, std::string("unstake:") + std::to_string(plan_id))
    ).send();
}

void gnosstake::claim(name staker, uint64_t plan_id) {
    require_auth(staker);

    gnos::stake_plan_table plans(get_self(), get_self().value);
    auto plan_itr = require_plan(plan_id, plans);

    gnos::stake_position_table positions(get_self(), get_self().value);
    auto pos_itr = require_position(staker, plan_id, positions);
    const asset claimable = compute_claimable(*plan_itr, *pos_itr);
    check(claimable.amount > 0, "no claimable reward");

    auto mutable_pos = positions.find(pos_itr->position_id);
    positions.modify(mutable_pos, same_payer, [&](auto& row) {
        row.pending_reward = asset{0, claimable.symbol};
        row.reward_per_share_paid = plan_itr->reward_per_share;
    });

    plans.modify(plan_itr, same_payer, [&](auto& row) {
        row.claimed_reward += claimable;
    });

    action(
        permission_level{get_self(), "active"_n},
        plan_itr->reward_token_contract,
        "transfer"_n,
        std::make_tuple(get_self(), staker, claimable, std::string("claim:") + std::to_string(plan_id))
    ).send();
}

void gnosstake::ontransfer(name from, name to, asset quantity, std::string memo) {
    if (to != get_self() || from == get_self()) {
        return;
    }

    const uint64_t stake_plan_id = parse_plan_memo(memo, "stake:");
    if (stake_plan_id != 0) {
        gnos::stake_plan_table plans(get_self(), get_self().value);
        auto plan_itr = require_plan(stake_plan_id, plans);
        check(get_first_receiver() == plan_itr->stake_token_contract, "stake token contract mismatch");
        apply_stake(from, stake_plan_id, quantity);
        return;
    }

    const uint64_t reward_plan_id = parse_plan_memo(memo, "reward:");
    if (reward_plan_id != 0) {
        gnos::stake_plan_table plans(get_self(), get_self().value);
        auto plan_itr = require_plan(reward_plan_id, plans);

        ntoken_stat_probe_table nft_stats(ntoken_contract, ntoken_contract.value);
        const auto& nft_stat = require_ntoken_stat(plan_itr->nft_id, nft_stats);
        check(from == get_nft_controller(nft_stat) || from == "nftv.mart"_n, "only nft owner or nftv.mart can fund reward");
        check(get_first_receiver() == plan_itr->reward_token_contract, "reward token contract mismatch");

        apply_fundreward(reward_plan_id, quantity);
        return;
    }
}
