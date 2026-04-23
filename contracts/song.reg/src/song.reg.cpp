#include <song.reg/song.reg.hpp>

#include <eosio/crypto.hpp>
#include <flon/nasset.hpp>
#include <gnos.ntoken/gnos.ntoken.hpp>

#include <limits>
#include <vector>

using namespace eosio;

static constexpr name cisum_token_contract = "cisum.token"_n;
static constexpr name ntoken_contract = "gnos.ntoken"_n;
static constexpr name active_permission = "active"_n;
static constexpr symbol cisum_symbol = symbol("CISUM", 4);

#define CREATE_N(bank, issuer, maximum_supply, symbol, token_uri, ipowner) \
    { gnos::gnosntoken::create_action act{ bank, { {issuer, active_permission} } }; \
      act.send(issuer, maximum_supply, symbol, token_uri, ipowner); }

#define ISSUE_N(bank, to, quantity, memo) \
    { gnos::gnosntoken::issue_action act{ bank, { {to, active_permission} } }; \
      act.send(to, quantity, memo); }

#define TRANSFER_N(bank, from, to, assets, memo) \
    { gnos::gnosntoken::transfer_action act{ bank, { {from, active_permission} } }; \
      act.send(from, to, assets, memo); }

std::vector<std::string> split(const std::string& s, const std::string& delimiter, size_t max_parts = 0) {
    std::vector<std::string> result;
    size_t pos_start = 0;
    size_t pos_end;
    auto delim_len = delimiter.length();
    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        if (max_parts > 0 && result.size() + 1 == max_parts) {
            break;
        }
        result.emplace_back(s.substr(pos_start, pos_end - pos_start));
        pos_start = pos_end + delim_len;
    }
    result.emplace_back(s.substr(pos_start));
    return result;
}

uint32_t parse_count(const std::string& value, const std::string& label) {
    check(!value.empty(), (label + " is required").c_str());

    uint64_t count = 0;
    for (size_t i = 0; i < value.size(); ++i) {
        const char c = value[i];
        check(c >= '0' && c <= '9', ("invalid " + label).c_str());
        count = count * 10 + static_cast<uint64_t>(c - '0');
        check(count <= static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()), (label + " is too large").c_str());
    }

    check(count > 0, (label + " must be positive").c_str());
    return static_cast<uint32_t>(count);
}

uint64_t parse_nft_id(const std::string& value) {
    check(!value.empty(), "nid is required");

    uint64_t nft_id = 0;
    for (size_t i = 0; i < value.size(); ++i) {
        const char c = value[i];
        check(c >= '0' && c <= '9', "invalid nft id");
        nft_id = nft_id * 10 + static_cast<uint64_t>(c - '0');
    }
    check(nft_id > 0, "nft id is required");
    return nft_id;
}

int64_t expected_payment_amount(const asset& nft_price, uint32_t nft_count) {
    check(nft_price.amount <= std::numeric_limits<int64_t>::max() / static_cast<int64_t>(nft_count), "payment quantity is too large");
    return nft_price.amount * static_cast<int64_t>(nft_count);
}

void check_cisum_price(const asset& nft_price) {
    check(nft_price.is_valid(), "invalid nft price");
    check(nft_price.amount > 0, "nft price must be positive");
    check(nft_price.symbol == cisum_symbol, "nft price must be CISUM");
}

uint32_t songreg::parse_songcreate_memo(const std::string& memo, uint64_t& nft_id, std::string& token_uri, std::string& music_hash, std::string& music_url) const {
    const auto parts = split(memo, ":", 6);
    check(parts.size() == 6, "memo must be SONGCREATE:<quantity>:<nid>:<token_uri_or_hash>:<music_hash>:<music_url>");
    check(parts[0] == "SONGCREATE", "memo type must be SONGCREATE");
    check(!parts[3].empty(), "token_uri is required");
    check(!parts[4].empty(), "music_hash is required");
    check(!parts[5].empty(), "music_url is required");

    const uint32_t issue_count = parse_count(parts[1], "issue_count");
    nft_id = parse_nft_id(parts[2]);
    token_uri = parts[3];
    music_hash = parts[4];
    music_url = parts[5];

    return issue_count;
}

uint32_t songreg::parse_songbuy_memo(const std::string& memo, uint64_t& nft_id) const {
    const auto parts = split(memo, ":");
    check(parts.size() == 3, "memo must be SONGBUY:<quantity>:<nid>");
    check(parts[0] == "SONGBUY", "memo type must be SONGBUY");

    const uint32_t buy_count = parse_count(parts[1], "buy_count");
    nft_id = parse_nft_id(parts[2]);

    return buy_count;
}

void songreg::create_song_nft(name creator, uint64_t nft_id, std::string token_uri, std::string music_hash, std::string music_url, uint32_t issue_count, const song_reg_global& gstate) {
    check(nft_id > 0, "nft id is required");
    check(!token_uri.empty(), "token_uri is required");
    check(!music_hash.empty(), "music_hash is required");
    check(!music_url.empty(), "music_url is required");
    check(issue_count > 0, "issue_count must be positive");

    gnos::nft_stat_t::idx_t nft_stats(ntoken_contract, ntoken_contract.value);
    check(nft_stats.find(nft_id) == nft_stats.end(), "nft id already exists");
    song_table songs(get_self(), get_self().value);
    check(songs.find(nft_id) == songs.end(), "song already registered");

    const flon::nsymbol nft_symbol(nft_id);

    check(gstate.nfts_per_issue > 0, "nfts_per_issue is required");
    check_cisum_price(gstate.nft_price);

    const uint32_t max_nft_supply = gstate.nfts_per_issue;
    check(issue_count <= max_nft_supply, "issue_count exceeds max nft supply");
    CREATE_N(
        ntoken_contract,
        get_self(),
        max_nft_supply,
        nft_symbol,
        token_uri,
        creator
    );

    ISSUE_N(
        ntoken_contract,
        get_self(),
        flon::nasset(max_nft_supply, nft_symbol),
        std::string("song init issue")
    );

    TRANSFER_N(
        ntoken_contract,
        get_self(),
        creator,
        std::vector<flon::nasset>{flon::nasset(issue_count, nft_symbol)},
        std::string("SONGNFTISSUE:") + std::to_string(issue_count) + ":" + std::to_string(nft_symbol.nid)
    );

    songs.emplace(get_self(), [&](auto& row) {
        const time_point_sec now = time_point_sec(current_time_point());
        row.nid = nft_id;
        row.creator = creator;
        row.music_hash = music_hash;
        row.music_url = music_url;
        row.create_at = now;
        row.update_at = now;
    });
}

void songreg::buy_song_nft(name buyer, uint64_t nft_id, uint32_t buy_count) {
    check(nft_id > 0, "nft id is required");
    check(buy_count > 0, "buy_count must be positive");

    gnos::nft_stat_t::idx_t nft_stats(ntoken_contract, ntoken_contract.value);
    check(nft_stats.find(nft_id) != nft_stats.end(), "nft id does not exist");

    const flon::nsymbol nft_symbol(nft_id);
    gnos::nft_account_t::idx_t accounts(ntoken_contract, get_self().value);
    auto account_itr = accounts.find(nft_id);
    check(account_itr != accounts.end(), "song.reg has no nft inventory");
    check(!account_itr->paused, "song.reg nft inventory is paused");
    check(account_itr->balance.amount >= static_cast<int64_t>(buy_count), "insufficient song.reg nft inventory");

    TRANSFER_N(
        ntoken_contract,
        get_self(),
        buyer,
        std::vector<flon::nasset>{flon::nasset(buy_count, nft_symbol)},
        std::string("SONGNFTBUY:") + std::to_string(buy_count) + ":" + std::to_string(nft_symbol.nid)
    );
}

// SONGCREATE:<issue_count>:<nid>:<token_uri_or_hash>:<music_hash>:<music_url>
// SONGBUY:<buy_count>:<nid>
void songreg::ontransfer(name from, name to, asset quantity, std::string memo) {
    if (to != get_self() || from == get_self()) {
        return;
    }

    check(get_first_receiver() == cisum_token_contract, "song.reg requires CISUM token transfer");
    check(quantity.amount > 0, "quantity must be positive");
    check(quantity.symbol == cisum_symbol, "song.reg requires CISUM");

    song_reg_global_singleton global(get_self(), get_self().value);
    check(global.exists(), "song.reg global config is required");
    const song_reg_global gstate = global.get();
    check(gstate.nfts_per_issue > 0, "nfts_per_issue is required");
    check_cisum_price(gstate.nft_price);

    const auto parts = split(memo, ":");
    check(!parts.empty(), "memo type is required");

    uint64_t nft_id = 0;
    if (parts[0] == "SONGCREATE") {
        std::string token_uri;
        std::string music_hash;
        std::string music_url;
        const uint32_t issue_count = parse_songcreate_memo(memo, nft_id, token_uri, music_hash, music_url);
        check(quantity.amount == expected_payment_amount(gstate.nft_price, issue_count), "payment quantity does not match SONGCREATE quantity");
        create_song_nft(from, nft_id, token_uri, music_hash, music_url, issue_count, gstate);
        return;
    }

    if (parts[0] == "SONGBUY") {
        const uint32_t buy_count = parse_songbuy_memo(memo, nft_id);
        check(quantity.amount == expected_payment_amount(gstate.nft_price, buy_count), "payment quantity does not match SONGBUY quantity");
        buy_song_nft(from, nft_id, buy_count);
        return;
    }

    check(false, "memo type must be SONGCREATE or SONGBUY");
}

void songreg::setglobal(uint32_t nfts_per_issue, asset nft_price) {
    require_auth(get_self());
    check(nfts_per_issue > 0, "nfts_per_issue is required");
    check_cisum_price(nft_price);

    song_reg_global_singleton global(get_self(), get_self().value);
    song_reg_global gstate{};
    gstate.nfts_per_issue = nfts_per_issue;
    gstate.nft_price = nft_price;
    global.set(gstate, get_self());
}
