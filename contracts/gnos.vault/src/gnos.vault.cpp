#include "gnos.vault.hpp"

#include <vector>

namespace flon {

using eosio::asset;
using eosio::check;
using eosio::is_account;
using eosio::name;
using eosio::symbol;
using std::string;

static constexpr name cisum_token_contract = "cisum.token"_n;
static constexpr symbol cisum_symbol = symbol("CISUM", 4);

std::vector<string> split(const string& s, const string& delimiter) {
  std::vector<string> result;
  size_t pos_start = 0;
  size_t pos_end;
  const auto delim_len = delimiter.length();
  while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
    result.emplace_back(s.substr(pos_start, pos_end - pos_start));
    pos_start = pos_end + delim_len;
  }
  result.emplace_back(s.substr(pos_start));
  return result;
}

void gnos_vault::require_admin() const {
  CHECKC(_gstate.admin.value != 0, err::RECORD_NO_FOUND, "admin not set");
  CHECKC(has_auth(get_self()) || has_auth(_gstate.admin), err::DID_NOT_AUTH, "admin/contract only");
}

void gnos_vault::init(const name& admin) {
  require_auth(get_self());

  CHECKC(is_account(admin), err::ACCOUNT_INVALID, "admin not exist");
  _gstate.admin = admin;
  _gstate.paused = false;
}

void gnos_vault::setadmin(const name& admin) {
  require_auth(get_self());
  CHECKC(is_account(admin), err::ACCOUNT_INVALID, "admin not exist");
  _gstate.admin = admin;
}

void gnos_vault::setpause(const bool& paused) {
  require_admin();
  _gstate.paused = paused;
}

void gnos_vault::ontransfer(const name& from, const name& to, const asset& quantity, const string& memo) {
  if (from == get_self() || to != get_self()) return;

  CHECKC(!_gstate.paused, err::UNDER_MAINTENANCE, "contract paused");
  CHECKC(get_first_receiver() == cisum_token_contract, err::INVALID_FORMAT, "invalid token contract");
  CHECKC(quantity.is_valid(), err::INVALID_FORMAT, "invalid quantity");
  CHECKC(quantity.amount > 0, err::NOT_POSITIVE, "must transfer positive");
  CHECKC(quantity.symbol == cisum_symbol, err::SYMBOL_MISMATCH, "symbol mismatch");
  CHECKC(memo.size() <= 256, err::INVALID_FORMAT, "memo too long");

  // memo format: buytoken:threadid[:...]
  auto parts = split(memo, ":");
  CHECKC(!parts.empty(), err::INVALID_FORMAT, "empty memo");

  const auto op = parts[0];
  CHECKC(op == "buytoken", err::INVALID_FORMAT, "memo op must be buytoken");
  CHECKC(parts.size() >= 2, err::INVALID_FORMAT, "memo must be buytoken:threadid[:...]");
  const auto threadid = parts[1];
  CHECKC(!threadid.empty(), err::INVALID_FORMAT, "threadid must not be empty");
}

} // namespace flon
