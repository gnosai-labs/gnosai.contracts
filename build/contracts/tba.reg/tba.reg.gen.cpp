#include "/Users/aaron/Documents/flon/gnos.contracts/contracts/tba.reg/src/tba.reg.cpp"


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("createtba:__eosio_action_createtba_tbareg"))) void __eosio_action_createtba_tbareg(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
eosio::name arg1; ds >> arg1;
flon::nasset arg2; ds >> arg2;
tbareg{eosio::name{r},eosio::name{c},ds}.createtba(arg0, arg1, arg2);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("setstatus:__eosio_action_setstatus_tbareg"))) void __eosio_action_setstatus_tbareg(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
uint8_t arg1; ds >> arg1;
tbareg{eosio::name{r},eosio::name{c},ds}.setstatus(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("setowner:__eosio_action_setowner_tbareg"))) void __eosio_action_setowner_tbareg(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
eosio::name arg1; ds >> arg1;
tbareg{eosio::name{r},eosio::name{c},ds}.setowner(arg0, arg1);
}}

extern "C" {
__attribute__((eosio_wasm_import))
void eosio_assert_code(uint32_t, uint64_t);	__attribute__((weak, eosio_wasm_entry, eosio_wasm_abi("{\"____comment\":\"This file was generated with eosio-abigen. DO NOT EDIT \",\"version\":\"flon::abi/1.2\",\"structs\":[{\"name\":\"createtba\",\"base\":\"\",\"fields\":[{\"name\":\"owner\",\"type\":\"name\"},{\"name\":\"nft_contract\",\"type\":\"name\"},{\"name\":\"root_nft\",\"type\":\"nasset\"}]},{\"name\":\"nasset\",\"base\":\"\",\"fields\":[{\"name\":\"amount\",\"type\":\"int64\"},{\"name\":\"symbol\",\"type\":\"nsymbol\"}]},{\"name\":\"nsymbol\",\"base\":\"\",\"fields\":[{\"name\":\"nid\",\"type\":\"uint64\"}]},{\"name\":\"setowner\",\"base\":\"\",\"fields\":[{\"name\":\"tba_id\",\"type\":\"uint64\"},{\"name\":\"owner\",\"type\":\"name\"}]},{\"name\":\"setstatus\",\"base\":\"\",\"fields\":[{\"name\":\"tba_id\",\"type\":\"uint64\"},{\"name\":\"status\",\"type\":\"uint8\"}]}],\"types\":[],\"actions\":[{\"name\":\"createtba\",\"type\":\"createtba\",\"ricardian_contract\":\"\"},{\"name\":\"setowner\",\"type\":\"setowner\",\"ricardian_contract\":\"\"},{\"name\":\"setstatus\",\"type\":\"setstatus\",\"ricardian_contract\":\"\"}],\"tables\":[],\"ricardian_clauses\":[],\"variants\":[],\"abi_extensions\":[],\"action_results\":[]}")))
	void __insert_eosio_abi(unsigned long long r, unsigned long long c, unsigned long long a){eosio_assert_code(false, 1);}
}