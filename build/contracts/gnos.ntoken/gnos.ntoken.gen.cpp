#include "/Users/aaron/Documents/flon/gnos.contracts/contracts/gnos.ntoken/src/gnos.ntoken.cpp"


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("create:__eosio_action_create_gnosntoken"))) void __eosio_action_create_gnosntoken(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
int64_t arg1; ds >> arg1;
flon::nsymbol arg2; ds >> arg2;
std::string arg3; ds >> arg3;
eosio::name arg4; ds >> arg4;
gnos::gnosntoken{eosio::name{r},eosio::name{c},ds}.create(arg0, arg1, arg2, arg3, arg4);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("issue:__eosio_action_issue_gnosntoken"))) void __eosio_action_issue_gnosntoken(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
flon::nasset arg1; ds >> arg1;
std::string arg2; ds >> arg2;
gnos::gnosntoken{eosio::name{r},eosio::name{c},ds}.issue(arg0, arg1, arg2);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("retire:__eosio_action_retire_gnosntoken"))) void __eosio_action_retire_gnosntoken(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
flon::nasset arg0; ds >> arg0;
std::string arg1; ds >> arg1;
gnos::gnosntoken{eosio::name{r},eosio::name{c},ds}.retire(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("transfer:__eosio_action_transfer_gnosntoken"))) void __eosio_action_transfer_gnosntoken(unsigned long long r, unsigned long long c) {
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
gnos::gnosntoken{eosio::name{r},eosio::name{c},ds}.transfer(arg0, arg1, arg2, arg3);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("setnotary:__eosio_action_setnotary_gnosntoken"))) void __eosio_action_setnotary_gnosntoken(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
bool arg1; ds >> arg1;
gnos::gnosntoken{eosio::name{r},eosio::name{c},ds}.setnotary(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("setipowner:__eosio_action_setipowner_gnosntoken"))) void __eosio_action_setipowner_gnosntoken(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
eosio::name arg1; ds >> arg1;
gnos::gnosntoken{eosio::name{r},eosio::name{c},ds}.setipowner(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("addwhitelist:__eosio_action_addwhitelist_gnosntoken"))) void __eosio_action_addwhitelist_gnosntoken(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
gnos::gnosntoken{eosio::name{r},eosio::name{c},ds}.addwhitelist(arg0);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("delwhitelist:__eosio_action_delwhitelist_gnosntoken"))) void __eosio_action_delwhitelist_gnosntoken(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
gnos::gnosntoken{eosio::name{r},eosio::name{c},ds}.delwhitelist(arg0);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("settokenuri:__eosio_action_settokenuri_gnosntoken"))) void __eosio_action_settokenuri_gnosntoken(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
uint64_t arg0; ds >> arg0;
std::string arg1; ds >> arg1;
gnos::gnosntoken{eosio::name{r},eosio::name{c},ds}.settokenuri(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("notarize:__eosio_action_notarize_gnosntoken"))) void __eosio_action_notarize_gnosntoken(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
uint32_t arg1; ds >> arg1;
gnos::gnosntoken{eosio::name{r},eosio::name{c},ds}.notarize(arg0, arg1);
}}


#include <eosio/datastream.hpp>
#include <eosio/name.hpp>
extern "C" {
__attribute__((eosio_wasm_import))
uint32_t action_data_size();
__attribute__((eosio_wasm_import))
uint32_t read_action_data(void*, uint32_t);
__attribute__((weak, eosio_wasm_action("setcreator:__eosio_action_setcreator_gnosntoken"))) void __eosio_action_setcreator_gnosntoken(unsigned long long r, unsigned long long c) {
size_t as = ::action_data_size();
void* buff = nullptr;
if (as > 0) {
buff = as >= 512 ? malloc(as) : alloca(as);
::read_action_data(buff, as);
}
eosio::datastream<const char*> ds{(char*)buff, as};
eosio::name arg0; ds >> arg0;
bool arg1; ds >> arg1;
gnos::gnosntoken{eosio::name{r},eosio::name{c},ds}.setcreator(arg0, arg1);
}}

extern "C" {
__attribute__((eosio_wasm_import))
void eosio_assert_code(uint32_t, uint64_t);	__attribute__((weak, eosio_wasm_entry, eosio_wasm_abi("{\"____comment\":\"This file was generated with eosio-abigen. DO NOT EDIT \",\"version\":\"flon::abi/1.2\",\"structs\":[{\"name\":\"addwhitelist\",\"base\":\"\",\"fields\":[{\"name\":\"acct\",\"type\":\"name\"}]},{\"name\":\"create\",\"base\":\"\",\"fields\":[{\"name\":\"issuer\",\"type\":\"name\"},{\"name\":\"maximum_supply\",\"type\":\"int64\"},{\"name\":\"symbol\",\"type\":\"nsymbol\"},{\"name\":\"token_uri\",\"type\":\"string\"},{\"name\":\"ipowner\",\"type\":\"name\"}]},{\"name\":\"delwhitelist\",\"base\":\"\",\"fields\":[{\"name\":\"acct\",\"type\":\"name\"}]},{\"name\":\"issue\",\"base\":\"\",\"fields\":[{\"name\":\"to\",\"type\":\"name\"},{\"name\":\"quantity\",\"type\":\"nasset\"},{\"name\":\"memo\",\"type\":\"string\"}]},{\"name\":\"nasset\",\"base\":\"\",\"fields\":[{\"name\":\"amount\",\"type\":\"int64\"},{\"name\":\"symbol\",\"type\":\"nsymbol\"}]},{\"name\":\"nft_account_t\",\"base\":\"\",\"fields\":[{\"name\":\"balance\",\"type\":\"nasset\"},{\"name\":\"paused\",\"type\":\"bool\"}]},{\"name\":\"nft_global_t\",\"base\":\"\",\"fields\":[{\"name\":\"creators\",\"type\":\"name[]\"},{\"name\":\"notaries\",\"type\":\"name[]\"},{\"name\":\"whitelist\",\"type\":\"name[]\"}]},{\"name\":\"nft_stat_t\",\"base\":\"\",\"fields\":[{\"name\":\"supply\",\"type\":\"nasset\"},{\"name\":\"max_supply\",\"type\":\"nasset\"},{\"name\":\"token_uri\",\"type\":\"string\"},{\"name\":\"ipowner\",\"type\":\"name\"},{\"name\":\"notary\",\"type\":\"name\"},{\"name\":\"issuer\",\"type\":\"name\"},{\"name\":\"issued_at\",\"type\":\"time_point_sec\"},{\"name\":\"notarized_at\",\"type\":\"time_point_sec\"},{\"name\":\"paused\",\"type\":\"bool\"}]},{\"name\":\"notarize\",\"base\":\"\",\"fields\":[{\"name\":\"notary\",\"type\":\"name\"},{\"name\":\"token_id\",\"type\":\"uint32\"}]},{\"name\":\"nsymbol\",\"base\":\"\",\"fields\":[{\"name\":\"nid\",\"type\":\"uint64\"}]},{\"name\":\"retire\",\"base\":\"\",\"fields\":[{\"name\":\"quantity\",\"type\":\"nasset\"},{\"name\":\"memo\",\"type\":\"string\"}]},{\"name\":\"setcreator\",\"base\":\"\",\"fields\":[{\"name\":\"creator\",\"type\":\"name\"},{\"name\":\"to_add\",\"type\":\"bool\"}]},{\"name\":\"setipowner\",\"base\":\"\",\"fields\":[{\"name\":\"symbid\",\"type\":\"uint64\"},{\"name\":\"ip_owner\",\"type\":\"name\"}]},{\"name\":\"setnotary\",\"base\":\"\",\"fields\":[{\"name\":\"notary\",\"type\":\"name\"},{\"name\":\"to_add\",\"type\":\"bool\"}]},{\"name\":\"settokenuri\",\"base\":\"\",\"fields\":[{\"name\":\"symbid\",\"type\":\"uint64\"},{\"name\":\"url\",\"type\":\"string\"}]},{\"name\":\"transfer\",\"base\":\"\",\"fields\":[{\"name\":\"from\",\"type\":\"name\"},{\"name\":\"to\",\"type\":\"name\"},{\"name\":\"assets\",\"type\":\"nasset[]\"},{\"name\":\"memo\",\"type\":\"string\"}]}],\"types\":[],\"actions\":[{\"name\":\"addwhitelist\",\"type\":\"addwhitelist\",\"ricardian_contract\":\"\"},{\"name\":\"create\",\"type\":\"create\",\"ricardian_contract\":\"\"},{\"name\":\"delwhitelist\",\"type\":\"delwhitelist\",\"ricardian_contract\":\"\"},{\"name\":\"issue\",\"type\":\"issue\",\"ricardian_contract\":\"\"},{\"name\":\"notarize\",\"type\":\"notarize\",\"ricardian_contract\":\"\"},{\"name\":\"retire\",\"type\":\"retire\",\"ricardian_contract\":\"\"},{\"name\":\"setcreator\",\"type\":\"setcreator\",\"ricardian_contract\":\"\"},{\"name\":\"setipowner\",\"type\":\"setipowner\",\"ricardian_contract\":\"\"},{\"name\":\"setnotary\",\"type\":\"setnotary\",\"ricardian_contract\":\"\"},{\"name\":\"settokenuri\",\"type\":\"settokenuri\",\"ricardian_contract\":\"\"},{\"name\":\"transfer\",\"type\":\"transfer\",\"ricardian_contract\":\"\"}],\"tables\":[{\"name\":\"accounts\",\"type\":\"nft_account_t\",\"index_type\":\"i64\",\"key_names\":[],\"key_types\":[]},{\"name\":\"nftglobal\",\"type\":\"nft_global_t\",\"index_type\":\"i64\",\"key_names\":[],\"key_types\":[]},{\"name\":\"tokenstats\",\"type\":\"nft_stat_t\",\"index_type\":\"i64\",\"key_names\":[],\"key_types\":[]}],\"ricardian_clauses\":[],\"variants\":[],\"abi_extensions\":[],\"action_results\":[]}")))
	void __insert_eosio_abi(unsigned long long r, unsigned long long c, unsigned long long a){eosio_assert_code(false, 1);}
}