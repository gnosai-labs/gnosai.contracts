#include <gnos/gnos.hpp>

#include <eosio/datastream.hpp>
#include <eosio/system.hpp>

namespace eosio {

namespace {

static constexpr uint8_t token_asset_type = 1;

}

void gnos::createsong(name creator, string title, string meta_uri) {
    require_auth(creator);
    check(!title.empty(), "title is required");

    song_table songs(get_self(), get_self().value);
    const uint64_t song_id = next_id(songs.available_primary_key());
    songs.emplace(creator, [&](auto& row) {
        row.song_id = song_id;
        row.creator = creator;
        row.title = title;
        row.meta_uri = meta_uri;
        row.created_at = time_point_sec(current_time_point());
    });
}

void gnos::createmv(name creator, string title, string meta_uri) {
    require_auth(creator);
    check(!title.empty(), "title is required");

    mv_table mvs(get_self(), get_self().value);
    const uint64_t mv_id = next_id(mvs.available_primary_key());
    mvs.emplace(creator, [&](auto& row) {
        row.mv_id = mv_id;
        row.creator = creator;
        row.title = title;
        row.meta_uri = meta_uri;
        row.created_at = time_point_sec(current_time_point());
    });
}

void gnos::mint(name creator, uint64_t content_id, uint8_t content_type_value, string meta_uri) {
    require_auth(creator);
    check_valid_content_type(content_type_value);

    if (content_type_value == song_content) {
        song_table songs(get_self(), get_self().value);
        auto itr = songs.find(content_id);
        check(itr != songs.end(), "song does not exist");
        check(itr->creator == creator, "creator mismatch");
    } else if (content_type_value == mv_content) {
        mv_table mvs(get_self(), get_self().value);
        auto itr = mvs.find(content_id);
        check(itr != mvs.end(), "mv does not exist");
        check(itr->creator == creator, "creator mismatch");
    } else {
        check(false, "bundle content is reserved for protocol use");
    }

    nft_table nfts(get_self(), get_self().value);
    const uint64_t nft_id = next_id(nfts.available_primary_key());
    nfts.emplace(creator, [&](auto& row) {
        row.nft_id = nft_id;
        row.owner = creator;
        row.creator = creator;
        row.content_type = content_type_value;
        row.content_id = content_id;
        row.meta_uri = meta_uri;
    });
}

void gnos::transfernft(name from, name to, uint64_t nft_id) {
    require_auth(from);
    check(is_account(to), "to account does not exist");

    nft_table nfts(get_self(), get_self().value);
    auto itr = nfts.find(nft_id);
    check(itr != nfts.end(), "nft does not exist");
    check(itr->owner == from || itr->approved == from, "missing transfer authority");
    assert_no_open_order_for_nft(nft_id);

    transfer_nft_ownership(nft_id, to);
}

void gnos::approve(name owner, name to, uint64_t nft_id) {
    require_auth(owner);
    check(is_account(to), "approved account does not exist");

    nft_table nfts(get_self(), get_self().value);
    auto itr = nfts.find(nft_id);
    check(itr != nfts.end(), "nft does not exist");
    check(itr->owner == owner, "not nft owner");

    nfts.modify(itr, same_payer, [&](auto& row) {
        row.approved = to;
    });
}

void gnos::revoke(name owner, uint64_t nft_id) {
    require_auth(owner);

    nft_table nfts(get_self(), get_self().value);
    auto itr = nfts.find(nft_id);
    check(itr != nfts.end(), "nft does not exist");
    check(itr->owner == owner, "not nft owner");

    nfts.modify(itr, same_payer, [&](auto& row) {
        row.approved = name{};
    });
}

void gnos::createtba(uint64_t nft_id) {
    nft_table nfts(get_self(), get_self().value);
    auto nft_itr = nfts.find(nft_id);
    check(nft_itr != nfts.end(), "nft does not exist");
    require_auth(nft_itr->owner);
    check(!nft_itr->has_tba, "tba already exists");
    assert_no_open_order_for_nft(nft_id);

    tba_map_table tbas(get_self(), get_self().value);
    const uint64_t tba_id = next_id(tbas.available_primary_key());
    tbas.emplace(nft_itr->owner, [&](auto& row) {
        row.tba_id = tba_id;
        row.nft_id = nft_id;
        row.account_contract = get_self();
        row.status = active;
    });

    nfts.modify(nft_itr, same_payer, [&](auto& row) {
        row.has_tba = true;
        row.tba_id = tba_id;
    });
}

void gnos::deposit(uint64_t tba_id, uint8_t asset_type, name asset_contract, uint128_t asset_ref_id, asset quantity, string memo) {
    tba_map_table tbas(get_self(), get_self().value);
    auto tba_itr = tbas.find(tba_id);
    check(tba_itr != tbas.end(), "tba does not exist");
    check(tba_itr->status == active, "tba is not active");

    nft_table nfts(get_self(), get_self().value);
    auto nft_itr = nfts.find(tba_itr->nft_id);
    check(nft_itr != nfts.end(), "nft does not exist");
    require_auth(nft_itr->owner);
    assert_no_open_order_for_nft(nft_itr->nft_id);

    check(asset_contract.value != 0, "asset contract is required");
    if (asset_type == token_asset_type) {
        check(quantity.is_valid(), "invalid quantity");
        check(quantity.amount > 0, "token quantity must be positive");
    } else {
        check(!quantity.is_valid() || quantity.amount >= 0, "invalid quantity");
    }

    tba_asset_table tba_assets(get_self(), get_self().value);
    const uint64_t asset_id = next_id(tba_assets.available_primary_key());
    tba_assets.emplace(nft_itr->owner, [&](auto& row) {
        row.id = asset_id;
        row.tba_id = tba_id;
        row.asset_type = asset_type;
        row.asset_contract = asset_contract;
        row.asset_ref_id = asset_ref_id;
        row.quantity = quantity;
        row.settlement_mode = deliverable;
        row.memo = memo;
    });
}

void gnos::withdraw(uint64_t tba_id, uint64_t asset_id) {
    tba_map_table tbas(get_self(), get_self().value);
    auto tba_itr = tbas.find(tba_id);
    check(tba_itr != tbas.end(), "tba does not exist");

    nft_table nfts(get_self(), get_self().value);
    auto nft_itr = nfts.find(tba_itr->nft_id);
    check(nft_itr != nfts.end(), "nft does not exist");
    require_auth(nft_itr->owner);
    assert_no_open_order_for_nft(nft_itr->nft_id);

    tba_asset_table tba_assets(get_self(), get_self().value);
    auto asset_itr = tba_assets.find(asset_id);
    check(asset_itr != tba_assets.end(), "asset does not exist");
    check(asset_itr->tba_id == tba_id, "asset does not belong to tba");
    check(!asset_itr->frozen, "asset is frozen");

    tba_assets.erase(asset_itr);
}

void gnos::setmode(uint64_t asset_id, uint8_t settlement_mode_value) {
    check_valid_settlement_mode(settlement_mode_value);

    tba_asset_table tba_assets(get_self(), get_self().value);
    auto asset_itr = tba_assets.find(asset_id);
    check(asset_itr != tba_assets.end(), "asset does not exist");
    check(!asset_itr->frozen, "asset is frozen");

    tba_map_table tbas(get_self(), get_self().value);
    auto tba_itr = tbas.find(asset_itr->tba_id);
    check(tba_itr != tbas.end(), "tba does not exist");

    nft_table nfts(get_self(), get_self().value);
    auto nft_itr = nfts.find(tba_itr->nft_id);
    check(nft_itr != nfts.end(), "nft does not exist");
    require_auth(nft_itr->owner);
    assert_no_open_order_for_nft(nft_itr->nft_id);

    tba_assets.modify(asset_itr, same_payer, [&](auto& row) {
        row.settlement_mode = settlement_mode_value;
    });
}

void gnos::genmanifest(uint64_t tba_id) {
    tba_map_table tbas(get_self(), get_self().value);
    auto tba_itr = tbas.find(tba_id);
    check(tba_itr != tbas.end(), "tba does not exist");
    check(tba_itr->status == active, "tba is not active");

    nft_table nfts(get_self(), get_self().value);
    auto nft_itr = nfts.find(tba_itr->nft_id);
    check(nft_itr != nfts.end(), "nft does not exist");
    require_auth(nft_itr->owner);

    auto manifest = make_manifest_snapshot(tba_id);
    store_manifest(manifest, nft_itr->owner);
}

void gnos::listnft(name seller, uint64_t nft_id, asset price) {
    require_auth(seller);
    check_valid_sale_price(price);

    nft_table nfts(get_self(), get_self().value);
    auto nft_itr = nfts.find(nft_id);
    check(nft_itr != nfts.end(), "nft does not exist");
    check(nft_itr->owner == seller, "not nft owner");
    assert_no_open_order_for_nft(nft_id);

    if (nft_itr->has_tba) {
        check(!tba_has_assets(nft_itr->tba_id), "nft with non-empty tba cannot be listed as nft_only");
    }

    order_table orders(get_self(), get_self().value);
    const uint64_t order_id = next_id(orders.available_primary_key());
    orders.emplace(seller, [&](auto& row) {
        row.order_id = order_id;
        row.sale_type = nft_only;
        row.nft_id = nft_id;
        row.seller = seller;
        row.price = price;
        row.status = listed;
        row.created_at = time_point_sec(current_time_point());
    });
}

void gnos::listbundle(name seller, uint64_t nft_id, uint64_t tba_id, asset price) {
    require_auth(seller);
    check_valid_sale_price(price);

    nft_table nfts(get_self(), get_self().value);
    auto nft_itr = nfts.find(nft_id);
    check(nft_itr != nfts.end(), "nft does not exist");
    check(nft_itr->owner == seller, "not nft owner");
    check(nft_itr->has_tba, "nft has no tba");
    check(nft_itr->tba_id == tba_id, "nft and tba mismatch");
    assert_no_open_order_for_nft(nft_id);

    tba_map_table tbas(get_self(), get_self().value);
    auto tba_itr = tbas.find(tba_id);
    check(tba_itr != tbas.end(), "tba does not exist");
    check(tba_itr->status == active, "tba is not active");

    auto manifest = make_manifest_snapshot(tba_id);
    manifest = store_manifest(manifest, seller);
    freeze_manifest_assets(tba_id, manifest, true);

    order_table orders(get_self(), get_self().value);
    const uint64_t order_id = next_id(orders.available_primary_key());
    orders.emplace(seller, [&](auto& row) {
        row.order_id = order_id;
        row.sale_type = tba_bundle;
        row.nft_id = nft_id;
        row.tba_id = tba_id;
        row.manifest_id = manifest.manifest_id;
        row.manifest_hash = manifest.manifest_hash;
        row.seller = seller;
        row.price = price;
        row.status = listed;
        row.created_at = time_point_sec(current_time_point());
    });
}

void gnos::cancel(name seller, uint64_t order_id) {
    require_auth(seller);

    order_table orders(get_self(), get_self().value);
    auto order_itr = orders.find(order_id);
    check(order_itr != orders.end(), "order does not exist");
    check(order_itr->seller == seller, "not seller");
    check(order_itr->status == listed, "order is not listed");

    if (order_itr->sale_type == tba_bundle) {
        bundle_manifest_table manifests(get_self(), get_self().value);
        auto manifest_itr = manifests.find(order_itr->manifest_id);
        check(manifest_itr != manifests.end(), "manifest does not exist");
        freeze_manifest_assets(order_itr->tba_id, *manifest_itr, false);
    }

    orders.modify(order_itr, same_payer, [&](auto& row) {
        row.status = cancelled;
    });
}

void gnos::setfee(uint16_t platform_bp, uint16_t creator_bp, uint16_t stake_bp, name platform_account, name stake_account) {
    require_auth(get_self());
    check(platform_bp + creator_bp + stake_bp <= 10000, "fee basis points exceed 10000");
    if (platform_bp > 0) {
        check(platform_account.value != 0, "platform account is required");
    }
    if (stake_bp > 0) {
        check(stake_account.value != 0, "stake account is required");
    }

    fee_config_singleton config(get_self(), get_self().value);
    fee_conf value;
    value.platform_bp = platform_bp;
    value.creator_bp = creator_bp;
    value.stake_bp = stake_bp;
    value.platform_account = platform_account;
    value.stake_account = stake_account;
    config.set(value, get_self());
}

void gnos::on_transfer(name from, name to, asset quantity, string memo) {
    if (to != get_self() || from == get_self()) {
        return;
    }

    const uint64_t order_id = parse_buy_order_id(memo);
    if (order_id == 0) {
        return;
    }

    check(quantity.is_valid(), "invalid quantity");
    check(quantity.amount > 0, "quantity must be positive");

    order_table orders(get_self(), get_self().value);
    auto order_itr = orders.find(order_id);
    check(order_itr != orders.end(), "order does not exist");
    check(order_itr->status == listed, "order is not listed");
    check(order_itr->price == quantity, "price mismatch");

    nft_table nfts(get_self(), get_self().value);
    auto nft_itr = nfts.find(order_itr->nft_id);
    check(nft_itr != nfts.end(), "nft does not exist");
    check(nft_itr->owner == order_itr->seller, "seller no longer owns nft");

    if (order_itr->sale_type == tba_bundle) {
        bundle_manifest_table manifests(get_self(), get_self().value);
        auto manifest_itr = manifests.find(order_itr->manifest_id);
        check(manifest_itr != manifests.end(), "manifest does not exist");
        check(manifest_itr->manifest_hash == order_itr->manifest_hash, "manifest hash mismatch");
        freeze_manifest_assets(order_itr->tba_id, *manifest_itr, false);
    }

    settle_sale(get_first_receiver(), *order_itr, *nft_itr, quantity);
    transfer_nft_ownership(order_itr->nft_id, from);

    orders.modify(order_itr, same_payer, [&](auto& row) {
        row.status = sold;
        row.buyer = from;
        row.sold_at = time_point_sec(current_time_point());
    });
}

void gnos::check_valid_content_type(uint8_t value) const {
    check(value == song_content || value == mv_content || value == bundle_content, "invalid content type");
}

void gnos::check_valid_settlement_mode(uint8_t value) const {
    check(
        value == deliverable || value == control_only || value == excluded || value == blocking,
        "invalid settlement mode"
    );
}

void gnos::check_valid_sale_price(const asset& price) const {
    check(price.is_valid(), "invalid price");
    check(price.amount > 0, "price must be positive");
}

uint64_t gnos::next_id(uint64_t value) const {
    return value == 0 ? 1 : value;
}

uint64_t gnos::parse_buy_order_id(const string& memo) const {
    static constexpr const char* prefix = "buy:";
    if (memo.size() <= 4 || memo.compare(0, 4, prefix) != 0) {
        return 0;
    }

    uint64_t value = 0;
    for (size_t i = 4; i < memo.size(); ++i) {
        const char c = memo[i];
        check(c >= '0' && c <= '9', "invalid buy memo");
        value = value * 10 + static_cast<uint64_t>(c - '0');
    }
    return value;
}

bool gnos::tba_has_assets(uint64_t tba_id) const {
    tba_asset_table tba_assets(get_self(), get_self().value);
    auto idx = tba_assets.get_index<"bytba"_n>();
    auto itr = idx.lower_bound(tba_id);
    return itr != idx.end() && itr->tba_id == tba_id;
}

void gnos::assert_no_open_order_for_nft(uint64_t nft_id) const {
    order_table orders(get_self(), get_self().value);
    auto idx = orders.get_index<"bynft"_n>();
    auto itr = idx.lower_bound(nft_id);
    for (; itr != idx.end() && itr->nft_id == nft_id; ++itr) {
        check(itr->status != listed, "nft has an active order");
    }
}

bundle_manifest_t gnos::make_manifest_snapshot(uint64_t tba_id) const {
    tba_asset_table tba_assets(get_self(), get_self().value);
    auto idx = tba_assets.get_index<"bytba"_n>();
    auto itr = idx.lower_bound(tba_id);

    vector<bundle_item_t> deliverables;
    vector<uint64_t> deliverable_asset_ids;
    for (; itr != idx.end() && itr->tba_id == tba_id; ++itr) {
        check(itr->settlement_mode != blocking, "blocking asset exists in tba");
        if (itr->settlement_mode == deliverable) {
            bundle_item_t item;
            item.asset_type = itr->asset_type;
            item.asset_contract = itr->asset_contract;
            item.asset_ref_id = itr->asset_ref_id;
            item.quantity = itr->quantity;
            deliverables.push_back(item);
            deliverable_asset_ids.push_back(itr->id);
        }
    }

    check(!deliverables.empty(), "bundle requires at least one deliverable asset");

    bundle_manifest_t draft;
    draft.tba_id = tba_id;
    draft.deliverables = deliverables;
    draft.deliverable_asset_ids = deliverable_asset_ids;
    draft.deliverable_count = deliverables.size();
    draft.manifest_hash = calculate_manifest_hash(tba_id, deliverables);
    draft.snapshot_at = time_point_sec(current_time_point());
    return draft;
}

bundle_manifest_t gnos::store_manifest(const bundle_manifest_t& draft, name ram_payer) {
    bundle_manifest_table manifests(get_self(), get_self().value);
    bundle_manifest_t stored = draft;
    stored.manifest_id = next_id(manifests.available_primary_key());

    manifests.emplace(ram_payer, [&](auto& row) {
        row = stored;
    });
    return stored;
}

void gnos::freeze_manifest_assets(uint64_t tba_id, const bundle_manifest_t& manifest, bool frozen_state) {
    tba_asset_table tba_assets(get_self(), get_self().value);
    check(manifest.deliverables.size() == manifest.deliverable_asset_ids.size(), "manifest asset snapshot mismatch");

    for (size_t i = 0; i < manifest.deliverable_asset_ids.size(); ++i) {
        const uint64_t asset_id = manifest.deliverable_asset_ids[i];
        const auto& item = manifest.deliverables[i];
        auto asset_itr = tba_assets.find(asset_id);
        check(asset_itr != tba_assets.end(), "manifest asset not found");
        check(asset_itr->tba_id == tba_id, "manifest asset tba mismatch");
        check(asset_itr->settlement_mode == deliverable, "manifest asset mode changed");
        check(asset_itr->asset_type == item.asset_type, "manifest asset type changed");
        check(asset_itr->asset_contract == item.asset_contract, "manifest asset contract changed");
        check(asset_itr->asset_ref_id == item.asset_ref_id, "manifest asset ref changed");
        check(asset_itr->quantity == item.quantity, "manifest asset quantity changed");
        if (frozen_state) {
            check(!asset_itr->frozen, "deliverable asset already frozen");
        }
        tba_assets.modify(asset_itr, same_payer, [&](auto& row) {
            row.frozen = frozen_state;
        });
    }
}

checksum256 gnos::calculate_manifest_hash(uint64_t tba_id, const vector<bundle_item_t>& deliverables) const {
    auto packed = pack(std::make_tuple(tba_id, deliverables));
    return sha256(packed.data(), packed.size());
}

void gnos::transfer_nft_ownership(uint64_t nft_id, name new_owner) {
    nft_table nfts(get_self(), get_self().value);
    auto itr = nfts.find(nft_id);
    check(itr != nfts.end(), "nft does not exist");
    nfts.modify(itr, same_payer, [&](auto& row) {
        row.owner = new_owner;
        row.approved = name{};
    });
}

void gnos::settle_sale(name token_contract, const order_t& order, const nft_t& nft, const asset& quantity) {
    fee_config_singleton config_singleton(get_self(), get_self().value);
    const fee_conf config = config_singleton.get_or_default();
    check(config.platform_bp + config.creator_bp + config.stake_bp <= 10000, "invalid fee config");

    const int64_t platform_amount = static_cast<int64_t>((static_cast<__int128_t>(quantity.amount) * config.platform_bp) / 10000);
    const int64_t creator_amount = static_cast<int64_t>((static_cast<__int128_t>(quantity.amount) * config.creator_bp) / 10000);
    const int64_t stake_amount = static_cast<int64_t>((static_cast<__int128_t>(quantity.amount) * config.stake_bp) / 10000);
    const int64_t seller_amount = quantity.amount - platform_amount - creator_amount - stake_amount;
    check(seller_amount >= 0, "invalid settlement result");

    const asset seller_share{seller_amount, quantity.symbol};
    const asset creator_share{creator_amount, quantity.symbol};
    const asset platform_share{platform_amount, quantity.symbol};
    const asset stake_share{stake_amount, quantity.symbol};

    action(permission_level{get_self(), "active"_n}, token_contract, "transfer"_n,
        std::make_tuple(get_self(), order.seller, seller_share, string("gnos seller settlement")))
        .send();

    if (creator_share.amount > 0 && nft.creator != order.seller) {
        action(permission_level{get_self(), "active"_n}, token_contract, "transfer"_n,
            std::make_tuple(get_self(), nft.creator, creator_share, string("gnos creator royalty")))
            .send();
    } else if (creator_share.amount > 0) {
        action(permission_level{get_self(), "active"_n}, token_contract, "transfer"_n,
            std::make_tuple(get_self(), order.seller, creator_share, string("gnos merged creator royalty")))
            .send();
    }

    if (platform_share.amount > 0) {
        check(config.platform_account.value != 0, "platform account is not configured");
        action(permission_level{get_self(), "active"_n}, token_contract, "transfer"_n,
            std::make_tuple(get_self(), config.platform_account, platform_share, string("gnos platform fee")))
            .send();
    }

    if (stake_share.amount > 0) {
        check(config.stake_account.value != 0, "stake account is not configured");
        action(permission_level{get_self(), "active"_n}, token_contract, "transfer"_n,
            std::make_tuple(get_self(), config.stake_account, stake_share, string("gnos stake fee")))
            .send();
    }
}

} // namespace eosio
