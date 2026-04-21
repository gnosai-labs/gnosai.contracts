#pragma once

#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>
#include <eosio/eosio.hpp>
#include <eosio/multi_index.hpp>
#include <eosio/singleton.hpp>
#include <flon/nasset.hpp>

#include <string>
#include <vector>

namespace gnos {

using eosio::asset;
using eosio::checksum256;
using eosio::indexed_by;
using eosio::multi_index;
using eosio::name;
using eosio::singleton;
using eosio::time_point_sec;
using flon::nasset;
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

struct [[eosio::table]] song_t {
    uint64_t song_id;
    name creator;
    string title;
    string token_uri;
    checksum256 song_hash;
    uint32_t issue_count = 0;
    uint32_t nft_supply = 0;
    time_point_sec created_at;

    uint64_t primary_key() const { return song_id; }
    checksum256 by_hash() const { return song_hash; }
};

struct [[eosio::table]] mv_t {
    uint64_t mv_id;
    uint64_t song_id = 0;
    name creator;
    string title;
    string mv_uri;
    checksum256 mv_hash;
    string meta_uri;
    time_point_sec created_at;

    uint64_t primary_key() const { return mv_id; }
    uint64_t by_song() const { return song_id; }
    checksum256 by_hash() const { return mv_hash; }
};

struct [[eosio::table]] nft_t {
    uint64_t nft_id;
    uint64_t token_id = 0;
    name owner;
    name creator;
    uint8_t content_type;
    uint64_t content_id;
    uint64_t song_id = 0;
    uint64_t mv_id = 0;
    string meta_uri;
    bool has_tba = false;
    uint64_t tba_id = 0;
    name approved;

    uint64_t primary_key() const { return nft_id; }
    uint64_t by_owner() const { return owner.value; }
    uint64_t by_token() const { return token_id; }
};

struct [[eosio::table]] nft_stat_t {
    uint64_t token_id;
    int64_t supply = 0;
    int64_t max_supply = 0;
    string token_uri;
    name ipowner;
    name issuer;
    uint8_t content_type = 0;
    uint64_t content_id = 0;
    uint64_t song_id = 0;
    uint64_t mv_id = 0;
    string meta_uri;
    time_point_sec issued_at;
    bool paused = false;

    uint64_t primary_key() const { return token_id; }
    uint64_t by_ipowner() const { return ipowner.value; }
    uint64_t by_issuer() const { return issuer.value; }
};

struct [[eosio::table]] nft_account_t {
    uint64_t token_id;
    int64_t balance = 0;
    bool paused = false;

    uint64_t primary_key() const { return token_id; }
};

struct [[eosio::table]] tba_map_t {
    uint64_t tba_id;
    uint64_t nft_id;
    name owner;
    name nft_contract;
    nasset root_nft;
    name account_contract;
    uint8_t status;

    uint64_t primary_key() const { return tba_id; }
    uint64_t by_nft() const { return root_nft.symbol.nid != 0 ? root_nft.symbol.nid : nft_id; }
    uint64_t by_owner() const { return owner.value; }
};

struct [[eosio::table]] tba_asset_t {
    uint64_t id;
    uint64_t tba_id;
    uint8_t asset_type;
    name asset_contract;
    unsigned __int128 asset_ref_id;
    asset quantity;
    nasset nft_quantity;
    uint8_t settlement_mode;
    bool frozen = false;
    string memo;

    uint64_t primary_key() const { return id; }
    uint64_t by_tba() const { return tba_id; }
};

struct bundle_item_t {
    uint64_t asset_id = 0;
    uint8_t asset_type = 0;
    name asset_contract;
    unsigned __int128 asset_ref_id = 0;
    asset quantity;
    nasset nft_quantity;
};

struct [[eosio::table]] bundle_manifest_t {
    uint64_t manifest_id;
    uint64_t tba_id;
    vector<bundle_item_t> deliverables;
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
    name nft_contract;
    nasset nft_quantity;
    uint64_t tba_id = 0;
    uint64_t manifest_id = 0;
    checksum256 manifest_hash;
    name seller;
    name pay_token_contract;
    asset price;
    uint8_t status;
    time_point_sec created_at;
    time_point_sec sold_at;
    name buyer;

    uint64_t primary_key() const { return order_id; }
    uint64_t by_nft() const { return nft_quantity.symbol.nid != 0 ? nft_quantity.symbol.nid : nft_id; }
    uint64_t by_seller() const { return seller.value; }
};

struct [[eosio::table]] fee_conf {
    uint16_t platform_bp = 0;
    uint16_t creator_bp = 0;
    uint16_t stake_bp = 0;
    name platform_account;
    name stake_account;
};

struct [[eosio::table]] pay_token_t {
    name token_contract;
    bool enabled = true;

    uint64_t primary_key() const { return token_contract.value; }
};

struct [[eosio::table]] stake_plan_t {
    uint64_t plan_id;
    uint64_t nft_id = 0;
    name stake_token_contract;
    name reward_token_contract;
    asset total_staked;
    asset total_reward;
    asset claimed_reward;
    asset undistributed_reward;
    uint64_t reward_per_share = 0;
    bool active = true;

    uint64_t primary_key() const { return plan_id; }
    uint64_t by_nft() const { return nft_id; }
};

struct [[eosio::table]] stake_position_t {
    uint64_t position_id;
    uint64_t plan_id = 0;
    name staker;
    asset staked;
    uint64_t reward_per_share_paid = 0;
    asset pending_reward;

    uint64_t primary_key() const { return position_id; }
    uint64_t by_plan() const { return plan_id; }
    uint64_t by_staker() const { return staker.value; }
};

using song_table = multi_index<
    "songs"_n, song_t,
    indexed_by<"byhash"_n, eosio::const_mem_fun<song_t, checksum256, &song_t::by_hash>>
>;
using mv_table = multi_index<
    "mvs"_n, mv_t,
    indexed_by<"bysong"_n, eosio::const_mem_fun<mv_t, uint64_t, &mv_t::by_song>>,
    indexed_by<"byhash"_n, eosio::const_mem_fun<mv_t, checksum256, &mv_t::by_hash>>
>;
using nft_table = multi_index<
    "nfts"_n, nft_t,
    indexed_by<"byowner"_n, eosio::const_mem_fun<nft_t, uint64_t, &nft_t::by_owner>>,
    indexed_by<"bytoken"_n, eosio::const_mem_fun<nft_t, uint64_t, &nft_t::by_token>>
>;
using nft_stat_table = multi_index<
    "nftstats"_n, nft_stat_t,
    indexed_by<"byipowner"_n, eosio::const_mem_fun<nft_stat_t, uint64_t, &nft_stat_t::by_ipowner>>,
    indexed_by<"byissuer"_n, eosio::const_mem_fun<nft_stat_t, uint64_t, &nft_stat_t::by_issuer>>
>;
using nft_account_table = multi_index<"accounts"_n, nft_account_t>;
using tba_map_table = multi_index<
    "tbamaps"_n, tba_map_t,
    indexed_by<"bynft"_n, eosio::const_mem_fun<tba_map_t, uint64_t, &tba_map_t::by_nft>>,
    indexed_by<"byowner"_n, eosio::const_mem_fun<tba_map_t, uint64_t, &tba_map_t::by_owner>>
>;
using tba_asset_table = multi_index<
    "tbaassets"_n, tba_asset_t,
    indexed_by<"bytba"_n, eosio::const_mem_fun<tba_asset_t, uint64_t, &tba_asset_t::by_tba>>
>;
using bundle_manifest_table = multi_index<
    "manifests"_n, bundle_manifest_t,
    indexed_by<"bytba"_n, eosio::const_mem_fun<bundle_manifest_t, uint64_t, &bundle_manifest_t::by_tba>>
>;
using order_table = multi_index<
    "orders"_n, order_t,
    indexed_by<"bynft"_n, eosio::const_mem_fun<order_t, uint64_t, &order_t::by_nft>>,
    indexed_by<"byseller"_n, eosio::const_mem_fun<order_t, uint64_t, &order_t::by_seller>>
>;
using fee_config_singleton = singleton<"feeconfig"_n, fee_conf>;
using pay_token_table = multi_index<"paytokens"_n, pay_token_t>;
using stake_plan_table = multi_index<
    "plans"_n, stake_plan_t,
    indexed_by<"bynft"_n, eosio::const_mem_fun<stake_plan_t, uint64_t, &stake_plan_t::by_nft>>
>;
using stake_position_table = multi_index<
    "positions"_n, stake_position_t,
    indexed_by<"byplan"_n, eosio::const_mem_fun<stake_position_t, uint64_t, &stake_position_t::by_plan>>,
    indexed_by<"bystaker"_n, eosio::const_mem_fun<stake_position_t, uint64_t, &stake_position_t::by_staker>>
>;

inline uint64_t next_id(uint64_t value) {
    return value == 0 ? 1 : value;
}

inline void check_valid_settlement_mode(uint8_t value) {
    eosio::check(
        value == deliverable || value == control_only || value == excluded || value == blocking,
        "invalid settlement mode"
    );
}

inline void check_valid_content_type(uint8_t value) {
    eosio::check(
        value == song_content || value == mv_content || value == bundle_content,
        "invalid content type"
    );
}

} // namespace gnos
