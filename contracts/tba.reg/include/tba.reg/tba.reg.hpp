#pragma once

#include <eosio/eosio.hpp>
#include <gnos.protocol/gnos.protocol.hpp>

class [[eosio::contract("tba.reg")]] tbareg : public eosio::contract {
public:
    using contract::contract;

    [[eosio::action]]
    void createtba(eosio::name owner, eosio::name nft_contract, flon::nasset root_nft);

    [[eosio::action]]
    void setstatus(uint64_t tba_id, uint8_t status);

    [[eosio::action]]
    void setowner(uint64_t tba_id, eosio::name owner);
};
