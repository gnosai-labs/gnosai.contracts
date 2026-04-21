#include <gnos.dao/gnos.dao.hpp>

using namespace eosio;

void gnosdao::setfee(uint16_t platform_bp, uint16_t creator_bp, uint16_t stake_bp, name platform_account, name stake_account) {
    require_auth(get_self());
    check(platform_bp + creator_bp + stake_bp <= 10000, "fee basis points exceed 10000");

    gnos::fee_conf conf;
    conf.platform_bp = platform_bp;
    conf.creator_bp = creator_bp;
    conf.stake_bp = stake_bp;
    conf.platform_account = platform_account;
    conf.stake_account = stake_account;

    gnos::fee_config_singleton config(get_self(), get_self().value);
    config.set(conf, get_self());
}
