#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <gnos.protocol/gnos.protocol.hpp>

class [[eosio::contract("gnos.stake")]] gnosstake : public eosio::contract {
public:
    using contract::contract;

    [[eosio::action]]
    void createplan(uint64_t nft_id, eosio::name stake_token_contract, eosio::symbol stake_symbol, eosio::name reward_token_contract, eosio::symbol reward_symbol);

    [[eosio::action]]
    void stake(eosio::name staker, uint64_t plan_id, eosio::asset quantity);

    [[eosio::action]]
    void fundreward(uint64_t plan_id, eosio::asset quantity);

    [[eosio::action]]
    void unstake(eosio::name staker, uint64_t plan_id, eosio::asset quantity);

    [[eosio::action]]
    void claim(eosio::name staker, uint64_t plan_id);

    [[eosio::on_notify("*::transfer")]]
    void ontransfer(eosio::name from, eosio::name to, eosio::asset quantity, std::string memo);

private:
    static constexpr uint64_t reward_precision = 1000000000000ULL;
    gnos::stake_plan_table::const_iterator require_plan(uint64_t plan_id, gnos::stake_plan_table& plans) const;
    gnos::stake_position_table::const_iterator require_position(eosio::name staker, uint64_t plan_id, gnos::stake_position_table& positions) const;
    eosio::asset compute_claimable(const gnos::stake_plan_t& plan, const gnos::stake_position_t& position) const;
    uint64_t parse_plan_memo(const std::string& memo, const char* prefix) const;
    void apply_stake(eosio::name staker, uint64_t plan_id, const eosio::asset& quantity);
    void apply_fundreward(uint64_t plan_id, const eosio::asset& quantity);
    void settle_position(const gnos::stake_plan_t& plan, gnos::stake_position_t& position) const;
};
