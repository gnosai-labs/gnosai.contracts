#include "/Users/aaron/Documents/flon/gnos.contracts/contracts/gnos.dao/src/gnos.dao.cpp"


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("setfee:__eosio_action_setfee_gnosdao"))) void __eosio_action_setfee_gnosdao(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint16_t arg0; ds >> arg0;
uint16_t arg1; ds >> arg1;
uint16_t arg2; ds >> arg2;
eosio::name arg3; ds >> arg3;
eosio::name arg4; ds >> arg4;
gnosdao{eosio::name{r},eosio::name{c},ds}.setfee(arg0, arg1, arg2, arg3, arg4);
}}

extern "C" {
__attribute__((eosio_wasm_import))
void eosio_assert_code(uint32_t, uint64_t);	__attribute__((weak, eosio_wasm_entry, eosio_wasm_abi("{\"____comment\":\"This file was generated with eosio-abigen. DO NOT EDIT \",\"version\":\"flon::abi/1.2\",\"structs\":[{\"name\":\"setfee\",\"base\":\"\",\"fields\":[{\"name\":\"platform_bp\",\"type\":\"uint16\"},{\"name\":\"creator_bp\",\"type\":\"uint16\"},{\"name\":\"stake_bp\",\"type\":\"uint16\"},{\"name\":\"platform_account\",\"type\":\"name\"},{\"name\":\"stake_account\",\"type\":\"name\"}]}],\"types\":[],\"actions\":[{\"name\":\"setfee\",\"type\":\"setfee\",\"ricardian_contract\":\"\"}],\"tables\":[],\"ricardian_clauses\":[],\"variants\":[],\"abi_extensions\":[],\"action_results\":[]}")))
	void __insert_eosio_abi(unsigned long long r, unsigned long long c, unsigned long long a){eosio_assert_code(false, 1);}
}