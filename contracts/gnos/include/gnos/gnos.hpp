#pragma once

#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>
#include <eosio/eosio.hpp>
#include <eosio/multi_index.hpp>
#include <eosio/singleton.hpp>

#include <string>
#include <tuple>
#include <vector>

namespace eosio {

using std::string;
using std::vector;

enum content_type : uint8_t {
    song_content = 1,
    mv_content = 2,
    bundle_content = 3
};

enum settlement_mode : uint8_t {
    deliverable = 1,
    control_only = 2,
    excluded = 3,
    blocking = 4
};

enum sale_type : uint8_t {
    nft_only = 1,
    tba_bundle = 2
};

enum order_status : uint8_t {
    listed = 1,
    sold = 2,
    cancelled = 3
};

enum tba_status : uint8_t {
    active = 1,
    frozen = 2,
    closed = 3
};

struct [[eosio::table]] nft_t {
    uint64_t nft_id;
    name owner;
    name creator;
    uint8_t content_type;
    uint64_t content_id;
    string meta_uri;
    bool has_tba = false;
    uint64_t tba_id = 0;
    name approved;

    uint64_t primary_key() const { return nft_id; }
    uint64_t by_owner() const { return owner.value; }
};

struct [[eosio::table]] tba_map_t {
    uint64_t tba_id;
    uint64_t nft_id;
    name account_contract;
    uint8_t status;

    uint64_t primary_key() const { return tba_id; }
    uint64_t by_nft() const { return nft_id; }
};

struct [[eosio::table]] tba_asset_t {
    uint64_t id;
    uint64_t tba_id;
    uint8_t asset_type;
    name asset_contract;
    uint128_t asset_ref_id;
    asset quantity;
    uint8_t settlement_mode;
    bool frozen = false;
    string memo;

    uint64_t primary_key() const { return id; }
    uint64_t by_tba() const { return tba_id; }
};

struct bundle_item_t {
    uint8_t asset_type;
    name asset_contract;
    uint128_t asset_ref_id;
    asset quantity;
};

struct [[eosio::table]] bundle_manifest_t {
    uint64_t manifest_id;
    uint64_t tba_id;
    vector<bundle_item_t> deliverables;
    vector<uint64_t> deliverable_asset_ids;
    uint64_t deliverable_count = 0;
    checksum256 manifest_hash;
    time_point_sec snapshot_at;

    uint64_t primary_key() const { return manifest_id; }
    uint64_t by_tba() const { return tba_id; }
};

struct [[eosio::table]] order_t {
    uint64_t order_id;
    uint8_t sale_type;
    uint64_t nft_id;
    uint64_t tba_id = 0;
    uint64_t manifest_id = 0;
    checksum256 manifest_hash;
    name seller;
    asset price;
    uint8_t status;
    time_point_sec created_at;
    time_point_sec sold_at;
    name buyer;

    uint64_t primary_key() const { return order_id; }
    uint64_t by_nft() const { return nft_id; }
    uint64_t by_seller() const { return seller.value; }
};

struct [[eosio::table]] fee_conf {
    uint16_t platform_bp = 0;
    uint16_t creator_bp = 0;
    uint16_t stake_bp = 0;
    name platform_account;
    name stake_account;
};

struct [[eosio::table]] song_t {
    uint64_t song_id;
    name creator;
    string title;
    string meta_uri;
    time_point_sec created_at;

    uint64_t primary_key() const { return song_id; }
};

struct [[eosio::table]] mv_t {
    uint64_t mv_id;
    name creator;
    string title;
    string meta_uri;
    time_point_sec created_at;

    uint64_t primary_key() const { return mv_id; }
};

class [[eosio::contract("gnos")]] gnos : public contract {
public:
    using contract::contract;

    [[eosio::action]]
    void createsong(name creator, string title, string meta_uri);

    [[eosio::action]]
    void createmv(name creator, string title, string meta_uri);

    [[eosio::action]]
    void mint(name creator, uint64_t content_id, uint8_t content_type, string meta_uri);

    [[eosio::action]]
    void transfernft(name from, name to, uint64_t nft_id);

    [[eosio::action]]
    void approve(name owner, name to, uint64_t nft_id);

    [[eosio::action]]
    void revoke(name owner, uint64_t nft_id);

    [[eosio::action]]
    void createtba(uint64_t nft_id);

    [[eosio::action]]
    void deposit(uint64_t tba_id, uint8_t asset_type, name asset_contract, uint128_t asset_ref_id, asset quantity, string memo);

    [[eosio::action]]
    void withdraw(uint64_t tba_id, uint64_t asset_id);

    [[eosio::action]]
    void setmode(uint64_t asset_id, uint8_t settlement_mode);

    [[eosio::action]]
    void genmanifest(uint64_t tba_id);

    [[eosio::action]]
    void listnft(name seller, uint64_t nft_id, asset price);

    [[eosio::action]]
    void listbundle(name seller, uint64_t nft_id, uint64_t tba_id, asset price);

    [[eosio::action]]
    void cancel(name seller, uint64_t order_id);

    [[eosio::action]]
    void setfee(uint16_t platform_bp, uint16_t creator_bp, uint16_t stake_bp, name platform_account, name stake_account);

    [[eosio::on_notify("*::transfer")]]
    void on_transfer(name from, name to, asset quantity, string memo);

private:
    using nft_table = multi_index<
        "nfts"_n, nft_t,
        indexed_by<"byowner"_n, const_mem_fun<nft_t, uint64_t, &nft_t::by_owner>>
    >;

    using tba_map_table = multi_index<
        "tbamaps"_n, tba_map_t,
        indexed_by<"bynft"_n, const_mem_fun<tba_map_t, uint64_t, &tba_map_t::by_nft>>
    >;

    using tba_asset_table = multi_index<
        "tbaassets"_n, tba_asset_t,
        indexed_by<"bytba"_n, const_mem_fun<tba_asset_t, uint64_t, &tba_asset_t::by_tba>>
    >;

    using bundle_manifest_table = multi_index<
        "manifests"_n, bundle_manifest_t,
        indexed_by<"bytba"_n, const_mem_fun<bundle_manifest_t, uint64_t, &bundle_manifest_t::by_tba>>
    >;

    using order_table = multi_index<
        "orders"_n, order_t,
        indexed_by<"bynft"_n, const_mem_fun<order_t, uint64_t, &order_t::by_nft>>,
        indexed_by<"byseller"_n, const_mem_fun<order_t, uint64_t, &order_t::by_seller>>
    >;

    using song_table = multi_index<"songs"_n, song_t>;
    using mv_table = multi_index<"mvs"_n, mv_t>;
    using fee_config_singleton = singleton<"feeconfig"_n, fee_conf>;

    void check_valid_content_type(uint8_t value) const;
    void check_valid_settlement_mode(uint8_t value) const;
    void check_valid_sale_price(const asset& price) const;
    uint64_t next_id(uint64_t value) const;
    uint64_t parse_buy_order_id(const string& memo) const;
    bool tba_has_assets(uint64_t tba_id) const;
    void assert_no_open_order_for_nft(uint64_t nft_id) const;
    bundle_manifest_t make_manifest_snapshot(uint64_t tba_id) const;
    bundle_manifest_t store_manifest(const bundle_manifest_t& draft, name ram_payer);
    void freeze_manifest_assets(uint64_t tba_id, const bundle_manifest_t& manifest, bool frozen_state);
    checksum256 calculate_manifest_hash(uint64_t tba_id, const vector<bundle_item_t>& deliverables) const;
    void transfer_nft_ownership(uint64_t nft_id, name new_owner);
    void settle_sale(name token_contract, const order_t& order, const nft_t& nft, const asset& quantity);
};

} // namespace eosio
