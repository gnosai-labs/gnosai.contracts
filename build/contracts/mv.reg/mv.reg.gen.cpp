#include "/Users/aaron/Documents/flon/gnos.contracts/contracts/mv.reg/src/mv.reg.cpp"


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("createmv:__eosio_action_createmv_mvreg"))) void __eosio_action_createmv_mvreg(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
uint64_t arg1; ds >> arg1;
std::string arg2; ds >> arg2;
std::string arg3; ds >> arg3;
eosio::checksum256 arg4; ds >> arg4;
std::string arg5; ds >> arg5;
mvreg{eosio::name{r},eosio::name{c},ds}.createmv(arg0, arg1, arg2, arg3, arg4, arg5);
}}

extern "C" {
__attribute__((eosio_wasm_import))
void eosio_assert_code(uint32_t, uint64_t);	__attribute__((weak, eosio_wasm_entry, eosio_wasm_abi("{\"____comment\":\"This file was generated with eosio-abigen. DO NOT EDIT \",\"version\":\"flon::abi/1.2\",\"structs\":[{\"name\":\"createmv\",\"base\":\"\",\"fields\":[{\"name\":\"creator\",\"type\":\"name\"},{\"name\":\"song_id\",\"type\":\"uint64\"},{\"name\":\"title\",\"type\":\"string\"},{\"name\":\"mv_uri\",\"type\":\"string\"},{\"name\":\"mv_hash\",\"type\":\"checksum256\"},{\"name\":\"meta_uri\",\"type\":\"string\"}]}],\"types\":[],\"actions\":[{\"name\":\"createmv\",\"type\":\"createmv\",\"ricardian_contract\":\"\"}],\"tables\":[],\"ricardian_clauses\":[],\"variants\":[],\"abi_extensions\":[],\"action_results\":[]}")))
	void __insert_eosio_abi(unsigned long long r, unsigned long long c, unsigned long long a){eosio_assert_code(false, 1);}
}