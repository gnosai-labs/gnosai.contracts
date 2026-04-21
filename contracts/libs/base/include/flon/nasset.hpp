#pragma once

#include <cstdint>
#include <eosio/eosio.hpp>

namespace flon {

struct nsymbol {
    uint64_t nid = 0;

    nsymbol() = default;

    explicit nsymbol(uint64_t n) : nid(n) {}

    friend bool operator==(const nsymbol& a, const nsymbol& b) {
        return a.nid == b.nid;
    }

    friend bool operator!=(const nsymbol& a, const nsymbol& b) {
        return !(a == b);
    }

    uint64_t raw() const {
        return nid;
    }

    constexpr bool is_valid() const {
        return nid != 0;
    }

    EOSLIB_SERIALIZE(nsymbol, (nid))
};

struct nasset {
    int64_t amount = 0;
    nsymbol symbol;

    static constexpr int64_t max_amount = (1LL << 62) - 1;

    nasset() = default;

    explicit nasset(const uint64_t& nid) : symbol(nid), amount(0) {
        eosio::check(is_amount_within_range(), "magnitude of asset amount must be less than 2^62");
        eosio::check(symbol.is_valid(), "invalid symbol name");
    }

    explicit nasset(const int64_t& amount, const nsymbol& symb) : amount(amount), symbol(symb) {
        eosio::check(is_amount_within_range(), "magnitude of asset amount must be less than 2^62");
        eosio::check(symbol.is_valid(), "invalid symbol name");
    }

    bool is_amount_within_range() const {
        return -max_amount <= amount && amount <= max_amount;
    }

    bool is_valid() const {
        return is_amount_within_range() && symbol.is_valid();
    }

    nasset& operator-=(const nasset& a) {
        eosio::check(a.symbol == symbol, "attempt to subtract asset with different symbol");
        amount -= a.amount;
        eosio::check(-max_amount <= amount, "subtraction underflow");
        eosio::check(amount <= max_amount, "subtraction overflow");
        return *this;
    }

    nasset& operator+=(const nasset& a) {
        eosio::check(a.symbol == symbol, "attempt to add asset with different symbol");
        amount += a.amount;
        eosio::check(-max_amount <= amount, "addition underflow");
        eosio::check(amount <= max_amount, "addition overflow");
        return *this;
    }

    friend nasset operator+(const nasset& a, const nasset& b) {
        nasset result = a;
        result += b;
        return result;
    }

    friend nasset operator-(const nasset& a, const nasset& b) {
        nasset result = a;
        result -= b;
        return result;
    }

    friend bool operator==(const nasset& a, const nasset& b) {
        return a.amount == b.amount && a.symbol == b.symbol;
    }

    friend bool operator!=(const nasset& a, const nasset& b) {
        return !(a == b);
    }

    EOSLIB_SERIALIZE(nasset, (amount)(symbol))
};

} // namespace flon
