#pragma once

#include <eosio/eosio.hpp>
#include <gnos.protocol/gnos.protocol.hpp>

class [[eosio::contract("tba.account")]] tbaaccount : public eosio::contract {
public:
    using contract::contract;

    [[eosio::action]]
    void deposit(uint64_t tba_id, uint8_t asset_type, eosio::name asset_contract, unsigned __int128 asset_ref_id, eosio::asset quantity, std::string memo);

    [[eosio::action]]
    void deposittoken(eosio::name owner, uint64_t tba_id, eosio::name token_contract, eosio::asset quantity, std::string memo);

    [[eosio::action]]
    void withdraw(uint64_t tba_id, uint64_t asset_id);

    [[eosio::action]]
    void setmode(uint64_t asset_id, uint8_t settlement_mode);

    [[eosio::action]]
    void freezeassets(uint64_t tba_id, std::vector<uint64_t> asset_ids, bool frozen_state);

    [[eosio::action]]
    void genmanifest(uint64_t tba_id);

    [[eosio::action]]
    void receivenft(uint64_t tba_id, eosio::name nft_contract, flon::nasset nft_quantity, std::string memo);

    [[eosio::on_notify("gnos.ntoken::transfer")]]
    void onnfttransfer(eosio::name from, eosio::name to, std::vector<flon::nasset> assets, std::string memo);

private:
    eosio::name require_tba_owner(uint64_t tba_id) const;
    gnos::bundle_manifest_t build_manifest(uint64_t tba_id) const;
    uint64_t parse_tba_deposit_memo(const std::string& memo, std::string& asset_memo) const;
};
