#pragma once

#include <eosio/eosio.hpp>
#include <eosio/permission.hpp>

#include <string>
#include <vector>

#include <gnos.ntoken/gnos.ntoken.db.hpp>

namespace gnos {

using namespace eosio;
using std::string;
using std::vector;

static constexpr uint8_t MAX_BALANCE_COUNT = 30;
static constexpr name DID_CONTRACT = "did.ntoken"_n;
static constexpr uint32_t DID_SYMBOL_ID = 1000001;

class [[eosio::contract("gnos.ntoken")]] gnosntoken : public contract {
public:
    using contract::contract;

    gnosntoken(name receiver, name code, datastream<const char*> ds)
        : contract(receiver, code, ds),
          _global(get_self(), get_self().value) {
        _gstate = _global.exists() ? _global.get() : nft_global_t{};
    }

    ~gnosntoken() {
        _global.set(_gstate, get_self());
    }

    ACTION create(const name& issuer, const int64_t& maximum_supply, const nsymbol& symbol, const string& token_uri, const name& ipowner);
    ACTION issue(const name& to, const nasset& quantity, const string& memo);
    ACTION retire(const nasset& quantity, const string& memo);
    ACTION transfer(const name& from, const name& to, const vector<nasset>& assets, const string& memo);

    ACTION setnotary(const name& notary, const bool& to_add);
    ACTION setipowner(const uint64_t& symbid, const name& ip_owner);
    ACTION addwhitelist(const name& acct);
    ACTION delwhitelist(const name& acct);
    ACTION settokenuri(const uint64_t& symbid, const string& url);
    ACTION notarize(const name& notary, const uint32_t& token_id);
    ACTION setcreator(const name& creator, const bool& to_add);

    using create_action = action_wrapper<"create"_n, &gnosntoken::create>;
    using issue_action = action_wrapper<"issue"_n, &gnosntoken::issue>;
    using retire_action = action_wrapper<"retire"_n, &gnosntoken::retire>;
    using transfer_action = action_wrapper<"transfer"_n, &gnosntoken::transfer>;

    static nasset get_balance(const name& contract, const name& owner, const nsymbol& sym) {
        auto acnts = nft_account_t::idx_t(contract, owner.value);
        const auto& acnt = acnts.get(sym.nid, "no balance object found");
        return acnt.paused ? nasset() : acnt.balance;
    }

private:
    void add_balance(const name& owner, const nasset& value, const name& ram_payer);
    void sub_balance(const name& owner, const nasset& value);
    void creator_auth_check(const name& creator);

private:
    nft_global_singleton _global;
    nft_global_t _gstate;
};

} // namespace gnos
