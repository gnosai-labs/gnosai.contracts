#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/multi_index.hpp>
#include <eosio/singleton.hpp>

#include <string>

struct [[eosio::table("global"), eosio::contract("song.reg")]] song_reg_global {
    uint32_t nfts_per_issue;
    eosio::asset nft_price;

    EOSLIB_SERIALIZE(song_reg_global, (nfts_per_issue)(nft_price))
};

using song_reg_global_singleton = eosio::singleton<"global"_n, song_reg_global>;

struct [[eosio::table("songs"), eosio::contract("song.reg")]] song_info {
    uint64_t nid;
    eosio::name creator;
    std::string music_hash;
    std::string music_url;
    eosio::time_point_sec create_at;
    eosio::time_point_sec update_at;

    uint64_t primary_key() const { return nid; }

    EOSLIB_SERIALIZE(song_info, (nid)(creator)(music_hash)(music_url)(create_at)(update_at))
};

using song_table = eosio::multi_index<"songs"_n, song_info>;

class [[eosio::contract("song.reg")]] songreg : public eosio::contract {
public:
    using contract::contract;

    [[eosio::action]]
    void setglobal(uint32_t nfts_per_issue, eosio::asset nft_price);

    [[eosio::on_notify("cisum.token::transfer")]]
    void ontransfer(eosio::name from, eosio::name to, eosio::asset quantity, std::string memo);

private:
    void create_song_nft(eosio::name creator, uint64_t nft_id, std::string token_uri, std::string music_hash, std::string music_url, uint32_t issue_count, const song_reg_global& gstate);
    void buy_song_nft(eosio::name buyer, uint64_t nft_id, uint32_t buy_count);
    uint32_t parse_songcreate_memo(const std::string& memo, uint64_t& nft_id, std::string& token_uri, std::string& music_hash, std::string& music_url) const;
    uint32_t parse_songbuy_memo(const std::string& memo, uint64_t& nft_id) const;
};
