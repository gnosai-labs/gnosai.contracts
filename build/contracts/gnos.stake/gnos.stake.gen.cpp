#include "/Users/aaron/Documents/flon/gnos.contracts/contracts/gnos.stake/src/gnos.stake.cpp"


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("createplan:__eosio_action_createplan_gnosstake"))) void __eosio_action_createplan_gnosstake(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
eosio::name arg1; ds >> arg1;
eosio::symbol arg2; ds >> arg2;
eosio::name arg3; ds >> arg3;
eosio::symbol arg4; ds >> arg4;
gnosstake{eosio::name{r},eosio::name{c},ds}.createplan(arg0, arg1, arg2, arg3, arg4);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("stake:__eosio_action_stake_gnosstake"))) void __eosio_action_stake_gnosstake(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
uint64_t arg1; ds >> arg1;
eosio::asset arg2; ds >> arg2;
gnosstake{eosio::name{r},eosio::name{c},ds}.stake(arg0, arg1, arg2);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("fundreward:__eosio_action_fundreward_gnosstake"))) void __eosio_action_fundreward_gnosstake(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
eosio::asset arg1; ds >> arg1;
gnosstake{eosio::name{r},eosio::name{c},ds}.fundreward(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("unstake:__eosio_action_unstake_gnosstake"))) void __eosio_action_unstake_gnosstake(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
uint64_t arg1; ds >> arg1;
eosio::asset arg2; ds >> arg2;
gnosstake{eosio::name{r},eosio::name{c},ds}.unstake(arg0, arg1, arg2);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("claim:__eosio_action_claim_gnosstake"))) void __eosio_action_claim_gnosstake(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
uint64_t arg1; ds >> arg1;
gnosstake{eosio::name{r},eosio::name{c},ds}.claim(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_notify("*::transfer:__eosio_notify_ontransfer_gnosstake"))) void __eosio_notify_ontransfer_gnosstake(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
eosio::name arg1; ds >> arg1;
eosio::asset arg2; ds >> arg2;
std::string arg3; ds >> arg3;
gnosstake{eosio::name{r},eosio::name{c},ds}.ontransfer(arg0, arg1, arg2, arg3);
}}

extern "C" {
__attribute__((eosio_wasm_import))
void eosio_assert_code(uint32_t, uint64_t);	__attribute__((weak, eosio_wasm_entry, eosio_wasm_abi("{\"____comment\":\"This file was generated with eosio-abigen. DO NOT EDIT \",\"version\":\"flon::abi/1.2\",\"structs\":[{\"name\":\"claim\",\"base\":\"\",\"fields\":[{\"name\":\"staker\",\"type\":\"name\"},{\"name\":\"plan_id\",\"type\":\"uint64\"}]},{\"name\":\"createplan\",\"base\":\"\",\"fields\":[{\"name\":\"nft_id\",\"type\":\"uint64\"},{\"name\":\"stake_token_contract\",\"type\":\"name\"},{\"name\":\"stake_symbol\",\"type\":\"symbol\"},{\"name\":\"reward_token_contract\",\"type\":\"name\"},{\"name\":\"reward_symbol\",\"type\":\"symbol\"}]},{\"name\":\"fundreward\",\"base\":\"\",\"fields\":[{\"name\":\"plan_id\",\"type\":\"uint64\"},{\"name\":\"quantity\",\"type\":\"asset\"}]},{\"name\":\"stake\",\"base\":\"\",\"fields\":[{\"name\":\"staker\",\"type\":\"name\"},{\"name\":\"plan_id\",\"type\":\"uint64\"},{\"name\":\"quantity\",\"type\":\"asset\"}]},{\"name\":\"unstake\",\"base\":\"\",\"fields\":[{\"name\":\"staker\",\"type\":\"name\"},{\"name\":\"plan_id\",\"type\":\"uint64\"},{\"name\":\"quantity\",\"type\":\"asset\"}]}],\"types\":[],\"actions\":[{\"name\":\"claim\",\"type\":\"claim\",\"ricardian_contract\":\"\"},{\"name\":\"createplan\",\"type\":\"createplan\",\"ricardian_contract\":\"\"},{\"name\":\"fundreward\",\"type\":\"fundreward\",\"ricardian_contract\":\"\"},{\"name\":\"stake\",\"type\":\"stake\",\"ricardian_contract\":\"\"},{\"name\":\"unstake\",\"type\":\"unstake\",\"ricardian_contract\":\"\"}],\"tables\":[],\"ricardian_clauses\":[],\"variants\":[],\"abi_extensions\":[],\"action_results\":[]}")))
	void __insert_eosio_abi(unsigned long long r, unsigned long long c, unsigned long long a){eosio_assert_code(false, 1);}
}