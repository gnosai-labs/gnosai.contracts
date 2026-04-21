#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>

struct [[eosio::table("global"), eosio::contract("song.reg")]] song_reg_global {
    uint32_t nfts_per_issue;
    eosio::asset nft_price;

    EOSLIB_SERIALIZE(song_reg_global, (nfts_per_issue)(nft_price))
};

using song_reg_global_singleton = eosio::singleton<"global"_n, song_reg_global>;

class [[eosio::contract("song.reg")]] songreg : public eosio::contract {
public:
    using contract::contract;

    [[eosio::action]]
    void setglobal(uint32_t nfts_per_issue, eosio::asset nft_price);

    [[eosio::on_notify("cisum.token::transfer")]]
    void ontransfer(eosio::name from, eosio::name to, eosio::asset quantity, std::string memo);

private:
    void create_song_nft(eosio::name creator, uint64_t nft_id, std::string token_uri, uint32_t issue_count, const song_reg_global& gstate);
    void buy_song_nft(eosio::name buyer, uint64_t nft_id, uint32_t buy_count);
    uint32_t parse_songcreate_memo(const std::string& memo, uint64_t& nft_id, std::string& token_uri) const;
    uint32_t parse_songbuy_memo(const std::string& memo, uint64_t& nft_id) const;
};
