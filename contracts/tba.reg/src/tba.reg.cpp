#include <tba.reg/tba.reg.hpp>

using namespace eosio;

namespace {

static constexpr name ntoken_contract = "gnos.ntoken"_n;

struct [[eosio::table("accounts")]] ntoken_account_probe {
    flon::nasset balance;
    bool paused = false;

    uint64_t primary_key() const {
        return balance.symbol.nid;
    }

    EOSLIB_SERIALIZE(ntoken_account_probe, (balance)(paused))
};

using ntoken_account_probe_table = multi_index<"accounts"_n, ntoken_account_probe>;

void check_owner_balance(name nft_contract, name owner, const flon::nasset& root_nft) {
    check(nft_contract == ntoken_contract, "only gnos.ntoken root nft is supported");
    check(root_nft.amount > 0, "root nft amount must be positive");
    check(root_nft.symbol.nid != 0, "root nft symbol is required");

    ntoken_account_probe_table accounts(nft_contract, owner.value);
    auto itr = accounts.find(root_nft.symbol.nid);
    check(itr != accounts.end(), "owner has no root nft balance");
    check(!itr->paused, "root nft balance is paused");
    check(itr->balance.amount >= root_nft.amount, "insufficient root nft balance");
}

}

void tbareg::createtba(name owner, name nft_contract, flon::nasset root_nft) {
    require_auth(owner);
    check(is_account(owner), "owner account does not exist");
    check_owner_balance(nft_contract, owner, root_nft);

    gnos::tba_map_table tbas(get_self(), get_self().value);
    auto nft_idx = tbas.get_index<"bynft"_n>();
    check(nft_idx.find(root_nft.symbol.nid) == nft_idx.end(), "tba already exists for root nft");
    const uint64_t tba_id = gnos::next_id(tbas.available_primary_key());
    tbas.emplace(owner, [&](auto& row) {
        row.tba_id = tba_id;
        row.nft_id = root_nft.symbol.nid;
        row.owner = owner;
        row.nft_contract = nft_contract;
        row.root_nft = root_nft;
        row.account_contract = "tba.account"_n;
        row.status = gnos::active;
    });
}

void tbareg::setstatus(uint64_t tba_id, uint8_t status) {
    require_auth(get_self());
    check(status == gnos::active || status == gnos::frozen || status == gnos::closed, "invalid tba status");

    gnos::tba_map_table tbas(get_self(), get_self().value);
    auto itr = tbas.find(tba_id);
    check(itr != tbas.end(), "tba does not exist");
    tbas.modify(itr, same_payer, [&](auto& row) {
        row.status = status;
    });
}

void tbareg::setowner(uint64_t tba_id, name owner) {
    require_auth("nftv.mart"_n);
    check(is_account(owner), "owner account does not exist");

    gnos::tba_map_table tbas(get_self(), get_self().value);
    auto itr = tbas.find(tba_id);
    check(itr != tbas.end(), "tba does not exist");

    tbas.modify(itr, same_payer, [&](auto& row) {
        row.owner = owner;
    });
}
