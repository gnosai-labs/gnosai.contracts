#pragma once

#include <eosio/eosio.hpp>
#include <gnos.protocol/gnos.protocol.hpp>

class [[eosio::contract("gnos.dao")]] gnosdao : public eosio::contract {
public:
    using contract::contract;

    [[eosio::action]]
    void setfee(uint16_t platform_bp, uint16_t creator_bp, uint16_t stake_bp, eosio::name platform_account, eosio::name stake_account);
};
