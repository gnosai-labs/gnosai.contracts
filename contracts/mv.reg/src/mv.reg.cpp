#include <mv.reg/mv.reg.hpp>

#include <flon/nasset.hpp>

using namespace eosio;

namespace {

static constexpr name ntoken_contract = "gnos.ntoken"_n;

struct [[eosio::table("tokenstats")]] ntoken_stat_probe {
    flon::nasset supply;
    flon::nasset max_supply;
    std::string token_uri;
    name ipowner;
    name notary;
    name issuer;
    time_point_sec issued_at;
    time_point_sec notarized_at;
    bool paused = false;

    uint64_t primary_key() const {
        return supply.symbol.nid;
    }

    EOSLIB_SERIALIZE(ntoken_stat_probe, (supply)(max_supply)(token_uri)(ipowner)(notary)(issuer)(issued_at)(notarized_at)(paused))
};

using ntoken_stat_probe_table = multi_index<"tokenstats"_n, ntoken_stat_probe>;

}

void mvreg::createmv(name creator, uint64_t song_id, std::string title, std::string mv_uri, checksum256 mv_hash, std::string meta_uri) {
    require_auth(creator);
    check(!title.empty(), "title is required");
    check(!mv_uri.empty(), "mv_uri is required");

    ntoken_stat_probe_table nft_stats(ntoken_contract, ntoken_contract.value);
    auto song_itr = nft_stats.find(song_id);
    check(song_itr != nft_stats.end(), "song nft does not exist");

    gnos::mv_table mvs(get_self(), get_self().value);
    auto hash_idx = mvs.get_index<"byhash"_n>();
    check(hash_idx.find(mv_hash) == hash_idx.end(), "mv_hash already exists");

    const uint64_t mv_id = gnos::next_id(mvs.available_primary_key());
    mvs.emplace(creator, [&](auto& row) {
        row.mv_id = mv_id;
        row.song_id = song_id;
        row.creator = creator;
        row.title = title;
        row.mv_uri = mv_uri;
        row.mv_hash = mv_hash;
        row.meta_uri = meta_uri;
        row.created_at = time_point_sec(current_time_point());
    });
}
