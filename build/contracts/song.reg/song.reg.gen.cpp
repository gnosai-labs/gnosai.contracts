#include "/Users/aaron/Documents/flon/gnos.contracts/contracts/song.reg/src/song.reg.cpp"


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("setglobal:__eosio_action_setglobal_songreg"))) void __eosio_action_setglobal_songreg(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint32_t arg0; ds >> arg0;
eosio::asset arg1; ds >> arg1;
songreg{eosio::name{r},eosio::name{c},ds}.setglobal(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_notify("cisum.token::transfer:__eosio_notify_ontransfer_songreg"))) void __eosio_notify_ontransfer_songreg(unsigned long long r, unsigned long long c) {
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
songreg{eosio::name{r},eosio::name{c},ds}.ontransfer(arg0, arg1, arg2, arg3);
}}

extern "C" {
__attribute__((eosio_wasm_import))
void eosio_assert_code(uint32_t, uint64_t);	__attribute__((weak, eosio_wasm_entry, eosio_wasm_abi("{\"____comment\":\"This file was generated with eosio-abigen. DO NOT EDIT \",\"version\":\"flon::abi/1.2\",\"structs\":[{\"name\":\"inventory_price\",\"base\":\"\",\"fields\":[{\"name\":\"nft_id\",\"type\":\"uint64\"},{\"name\":\"nft_price\",\"type\":\"asset\"}]},{\"name\":\"setglobal\",\"base\":\"\",\"fields\":[{\"name\":\"nfts_per_issue\",\"type\":\"uint32\"},{\"name\":\"nft_price\",\"type\":\"asset\"}]},{\"name\":\"song_reg_global\",\"base\":\"\",\"fields\":[{\"name\":\"nfts_per_issue\",\"type\":\"uint32\"},{\"name\":\"nft_price\",\"type\":\"asset\"}]}],\"types\":[],\"actions\":[{\"name\":\"setglobal\",\"type\":\"setglobal\",\"ricardian_contract\":\"\"}],\"tables\":[{\"name\":\"global\",\"type\":\"song_reg_global\",\"index_type\":\"i64\",\"key_names\":[],\"key_types\":[]},{\"name\":\"invprices\",\"type\":\"inventory_price\",\"index_type\":\"i64\",\"key_names\":[],\"key_types\":[]}],\"ricardian_clauses\":[],\"variants\":[],\"abi_extensions\":[],\"action_results\":[]}")))
	void __insert_eosio_abi(unsigned long long r, unsigned long long c, unsigned long long a){eosio_assert_code(false, 1);}
}