#include <gnos.ntoken/gnos.ntoken.hpp>

namespace gnos {

void gnosntoken::create(const name& issuer, const int64_t& maximum_supply, const nsymbol& symbol, const string& token_uri, const name& ipowner) {
    require_auth(issuer);

    check(is_account(issuer), "issuer account does not exist");
    check(is_account(ipowner) || ipowner.length() == 0, "ipowner account does not exist");
    check(maximum_supply > 0, "max-supply must be positive");
    check(token_uri.length() < 1024, "token uri length > 1024");

    creator_auth_check(issuer);

    auto nsymb = symbol;
    auto nstats = nft_stat_t::idx_t(get_self(), get_self().value);
    auto token_uri_idx = nstats.get_index<"tokenuriidx"_n>();
    auto token_uri_hash = GNOS_HASH256(token_uri);

    check(token_uri_idx.find(token_uri_hash) == token_uri_idx.end(), "token with token_uri already exists");
    check(nstats.find(nsymb.nid) == nstats.end(), "token of nsymbol: " + to_string(nsymb.nid) + " alreay exists");
    check(nsymb.nid != 0, "symbol id is required");

    nstats.emplace(issuer, [&](auto& row) {
        row.supply.symbol = nsymb;
        row.max_supply = nasset(maximum_supply, symbol);
        row.token_uri = token_uri;
        row.ipowner = ipowner;
        row.issuer = issuer;
        row.issued_at = current_time_point();
    });
}

void gnosntoken::issue(const name& to, const nasset& quantity, const string& memo) {
    auto sym = quantity.symbol;
    check(memo.size() <= 256, "memo has more than 256 bytes");

    auto nstats = nft_stat_t::idx_t(get_self(), get_self().value);
    auto existing = nstats.find(sym.nid);
    check(existing != nstats.end(), "token with symbol does not exist, create token before issue");
    const auto& st = *existing;

    check(to == st.issuer, "tokens can only be issued to issuer account");
    require_auth(st.issuer);
    check(quantity.amount > 0, "must issue positive quantity");
    check(quantity.symbol == st.supply.symbol, "symbol mismatch");
    check(quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    nstats.modify(st, get_self(), [&](auto& row) {
        row.supply += quantity;
        row.issued_at = current_time_point();
    });

    add_balance(st.issuer, quantity, st.issuer);
}

void gnosntoken::retire(const nasset& quantity, const string& memo) {
    auto sym = quantity.symbol;
    check(memo.size() <= 256, "memo has more than 256 bytes");

    auto nstats = nft_stat_t::idx_t(get_self(), get_self().value);
    auto existing = nstats.find(sym.nid);
    check(existing != nstats.end(), "token with symbol does not exist");
    const auto& st = *existing;

    require_auth(st.issuer);
    check(quantity.amount > 0, "must retire positive quantity");
    check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");

    nstats.modify(st, get_self(), [&](auto& row) {
        row.supply -= quantity;
    });

    sub_balance(st.issuer, quantity);
}

void gnosntoken::transfer(const name& from, const name& to, const vector<nasset>& assets, const string& memo) {
    check(from != to, "cannot transfer to self");
    require_auth(from);
    check(is_account(to), "to account does not exist");
    check(memo.size() <= 256, "memo has more than 256 bytes");
    if (!_gstate.whitelist.empty()) {
        const bool from_whitelisted = _gstate.whitelist.find(from) != _gstate.whitelist.end();
        const bool to_whitelisted = _gstate.whitelist.find(to) != _gstate.whitelist.end();
        check(from_whitelisted || to_whitelisted, "transfer requires a whitelisted sender or receiver");
    }

    auto payer = has_auth(to) ? to : from;

    require_recipient(from);
    require_recipient(to);

    for (const auto& quantity : assets) {
        auto sym = quantity.symbol;
        auto nstats = nft_stat_t::idx_t(get_self(), get_self().value);
        const auto& st = nstats.get(sym.nid);

        check(quantity.amount > 0, "must transfer positive quantity");
        check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");

        sub_balance(from, quantity);
        add_balance(to, quantity, payer);
    }
}

void gnosntoken::setipowner(const uint64_t& symbid, const name& ip_owner) {
    check(has_auth(get_self()), "no auth");

    auto nstats = nft_stat_t::idx_t(get_self(), get_self().value);
    auto itr = nstats.find(symbid);
    check(itr != nstats.end(), "nft not found");

    nstats.modify(itr, same_payer, [&](auto& row) {
        row.ipowner = ip_owner;
    });
}

void gnosntoken::settokenuri(const uint64_t& symbid, const string& url) {
    check(has_auth(get_self()), "non authorized");

    auto nstats = nft_stat_t::idx_t(get_self(), get_self().value);
    auto itr = nstats.find(symbid);
    check(itr != nstats.end(), "nft not found  " + to_string(symbid));

    nstats.modify(itr, get_self(), [&](auto& row) {
        row.token_uri = url;
    });
}

void gnosntoken::setnotary(const name& notary, const bool& to_add) {
    require_auth(get_self());

    if (to_add) {
        _gstate.notaries.insert(notary);
    } else {
        _gstate.notaries.erase(notary);
    }
}

void gnosntoken::notarize(const name& notary, const uint32_t& token_id) {
    require_auth(notary);
    check(_gstate.notaries.find(notary) != _gstate.notaries.end(), "not authorized notary");

    auto nstats = nft_stat_t::idx_t(get_self(), get_self().value);
    auto itr = nstats.find(token_id);
    check(itr != nstats.end(), "token not found: " + to_string(token_id));

    nstats.modify(itr, same_payer, [&](auto& row) {
        row.notary = notary;
        row.notarized_at = time_point_sec(current_time_point());
    });
}

void gnosntoken::addwhitelist(const name& acct) {
    require_auth(get_self());
    check(is_account(acct), "account not exist");

    auto [it, inserted] = _gstate.whitelist.insert(acct);
    check(inserted, "already in whitelist");
    _global.set(_gstate, get_self());
}

void gnosntoken::delwhitelist(const name& acct) {
    require_auth(get_self());

    auto it = _gstate.whitelist.find(acct);
    check(it != _gstate.whitelist.end(), "not in whitelist");

    _gstate.whitelist.erase(it);
    _global.set(_gstate, get_self());
}

void gnosntoken::setcreator(const name& creator, const bool& to_add) {
    require_auth(get_self());
    check(is_account(creator), "creator does not exist");

    if (to_add) {
        _gstate.creators.insert(creator);
    } else {
        check(_gstate.creators.find(creator) != _gstate.creators.end(), "creator not found:" + creator.to_string());
        _gstate.creators.erase(creator);
    }
    _global.set(_gstate, get_self());
}

void gnosntoken::sub_balance(const name& owner, const nasset& value) {
    auto from_acnts = nft_account_t::idx_t(get_self(), owner.value);
    const auto& from = from_acnts.get(value.symbol.nid, "no balance object found");

    check(!from.paused, "balance is paused");
    check(from.balance.amount >= value.amount, "overdrawn balance");

    from_acnts.modify(from, owner, [&](auto& row) {
        row.balance -= value;
    });
}

void gnosntoken::add_balance(const name& owner, const nasset& value, const name& ram_payer) {
    auto to_acnts = nft_account_t::idx_t(get_self(), owner.value);
    auto to = to_acnts.find(value.symbol.nid);

    if (to == to_acnts.end()) {
        to_acnts.emplace(ram_payer, [&](auto& row) {
            row.balance = value;
        });
    } else {
        to_acnts.modify(to, same_payer, [&](auto& row) {
            row.balance += value;
        });
    }
}

void gnosntoken::creator_auth_check(const name& creator) {
    if (_gstate.creators.empty()) {
        return;
    }

    auto found = _gstate.creators.find(creator) != _gstate.creators.end();
    check(found, "creator not authorized: " + creator.to_string());
}

} // namespace gnos
