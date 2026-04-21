#pragma once

#include <cstdint>
#include <eosio/eosio.hpp>

namespace flon {

struct nsymbol {
    uint64_t nid = 0;

    static constexpr uint32_t U1E9 = 10'0000'0000UL;

    nsymbol() = default;

    explicit nsymbol(uint32_t i, uint32_t p = 0) {
        eosio::check(p < U1E9, "pid must be below 10**9");
        eosio::check(i < U1E9, "id must be below 10**9");
        nid = static_cast<uint64_t>(p) * U1E9 + i;
    }

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

    uint32_t id() const {
        return nid % U1E9;
    }

    uint32_t pid() const {
        return nid / U1E9;
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

    explicit nasset(const uint32_t& id) : symbol(id), amount(0) {}

    explicit nasset(uint32_t id, uint32_t pid, int64_t amount = 0) : amount(amount), symbol(id, pid) {
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
