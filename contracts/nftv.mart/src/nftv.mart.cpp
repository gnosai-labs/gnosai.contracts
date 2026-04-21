#include <nftv.mart/nftv.mart.hpp>

#include <tuple>

using namespace eosio;

namespace {

static constexpr name ntoken_contract = "gnos.ntoken"_n;
static constexpr name active_permission = "active"_n;

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

struct [[eosio::table("accounts")]] ntoken_account_probe {
    flon::nasset balance;
    bool paused = false;

    uint64_t primary_key() const {
        return balance.symbol.nid;
    }

    EOSLIB_SERIALIZE(ntoken_account_probe, (balance)(paused))
};

using ntoken_account_probe_table = multi_index<"accounts"_n, ntoken_account_probe>;

struct ntoken_action {
    [[eosio::action]]
    void transfer(const name& from, const name& to, const std::vector<flon::nasset>& assets, const std::string& memo);

    using transfer_action = action_wrapper<"transfer"_n, &ntoken_action::transfer>;
};

#define TRANSFER_NFT(bank, from, to, assets, memo) \
    { ntoken_action::transfer_action act{ bank, { {from, active_permission} } }; \
      act.send(from, to, assets, memo); }

void check_nft_quantity(const flon::nasset& nft_quantity) {
    check(nft_quantity.amount > 0, "nft quantity must be positive");
    check(nft_quantity.symbol.nid != 0, "nft symbol is required");
}

void check_balance(name owner, const flon::nasset& nft_quantity) {
    ntoken_account_probe_table accounts(ntoken_contract, owner.value);
    auto itr = accounts.find(nft_quantity.symbol.nid);
    check(itr != accounts.end(), "owner has no nft balance");
    check(!itr->paused, "nft balance is paused");
    check(itr->balance.amount >= nft_quantity.amount, "insufficient nft balance");
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

void nftvmart::check_price(const asset& price) const {
    check(price.is_valid(), "invalid price");
    check(price.amount > 0, "price must be positive");
}

void nftvmart::check_pay_token(name token_contract) const {
    check(token_contract.value != 0, "pay token contract is required");

    gnos::pay_token_table pay_tokens(get_self(), get_self().value);
    auto itr = pay_tokens.find(token_contract.value);
    check(itr != pay_tokens.end() && itr->enabled, "pay token is not whitelisted");
}

gnos::bundle_manifest_t nftvmart::build_manifest(uint64_t tba_id) const {
    gnos::tba_asset_table assets("tba.account"_n, "tba.account"_n.value);
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

    check(!items.empty(), "bundle requires deliverable assets");

    gnos::bundle_manifest_t manifest;
    manifest.tba_id = tba_id;
    manifest.deliverables = items;
    manifest.deliverable_count = items.size();
    manifest.manifest_hash = calc_manifest_hash(tba_id, items);
    manifest.snapshot_at = time_point_sec(current_time_point());
    return manifest;
}

checksum256 nftvmart::calc_manifest_hash(uint64_t tba_id, const std::vector<gnos::bundle_item_t>& items) const {
    auto packed = pack(std::make_tuple(tba_id, items));
    return sha256(packed.data(), packed.size());
}

name nftvmart::get_nft_creator(name nft_contract, flon::nasset nft_quantity) const {
    check(nft_contract == ntoken_contract, "only gnos.ntoken nft is supported");
    ntoken_stat_probe_table stats(nft_contract, nft_contract.value);
    auto itr = stats.find(nft_quantity.symbol.nid);
    check(itr != stats.end(), "nft token does not exist");
    return itr->ipowner.value == 0 ? itr->issuer : itr->ipowner;
}

void nftvmart::transfer_nft(name from, name to, flon::nasset nft_quantity, std::string memo) const {
    TRANSFER_NFT(ntoken_contract, from, to, std::vector<flon::nasset>{nft_quantity}, memo);
}

void nftvmart::distribute_fees(name token_contract, const gnos::order_t& order, name creator, const asset& quantity) {
    gnos::fee_config_singleton config(get_self(), get_self().value);
    const gnos::fee_conf conf = config.get_or_default();
    check(conf.platform_bp + conf.creator_bp + conf.stake_bp <= 10000, "invalid fee config");

    const int64_t platform_amount = static_cast<int64_t>((static_cast<__int128_t>(quantity.amount) * conf.platform_bp) / 10000);
    const int64_t creator_amount = static_cast<int64_t>((static_cast<__int128_t>(quantity.amount) * conf.creator_bp) / 10000);
    const int64_t stake_amount = static_cast<int64_t>((static_cast<__int128_t>(quantity.amount) * conf.stake_bp) / 10000);
    const int64_t seller_amount = quantity.amount - platform_amount - creator_amount - stake_amount;
    check(seller_amount >= 0, "invalid settlement result");

    const asset seller_share{seller_amount, quantity.symbol};
    const asset creator_share{creator_amount, quantity.symbol};
    const asset platform_share{platform_amount, quantity.symbol};
    const asset stake_share{stake_amount, quantity.symbol};

    action(permission_level{get_self(), active_permission}, token_contract, "transfer"_n,
        std::make_tuple(get_self(), order.seller, seller_share, std::string("gnos seller settlement"))).send();

    if (creator_share.amount > 0 && creator != order.seller) {
        action(permission_level{get_self(), active_permission}, token_contract, "transfer"_n,
            std::make_tuple(get_self(), creator, creator_share, std::string("gnos creator royalty"))).send();
    } else if (creator_share.amount > 0) {
        action(permission_level{get_self(), active_permission}, token_contract, "transfer"_n,
            std::make_tuple(get_self(), order.seller, creator_share, std::string("gnos merged creator royalty"))).send();
    }

    if (platform_share.amount > 0) {
        check(conf.platform_account.value != 0, "platform account is not configured");
        action(permission_level{get_self(), active_permission}, token_contract, "transfer"_n,
            std::make_tuple(get_self(), conf.platform_account, platform_share, std::string("gnos platform fee"))).send();
    }

    if (stake_share.amount > 0) {
        check(conf.stake_account.value != 0, "stake account is not configured");
        action(permission_level{get_self(), active_permission}, token_contract, "transfer"_n,
            std::make_tuple(get_self(), conf.stake_account, stake_share, std::string("gnos stake fee"))).send();
    }
}

void nftvmart::setfee(uint16_t platform_bp, uint16_t creator_bp, uint16_t stake_bp, name platform_account, name stake_account) {
    require_auth(get_self());
    check(platform_bp + creator_bp + stake_bp <= 10000, "fee basis points exceed 10000");

    gnos::fee_conf conf;
    conf.platform_bp = platform_bp;
    conf.creator_bp = creator_bp;
    conf.stake_bp = stake_bp;
    conf.platform_account = platform_account;
    conf.stake_account = stake_account;

    gnos::fee_config_singleton config(get_self(), get_self().value);
    config.set(conf, get_self());
}

void nftvmart::listnft(name seller, flon::nasset nft_quantity, name pay_token_contract, asset price) {
    require_auth(seller);
    check_pay_token(pay_token_contract);
    check_price(price);
    check_nft_quantity(nft_quantity);
    check_balance(seller, nft_quantity);

    transfer_nft(seller, get_self(), nft_quantity, "gnos mart escrow");

    gnos::order_table orders(get_self(), get_self().value);
    const uint64_t order_id = gnos::next_id(orders.available_primary_key());
    orders.emplace(seller, [&](auto& row) {
        row.order_id = order_id;
        row.sale_type = gnos::nft_only;
        row.nft_id = nft_quantity.symbol.nid;
        row.nft_contract = ntoken_contract;
        row.nft_quantity = nft_quantity;
        row.seller = seller;
        row.pay_token_contract = pay_token_contract;
        row.price = price;
        row.status = gnos::listed;
        row.created_at = time_point_sec(current_time_point());
    });
}

void nftvmart::listbundle(name seller, uint64_t tba_id, name pay_token_contract, asset price) {
    require_auth(seller);
    check_pay_token(pay_token_contract);
    check_price(price);

    gnos::tba_map_table tbas("tba.reg"_n, "tba.reg"_n.value);
    auto tba_itr = tbas.find(tba_id);
    check(tba_itr != tbas.end(), "tba does not exist");
    check(tba_itr->status == gnos::active, "tba is not active");
    check(tba_itr->owner == seller, "not tba owner");
    check_nft_quantity(tba_itr->root_nft);
    check_balance(seller, tba_itr->root_nft);

    gnos::bundle_manifest_t manifest = build_manifest(tba_id);
    gnos::bundle_manifest_table manifests(get_self(), get_self().value);
    manifest.manifest_id = gnos::next_id(manifests.available_primary_key());
    manifests.emplace(seller, [&](auto& row) {
        row = manifest;
    });

    std::vector<uint64_t> asset_ids;
    for (const auto& item : manifest.deliverables) {
        asset_ids.push_back(item.asset_id);
    }

    action(
        permission_level{get_self(), active_permission},
        "tba.account"_n,
        "freezeassets"_n,
        std::make_tuple(tba_id, asset_ids, true)
    ).send();

    transfer_nft(seller, get_self(), tba_itr->root_nft, "gnos bundle escrow");

    gnos::order_table orders(get_self(), get_self().value);
    const uint64_t order_id = gnos::next_id(orders.available_primary_key());
    orders.emplace(seller, [&](auto& row) {
        row.order_id = order_id;
        row.sale_type = gnos::tba_bundle;
        row.nft_id = tba_itr->root_nft.symbol.nid;
        row.nft_contract = ntoken_contract;
        row.nft_quantity = tba_itr->root_nft;
        row.tba_id = tba_id;
        row.manifest_id = manifest.manifest_id;
        row.manifest_hash = manifest.manifest_hash;
        row.seller = seller;
        row.pay_token_contract = pay_token_contract;
        row.price = price;
        row.status = gnos::listed;
        row.created_at = time_point_sec(current_time_point());
    });
}

void nftvmart::cancel(name seller, uint64_t order_id) {
    require_auth(seller);

    gnos::order_table orders(get_self(), get_self().value);
    auto order_itr = orders.find(order_id);
    check(order_itr != orders.end(), "order does not exist");
    check(order_itr->seller == seller, "not seller");
    check(order_itr->status == gnos::listed, "order is not listed");

    if (order_itr->sale_type == gnos::tba_bundle) {
        gnos::bundle_manifest_table manifests(get_self(), get_self().value);
        auto manifest_itr = manifests.find(order_itr->manifest_id);
        check(manifest_itr != manifests.end(), "manifest does not exist");

        std::vector<uint64_t> asset_ids;
        for (const auto& item : manifest_itr->deliverables) {
            asset_ids.push_back(item.asset_id);
        }

        action(
            permission_level{get_self(), active_permission},
            "tba.account"_n,
            "freezeassets"_n,
            std::make_tuple(order_itr->tba_id, asset_ids, false)
        ).send();
    }

    transfer_nft(get_self(), seller, order_itr->nft_quantity, "gnos mart cancel");

    orders.modify(order_itr, same_payer, [&](auto& row) {
        row.status = gnos::cancelled;
    });
}

void nftvmart::buy(name buyer, uint64_t order_id) {
    require_auth(buyer);

    gnos::order_table orders(get_self(), get_self().value);
    auto order_itr = orders.find(order_id);
    check(order_itr != orders.end(), "order does not exist");
    check(order_itr->status == gnos::listed, "order is not listed");
    check_pay_token(order_itr->pay_token_contract);
    check_balance(get_self(), order_itr->nft_quantity);

    transfer_token(
        order_itr->pay_token_contract,
        buyer,
        get_self(),
        order_itr->price,
        std::string("buy:") + std::to_string(order_id)
    );

    if (order_itr->sale_type == gnos::tba_bundle) {
        gnos::bundle_manifest_table manifests(get_self(), get_self().value);
        auto manifest_itr = manifests.find(order_itr->manifest_id);
        check(manifest_itr != manifests.end(), "manifest does not exist");
        check(manifest_itr->manifest_hash == order_itr->manifest_hash, "manifest hash mismatch");

        std::vector<uint64_t> asset_ids;
        for (const auto& item : manifest_itr->deliverables) {
            asset_ids.push_back(item.asset_id);
        }

        action(
            permission_level{get_self(), active_permission},
            "tba.account"_n,
            "freezeassets"_n,
            std::make_tuple(order_itr->tba_id, asset_ids, false)
        ).send();

        action(
            permission_level{get_self(), active_permission},
            "tba.reg"_n,
            "setowner"_n,
            std::make_tuple(order_itr->tba_id, buyer)
        ).send();
    }

    const name creator = get_nft_creator(order_itr->nft_contract, order_itr->nft_quantity);
    distribute_fees(order_itr->pay_token_contract, *order_itr, creator, order_itr->price);
    transfer_nft(get_self(), buyer, order_itr->nft_quantity, "gnos mart sale");

    orders.modify(order_itr, same_payer, [&](auto& row) {
        row.status = gnos::sold;
        row.buyer = buyer;
        row.sold_at = time_point_sec(current_time_point());
    });
}

void nftvmart::setpaytoken(name token_contract, bool enabled) {
    require_auth(get_self());
    check(token_contract.value != 0, "pay token contract is required");

    gnos::pay_token_table pay_tokens(get_self(), get_self().value);
    auto itr = pay_tokens.find(token_contract.value);
    if (itr == pay_tokens.end()) {
        pay_tokens.emplace(get_self(), [&](auto& row) {
            row.token_contract = token_contract;
            row.enabled = enabled;
        });
    } else {
        pay_tokens.modify(itr, same_payer, [&](auto& row) {
            row.enabled = enabled;
        });
    }
}
