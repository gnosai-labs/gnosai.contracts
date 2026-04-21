#!/bin/bash

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
source "$SCRIPT_DIR/assertions.sh"

song_reg=song.reg
mv_reg=mv.reg
ntoken=gnos.ntoken
tba_reg=tba.reg
tba_account=tba.account
mart=nftv.mart
pay_token=cisum.token

owner=flonian
buyer=mywallet2

mv_hash="bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"

mpush $pay_token transfer "[\"$owner\",\"$song_reg\",\"300.0000 CISUM\",\"SONGCREATE:30:6000000000:song-1-json-hash\"]" -p $owner
mpush $song_reg setglobal "[100,\"20.0000 CISUM\"]" -p $song_reg
mpush $pay_token transfer "[\"$buyer\",\"$song_reg\",\"40.0000 CISUM\",\"SONGBUY:2:6000000000\"]" -p $buyer
mpush $song_reg setglobal "[100,\"10.0000 CISUM\"]" -p $song_reg
mpush $mv_reg createmv "[\"$owner\",6000000000,\"GNOS MV #1\",\"ipfs://mv-1.mp4\",\"$mv_hash\",\"ipfs://mv-meta-1.json\"]" -p $owner

mpush $mart listnft "[\"$owner\",{\"amount\":1,\"symbol\":{\"nid\":6000000000}},\"$pay_token\",\"100.0000 CISUM\"]" -p $owner
mpush $mart buy "[\"$buyer\",1]" -p $buyer

mpush $tba_reg createtba "[\"$owner\",\"$ntoken\",{\"amount\":1,\"symbol\":{\"nid\":6000000000}}]" -p $owner
mpush $tba_account deposittoken "[\"$owner\",1,\"$pay_token\",\"200.0000 CISUM\",\"bundle token\"]" -p $owner
mpush $ntoken transfer "[\"$owner\",\"$tba_account\",[{\"amount\":1,\"symbol\":{\"nid\":6000000000}}],\"deposit:1:nested song nft\"]" -p $owner
mpush $mart listbundle "[\"$owner\",1,\"$pay_token\",\"1000.0000 CISUM\"]" -p $owner
mpush $mart buy "[\"$buyer\",2]" -p $buyer

assert_table "$ntoken" "$ntoken" tokenstats \
  '.rows[] | select((.supply.symbol.nid | tostring) == "6000000000" and .supply.amount == 100 and .max_supply.amount == 100 and .token_uri == "song-1-json-hash" and .issuer == "song.reg" and .ipowner == "flonian")' \
  "gnos.ntoken tokenstats created with max 100 and issued 100"

assert_table "$ntoken" "$ntoken" nftglobal \
  '.rows[] | select((.creators | index("song.reg")) and (.whitelist | length == 0))' \
  "gnos.ntoken only restricts creators and leaves transfers unrestricted"

assert_table "$mart" "$mart" paytokens \
  '.rows[] | select(.token_contract == "cisum.token" and .enabled == true)' \
  "market pay token whitelist contains cisum.token"

assert_nft_balance "$ntoken" "$owner" 6000000000 27
assert_nft_balance "$ntoken" "$buyer" 6000000000 4
assert_nft_balance "$ntoken" "$tba_account" 6000000000 1
assert_nft_balance "$ntoken" "$song_reg" 6000000000 68

assert_table "$tba_reg" "$tba_reg" tbamaps \
  '.rows[] | select(.tba_id == 1 and .owner == "mywallet2" and .nft_contract == "gnos.ntoken" and (.root_nft.symbol.nid | tostring) == "6000000000" and .root_nft.amount == 1 and .status == 1)' \
  "bundle sale transfers tba owner to buyer"

assert_table "$tba_account" "$tba_account" tbaassets \
  '.rows | length == 2' \
  "tba has two deposited assets"

assert_table "$tba_account" "$tba_account" tbaassets \
  '.rows[] | select(.tba_id == 1 and .asset_type == 1 and .asset_contract == "cisum.token" and .quantity == "200.0000 CISUM" and .memo == "bundle token")' \
  "tba token deposit recorded"

assert_table "$tba_account" "$tba_account" tbaassets \
  '.rows[] | select(.tba_id == 1 and .asset_type == 2 and .asset_contract == "gnos.ntoken" and (.nft_quantity.symbol.nid | tostring) == "6000000000" and .nft_quantity.amount == 1 and .memo == "nested song nft")' \
  "tba nft deposit recorded"

assert_table "$mart" "$mart" orders \
  '.rows[] | select(.order_id == 1 and .sale_type == 1 and .status == 2 and .seller == "flonian" and .buyer == "mywallet2" and .pay_token_contract == "cisum.token" and .price == "100.0000 CISUM" and (.nft_quantity.symbol.nid | tostring) == "6000000000" and .nft_quantity.amount == 1)' \
  "market single nft order sold"

assert_table "$mart" "$mart" orders \
  '.rows[] | select(.order_id == 2 and .sale_type == 2 and .status == 2 and .seller == "flonian" and .buyer == "mywallet2" and .pay_token_contract == "cisum.token" and .price == "1000.0000 CISUM" and (.nft_quantity.symbol.nid | tostring) == "6000000000" and .nft_quantity.amount == 1)' \
  "market bundle order sold"

echo "GNOS NFT happy path executed. Inspect tokenstats/accounts on $ntoken."
