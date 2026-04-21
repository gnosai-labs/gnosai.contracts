#pragma once

#include <eosio/crypto.hpp>
#include <eosio/eosio.hpp>
#include <gnos.protocol/gnos.protocol.hpp>

class [[eosio::contract("nftv.mart")]] nftvmart : public eosio::contract {
public:
    using contract::contract;

    [[eosio::action]]
    void listnft(eosio::name seller, flon::nasset nft_quantity, eosio::name pay_token_contract, eosio::asset price);

    [[eosio::action]]
    void listbundle(eosio::name seller, uint64_t tba_id, eosio::name pay_token_contract, eosio::asset price);

    [[eosio::action]]
    void cancel(eosio::name seller, uint64_t order_id);

    [[eosio::action]]
    void buy(eosio::name buyer, uint64_t order_id);

    [[eosio::action]]
    void setpaytoken(eosio::name token_contract, bool enabled);

private:
    void check_price(const eosio::asset& price) const;
    void check_pay_token(eosio::name token_contract) const;
    gnos::bundle_manifest_t build_manifest(uint64_t tba_id) const;
    eosio::checksum256 calc_manifest_hash(uint64_t tba_id, const std::vector<gnos::bundle_item_t>& items) const;
    eosio::name get_nft_creator(eosio::name nft_contract, flon::nasset nft_quantity) const;
    void transfer_nft(eosio::name from, eosio::name to, flon::nasset nft_quantity, std::string memo) const;
    void distribute_fees(eosio::name token_contract, const gnos::order_t& order, eosio::name creator, const eosio::asset& quantity);
};
