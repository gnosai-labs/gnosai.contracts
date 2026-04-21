#include "/Users/aaron/Documents/flon/gnos.contracts/contracts/tba.account/src/tba.account.cpp"


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("deposit:__eosio_action_deposit_tbaaccount"))) void __eosio_action_deposit_tbaaccount(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
uint8_t arg1; ds >> arg1;
eosio::name arg2; ds >> arg2;
unsigned __int128 arg3; ds >> arg3;
eosio::asset arg4; ds >> arg4;
std::string arg5; ds >> arg5;
tbaaccount{eosio::name{r},eosio::name{c},ds}.deposit(arg0, arg1, arg2, arg3, arg4, arg5);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("deposittoken:__eosio_action_deposittoken_tbaaccount"))) void __eosio_action_deposittoken_tbaaccount(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
uint64_t arg1; ds >> arg1;
eosio::name arg2; ds >> arg2;
eosio::asset arg3; ds >> arg3;
std::string arg4; ds >> arg4;
tbaaccount{eosio::name{r},eosio::name{c},ds}.deposittoken(arg0, arg1, arg2, arg3, arg4);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("withdraw:__eosio_action_withdraw_tbaaccount"))) void __eosio_action_withdraw_tbaaccount(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
uint64_t arg1; ds >> arg1;
tbaaccount{eosio::name{r},eosio::name{c},ds}.withdraw(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("setmode:__eosio_action_setmode_tbaaccount"))) void __eosio_action_setmode_tbaaccount(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
uint8_t arg1; ds >> arg1;
tbaaccount{eosio::name{r},eosio::name{c},ds}.setmode(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("freezeassets:__eosio_action_freezeassets_tbaaccount"))) void __eosio_action_freezeassets_tbaaccount(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
std::vector<uint64_t> arg1; ds >> arg1;
bool arg2; ds >> arg2;
tbaaccount{eosio::name{r},eosio::name{c},ds}.freezeassets(arg0, arg1, arg2);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("genmanifest:__eosio_action_genmanifest_tbaaccount"))) void __eosio_action_genmanifest_tbaaccount(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
tbaaccount{eosio::name{r},eosio::name{c},ds}.genmanifest(arg0);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("receivenft:__eosio_action_receivenft_tbaaccount"))) void __eosio_action_receivenft_tbaaccount(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
eosio::name arg1; ds >> arg1;
flon::nasset arg2; ds >> arg2;
std::string arg3; ds >> arg3;
tbaaccount{eosio::name{r},eosio::name{c},ds}.receivenft(arg0, arg1, arg2, arg3);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_notify("gnos.ntoken::transfer:__eosio_notify_onnfttransfer_tbaaccount"))) void __eosio_notify_onnfttransfer_tbaaccount(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
eosio::name arg1; ds >> arg1;
std::vector<flon::nasset> arg2; ds >> arg2;
std::string arg3; ds >> arg3;
tbaaccount{eosio::name{r},eosio::name{c},ds}.onnfttransfer(arg0, arg1, arg2, arg3);
}}

extern "C" {
__attribute__((eosio_wasm_import))
void eosio_assert_code(uint32_t, uint64_t);	__attribute__((weak, eosio_wasm_entry, eosio_wasm_abi("{\"____comment\":\"This file was generated with eosio-abigen. DO NOT EDIT \",\"version\":\"flon::abi/1.2\",\"structs\":[{\"name\":\"deposit\",\"base\":\"\",\"fields\":[{\"name\":\"tba_id\",\"type\":\"uint64\"},{\"name\":\"asset_type\",\"type\":\"uint8\"},{\"name\":\"asset_contract\",\"type\":\"name\"},{\"name\":\"asset_ref_id\",\"type\":\"uint128\"},{\"name\":\"quantity\",\"type\":\"asset\"},{\"name\":\"memo\",\"type\":\"string\"}]},{\"name\":\"deposittoken\",\"base\":\"\",\"fields\":[{\"name\":\"owner\",\"type\":\"name\"},{\"name\":\"tba_id\",\"type\":\"uint64\"},{\"name\":\"token_contract\",\"type\":\"name\"},{\"name\":\"quantity\",\"type\":\"asset\"},{\"name\":\"memo\",\"type\":\"string\"}]},{\"name\":\"freezeassets\",\"base\":\"\",\"fields\":[{\"name\":\"tba_id\",\"type\":\"uint64\"},{\"name\":\"asset_ids\",\"type\":\"uint64[]\"},{\"name\":\"frozen_state\",\"type\":\"bool\"}]},{\"name\":\"genmanifest\",\"base\":\"\",\"fields\":[{\"name\":\"tba_id\",\"type\":\"uint64\"}]},{\"name\":\"nasset\",\"base\":\"\",\"fields\":[{\"name\":\"amount\",\"type\":\"int64\"},{\"name\":\"symbol\",\"type\":\"nsymbol\"}]},{\"name\":\"nsymbol\",\"base\":\"\",\"fields\":[{\"name\":\"nid\",\"type\":\"uint64\"}]},{\"name\":\"receivenft\",\"base\":\"\",\"fields\":[{\"name\":\"tba_id\",\"type\":\"uint64\"},{\"name\":\"nft_contract\",\"type\":\"name\"},{\"name\":\"nft_quantity\",\"type\":\"nasset\"},{\"name\":\"memo\",\"type\":\"string\"}]},{\"name\":\"setmode\",\"base\":\"\",\"fields\":[{\"name\":\"asset_id\",\"type\":\"uint64\"},{\"name\":\"settlement_mode\",\"type\":\"uint8\"}]},{\"name\":\"withdraw\",\"base\":\"\",\"fields\":[{\"name\":\"tba_id\",\"type\":\"uint64\"},{\"name\":\"asset_id\",\"type\":\"uint64\"}]}],\"types\":[],\"actions\":[{\"name\":\"deposit\",\"type\":\"deposit\",\"ricardian_contract\":\"\"},{\"name\":\"deposittoken\",\"type\":\"deposittoken\",\"ricardian_contract\":\"\"},{\"name\":\"freezeassets\",\"type\":\"freezeassets\",\"ricardian_contract\":\"\"},{\"name\":\"genmanifest\",\"type\":\"genmanifest\",\"ricardian_contract\":\"\"},{\"name\":\"receivenft\",\"type\":\"receivenft\",\"ricardian_contract\":\"\"},{\"name\":\"setmode\",\"type\":\"setmode\",\"ricardian_contract\":\"\"},{\"name\":\"withdraw\",\"type\":\"withdraw\",\"ricardian_contract\":\"\"}],\"tables\":[],\"ricardian_clauses\":[],\"variants\":[],\"abi_extensions\":[],\"action_results\":[]}")))
	void __insert_eosio_abi(unsigned long long r, unsigned long long c, unsigned long long a){eosio_assert_code(false, 1);}
}