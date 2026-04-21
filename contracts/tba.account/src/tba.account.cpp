#include <tba.account/tba.account.hpp>

#include <tuple>

using namespace eosio;

namespace {

static constexpr name ntoken_contract = "gnos.ntoken"_n;
static constexpr name active_permission = "active"_n;

struct ntoken_action {
    [[eosio::action]]
    void transfer(const name& from, const name& to, const std::vector<flon::nasset>& assets, const std::string& memo);

    using transfer_action = action_wrapper<"transfer"_n, &ntoken_action::transfer>;
};

struct [[eosio::table("accounts")]] ntoken_account_probe {
    flon::nasset balance;
    bool paused = false;

    uint64_t primary_key() const {
        return balance.symbol.nid;
    }

    EOSLIB_SERIALIZE(ntoken_account_probe, (balance)(paused))
};

using ntoken_account_probe_table = multi_index<"accounts"_n, ntoken_account_probe>;

#define TRANSFER_NFT(bank, from, to, assets, memo) \
    { ntoken_action::transfer_action act{ bank, { {from, active_permission} } }; \
      act.send(from, to, assets, memo); }

void check_owner_root_balance(const gnos::tba_map_t& tba) {
    ntoken_account_probe_table accounts(tba.nft_contract, tba.owner.value);
    auto itr = accounts.find(tba.root_nft.symbol.nid);
    check(itr != accounts.end(), "tba owner has no root nft balance");
    check(!itr->paused, "tba owner root nft balance is paused");
    check(itr->balance.amount >= tba.root_nft.amount, "tba owner no longer holds root nft");
}

void transfer_token(name token_contract, name from, name to, asset quantity, const std::string& memo) {
    action(
        permission_level{from, active_permission},
        token_contract,
        "transfer"_n,
        std::make_tuple(from, to, quantity, memo)
    ).send();
}

}

name tbaaccount::require_tba_owner(uint64_t tba_id) const {
    gnos::tba_map_table tbas("tba.reg"_n, "tba.reg"_n.value);
    auto tba_itr = tbas.find(tba_id);
    check(tba_itr != tbas.end(), "tba does not exist");
    check(tba_itr->status == gnos::active, "tba is not active");
    check(tba_itr->owner.value != 0, "tba owner is not set");
    check_owner_root_balance(*tba_itr);
    return tba_itr->owner;
}

uint64_t tbaaccount::parse_tba_deposit_memo(const std::string& memo, std::string& asset_memo) const {
    static const std::string prefix = "deposit:";
    if (memo.size() <= prefix.size() || memo.compare(0, prefix.size(), prefix) != 0) {
        return 0;
    }

    uint64_t tba_id = 0;
    size_t cursor = prefix.size();
    for (; cursor < memo.size(); ++cursor) {
        const char c = memo[cursor];
        if (c == ':') {
            ++cursor;
            break;
        }
        check(c >= '0' && c <= '9', "invalid deposit memo");
        tba_id = tba_id * 10 + static_cast<uint64_t>(c - '0');
    }

    asset_memo = cursor < memo.size() ? memo.substr(cursor) : std::string{};
    return tba_id;
}

gnos::bundle_manifest_t tbaaccount::build_manifest(uint64_t tba_id) const {
    gnos::tba_asset_table assets(get_self(), get_self().value);
    auto idx = assets.get_index<"bytba"_n>();
    auto itr = idx.lower_bound(tba_id);

    std::vector<gnos::bundle_item_t> items;
    for (; itr != idx.end() && itr->tba_id == tba_id; ++itr) {
        check(itr->settlement_mode != gnos::blocking, "blocking asset exists in tba");
        if (itr->settlement_mode == gnos::deliverable) {
            gnos::bundle_item_t item;
            item.asset_id = itr->id;
            item.asset_type = itr->asset_type;
            item.asset_contract = itr->asset_contract;
            item.asset_ref_id = itr->asset_ref_id;
            item.quantity = itr->quantity;
            item.nft_quantity = itr->nft_quantity;
            items.push_back(item);
        }
    }

    check(!items.empty(), "bundle requires at least one deliverable asset");

    gnos::bundle_manifest_t manifest;
    manifest.tba_id = tba_id;
    manifest.deliverables = items;
    manifest.deliverable_count = items.size();
    auto packed = pack(std::make_tuple(tba_id, items));
    manifest.manifest_hash = sha256(packed.data(), packed.size());
    manifest.snapshot_at = time_point_sec(current_time_point());
    return manifest;
}

void tbaaccount::deposit(uint64_t tba_id, uint8_t asset_type, name asset_contract, unsigned __int128 asset_ref_id, asset quantity, std::string memo) {
    const name owner = require_tba_owner(tba_id);
    require_auth(owner);
    check(asset_contract.value != 0, "asset contract is required");
    check(asset_type != 1, "token deposits must use token transfer with memo deposit:<tba_id>[:memo]");
    check(asset_type != 2, "nft deposits must use gnos.ntoken transfer with memo deposit:<tba_id>[:memo]");
    check(false, "unsupported asset type for direct deposit");
}

void tbaaccount::deposittoken(name owner, uint64_t tba_id, name token_contract, asset quantity, std::string memo) {
    require_auth(owner);
    check(token_contract.value != 0, "token contract is required");
    check(quantity.is_valid(), "invalid token quantity");
    check(quantity.amount > 0, "token quantity must be positive");

    const name tba_owner = require_tba_owner(tba_id);
    check(owner == tba_owner, "only current tba owner can deposit token assets");

    transfer_token(token_contract, owner, get_self(), quantity, std::string("deposit:") + std::to_string(tba_id));

    gnos::tba_asset_table assets(get_self(), get_self().value);
    const uint64_t asset_id = gnos::next_id(assets.available_primary_key());
    assets.emplace(owner, [&](auto& row) {
        row.id = asset_id;
        row.tba_id = tba_id;
        row.asset_type = 1;
        row.asset_contract = token_contract;
        row.asset_ref_id = 0;
        row.quantity = quantity;
        row.settlement_mode = gnos::deliverable;
        row.memo = memo;
    });
}

void tbaaccount::withdraw(uint64_t tba_id, uint64_t asset_id) {
    const name owner = require_tba_owner(tba_id);
    require_auth(owner);

    gnos::tba_asset_table assets(get_self(), get_self().value);
    auto itr = assets.find(asset_id);
    check(itr != assets.end(), "asset does not exist");
    check(itr->tba_id == tba_id, "asset does not belong to tba");
    check(!itr->frozen, "asset is frozen");

    if (itr->asset_type == 1) {
        action(
            permission_level{get_self(), active_permission},
            itr->asset_contract,
            "transfer"_n,
            std::make_tuple(get_self(), owner, itr->quantity, std::string("tba withdraw:") + std::to_string(tba_id))
        ).send();
    } else if (itr->asset_type == 2 && itr->asset_contract == ntoken_contract) {
        TRANSFER_NFT(
            itr->asset_contract,
            get_self(),
            owner,
            std::vector<flon::nasset>{itr->nft_quantity},
            std::string("tba nft withdraw:") + std::to_string(tba_id)
        );
    } else {
        check(false, "unsupported asset type for withdraw");
    }

    assets.erase(itr);
}

void tbaaccount::setmode(uint64_t asset_id, uint8_t settlement_mode_value) {
    gnos::check_valid_settlement_mode(settlement_mode_value);

    gnos::tba_asset_table assets(get_self(), get_self().value);
    auto itr = assets.find(asset_id);
    check(itr != assets.end(), "asset does not exist");
    check(!itr->frozen, "asset is frozen");

    const name owner = require_tba_owner(itr->tba_id);
    require_auth(owner);

    assets.modify(itr, same_payer, [&](auto& row) {
        row.settlement_mode = settlement_mode_value;
    });
}

void tbaaccount::freezeassets(uint64_t tba_id, std::vector<uint64_t> asset_ids, bool frozen_state) {
    check(has_auth("nftv.mart"_n) || has_auth(require_tba_owner(tba_id)), "missing authority");

    gnos::tba_asset_table assets(get_self(), get_self().value);
    for (uint64_t asset_id : asset_ids) {
        auto itr = assets.find(asset_id);
        check(itr != assets.end(), "asset does not exist");
        check(itr->tba_id == tba_id, "asset does not belong to tba");
        if (frozen_state) {
            check(!itr->frozen, "asset already frozen");
        }
        assets.modify(itr, same_payer, [&](auto& row) {
            row.frozen = frozen_state;
        });
    }
}

void tbaaccount::genmanifest(uint64_t tba_id) {
    const name owner = require_tba_owner(tba_id);
    require_auth(owner);

    gnos::bundle_manifest_t manifest = build_manifest(tba_id);
    gnos::bundle_manifest_table manifests(get_self(), get_self().value);
    manifest.manifest_id = gnos::next_id(manifests.available_primary_key());
    manifests.emplace(owner, [&](auto& row) {
        row = manifest;
    });
}

void tbaaccount::receivenft(uint64_t tba_id, name nft_contract, flon::nasset nft_quantity, std::string memo) {
    require_auth(nft_contract);
    check(nft_contract == ntoken_contract, "only gnos.ntoken custody is currently supported");
    check(nft_quantity.amount > 0, "nft quantity must be positive");
    check(nft_quantity.symbol.nid != 0, "nft symbol is required");

    const name owner = require_tba_owner(tba_id);

    gnos::tba_asset_table assets(get_self(), get_self().value);
    const uint64_t asset_id = gnos::next_id(assets.available_primary_key());
    assets.emplace(owner, [&](auto& row) {
        row.id = asset_id;
        row.tba_id = tba_id;
        row.asset_type = 2;
        row.asset_contract = nft_contract;
        row.asset_ref_id = nft_quantity.symbol.nid;
        row.nft_quantity = nft_quantity;
        row.quantity = asset{0, symbol("NFT", 0)};
        row.settlement_mode = gnos::deliverable;
        row.memo = memo;
    });
}

void tbaaccount::onnfttransfer(name from, name to, std::vector<flon::nasset> nft_assets, std::string memo) {
    if (to != get_self() || from == get_self()) {
        return;
    }

    std::string asset_memo;
    const uint64_t tba_id = parse_tba_deposit_memo(memo, asset_memo);
    if (tba_id == 0) {
        return;
    }

    check(get_first_receiver() == ntoken_contract, "only gnos.ntoken deposits are supported");
    const name owner = require_tba_owner(tba_id);
    check(from == owner, "only current tba owner can deposit nft assets");

    for (const auto& nft_quantity : nft_assets) {
        check(nft_quantity.amount > 0, "nft quantity must be positive");
        check(nft_quantity.symbol.nid != 0, "nft symbol is required");

        gnos::tba_asset_table assets(get_self(), get_self().value);
        const uint64_t asset_id = gnos::next_id(assets.available_primary_key());
        assets.emplace(owner, [&](auto& row) {
            row.id = asset_id;
            row.tba_id = tba_id;
            row.asset_type = 2;
            row.asset_contract = get_first_receiver();
            row.asset_ref_id = nft_quantity.symbol.nid;
            row.nft_quantity = nft_quantity;
            row.quantity = asset{0, symbol("NFT", 0)};
            row.settlement_mode = gnos::deliverable;
            row.memo = asset_memo;
        });
    }
}
