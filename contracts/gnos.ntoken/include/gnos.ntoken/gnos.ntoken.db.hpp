#pragma once

#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>
#include <eosio/eosio.hpp>
#include <eosio/multi_index.hpp>
#include <eosio/singleton.hpp>
#include <eosio/time.hpp>
#include <flon/nasset.hpp>

#include <set>
#include <string>

namespace gnos {

using namespace eosio;
using namespace std;
using flon::nasset;
using flon::nsymbol;

#define GNOS_HASH256(str) sha256(const_cast<char*>(str.c_str()), str.size())
#define GNOS_NTOKEN_TABLE struct [[eosio::table, eosio::contract("gnos.ntoken")]]
#define GNOS_NTOKEN_NAMED_TABLE(name) struct [[eosio::table(name), eosio::contract("gnos.ntoken")]]

GNOS_NTOKEN_NAMED_TABLE("nftglobal") nft_global_t {
    set<name> creators;
    set<name> notaries;
    set<name> whitelist;

    EOSLIB_SERIALIZE(nft_global_t, (creators)(notaries)(whitelist))
};

using nft_global_singleton = singleton<"nftglobal"_n, nft_global_t>;

GNOS_NTOKEN_TABLE nft_stat_t {
    nasset supply;
    nasset max_supply;
    string token_uri;
    name ipowner;
    name notary;
    name issuer;
    time_point_sec issued_at;
    time_point_sec notarized_at;
    bool paused = false;

    uint64_t primary_key() const { return supply.symbol.nid; }
    uint64_t by_ipowner() const { return ipowner.value; }
    uint64_t by_issuer() const { return issuer.value; }
    uint128_t by_issuer_created() const {
        return (uint128_t)issuer.value << 64 | (uint128_t)issued_at.sec_since_epoch();
    }
    checksum256 by_token_uri() const { return GNOS_HASH256(token_uri); }

    using idx_t = multi_index<
        "tokenstats"_n,
        nft_stat_t,
        indexed_by<"ipowneridx"_n, const_mem_fun<nft_stat_t, uint64_t, &nft_stat_t::by_ipowner>>,
        indexed_by<"issueridx"_n, const_mem_fun<nft_stat_t, uint64_t, &nft_stat_t::by_issuer>>,
        indexed_by<"issuercreate"_n, const_mem_fun<nft_stat_t, uint128_t, &nft_stat_t::by_issuer_created>>,
        indexed_by<"tokenuriidx"_n, const_mem_fun<nft_stat_t, checksum256, &nft_stat_t::by_token_uri>>
    >;

    EOSLIB_SERIALIZE(nft_stat_t, (supply)(max_supply)(token_uri)(ipowner)(notary)(issuer)(issued_at)(notarized_at)(paused))
};

GNOS_NTOKEN_TABLE nft_account_t {
    nasset balance;
    bool paused = false;

    nft_account_t() {}
    explicit nft_account_t(const nasset& asset) : balance(asset) {}

    uint64_t primary_key() const { return balance.symbol.nid; }

    using idx_t = multi_index<"accounts"_n, nft_account_t>;

    EOSLIB_SERIALIZE(nft_account_t, (balance)(paused))
};

} // namespace gnos
