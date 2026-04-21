#include "/Users/aaron/Documents/flon/gnos.contracts/contracts/nftv.mart/src/nftv.mart.cpp"


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("listnft:__eosio_action_listnft_nftvmart"))) void __eosio_action_listnft_nftvmart(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
flon::nasset arg1; ds >> arg1;
eosio::name arg2; ds >> arg2;
eosio::asset arg3; ds >> arg3;
nftvmart{eosio::name{r},eosio::name{c},ds}.listnft(arg0, arg1, arg2, arg3);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("listbundle:__eosio_action_listbundle_nftvmart"))) void __eosio_action_listbundle_nftvmart(unsigned long long r, unsigned long long c) {
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
nftvmart{eosio::name{r},eosio::name{c},ds}.listbundle(arg0, arg1, arg2, arg3);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("cancel:__eosio_action_cancel_nftvmart"))) void __eosio_action_cancel_nftvmart(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
uint64_t arg1; ds >> arg1;
nftvmart{eosio::name{r},eosio::name{c},ds}.cancel(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("buy:__eosio_action_buy_nftvmart"))) void __eosio_action_buy_nftvmart(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
uint64_t arg1; ds >> arg1;
nftvmart{eosio::name{r},eosio::name{c},ds}.buy(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("setpaytoken:__eosio_action_setpaytoken_nftvmart"))) void __eosio_action_setpaytoken_nftvmart(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
bool arg1; ds >> arg1;
nftvmart{eosio::name{r},eosio::name{c},ds}.setpaytoken(arg0, arg1);
}}

extern "C" {
__attribute__((eosio_wasm_import))
void eosio_assert_code(uint32_t, uint64_t);	__attribute__((weak, eosio_wasm_entry, eosio_wasm_abi("{\"____comment\":\"This file was generated with eosio-abigen. DO NOT EDIT \",\"version\":\"flon::abi/1.2\",\"structs\":[{\"name\":\"buy\",\"base\":\"\",\"fields\":[{\"name\":\"buyer\",\"type\":\"name\"},{\"name\":\"order_id\",\"type\":\"uint64\"}]},{\"name\":\"cancel\",\"base\":\"\",\"fields\":[{\"name\":\"seller\",\"type\":\"name\"},{\"name\":\"order_id\",\"type\":\"uint64\"}]},{\"name\":\"listbundle\",\"base\":\"\",\"fields\":[{\"name\":\"seller\",\"type\":\"name\"},{\"name\":\"tba_id\",\"type\":\"uint64\"},{\"name\":\"pay_token_contract\",\"type\":\"name\"},{\"name\":\"price\",\"type\":\"asset\"}]},{\"name\":\"listnft\",\"base\":\"\",\"fields\":[{\"name\":\"seller\",\"type\":\"name\"},{\"name\":\"nft_quantity\",\"type\":\"nasset\"},{\"name\":\"pay_token_contract\",\"type\":\"name\"},{\"name\":\"price\",\"type\":\"asset\"}]},{\"name\":\"nasset\",\"base\":\"\",\"fields\":[{\"name\":\"amount\",\"type\":\"int64\"},{\"name\":\"symbol\",\"type\":\"nsymbol\"}]},{\"name\":\"nsymbol\",\"base\":\"\",\"fields\":[{\"name\":\"nid\",\"type\":\"uint64\"}]},{\"name\":\"setpaytoken\",\"base\":\"\",\"fields\":[{\"name\":\"token_contract\",\"type\":\"name\"},{\"name\":\"enabled\",\"type\":\"bool\"}]}],\"types\":[],\"actions\":[{\"name\":\"buy\",\"type\":\"buy\",\"ricardian_contract\":\"\"},{\"name\":\"cancel\",\"type\":\"cancel\",\"ricardian_contract\":\"\"},{\"name\":\"listbundle\",\"type\":\"listbundle\",\"ricardian_contract\":\"\"},{\"name\":\"listnft\",\"type\":\"listnft\",\"ricardian_contract\":\"\"},{\"name\":\"setpaytoken\",\"type\":\"setpaytoken\",\"ricardian_contract\":\"\"}],\"tables\":[],\"ricardian_clauses\":[],\"variants\":[],\"abi_extensions\":[],\"action_results\":[]}")))
	void __insert_eosio_abi(unsigned long long r, unsigned long long c, unsigned long long a){eosio_assert_code(false, 1);}
}