#pragma once

#include <eosio/eosio.hpp>
#include <gnos.protocol/gnos.protocol.hpp>

class [[eosio::contract("mv.reg")]] mvreg : public eosio::contract {
public:
    using contract::contract;

    [[eosio::action]]
    void createmv(eosio::name creator, uint64_t song_id, std::string title, std::string mv_uri, eosio::checksum256 mv_hash, std::string meta_uri);
};
