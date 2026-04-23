#!/bin/bash

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
source "$SCRIPT_DIR/assertions.sh"

ntoken=gnos.ntoken
song_reg=song.reg
tba_reg=tba.reg
tba_account=tba.account
mart=nftv.mart
pay_token=cisum.token

owner=flonian
buyer=mywallet2

expect_fail() {
  if "$@"; then
    echo "expected failure but command succeeded: $*"
    exit 1
  fi
}

# Prerequisite: run 1-init.sh and 2-happy-path.sh first.
# Song #1 creates symbol nid 6000000000. NFT transfers are unrestricted; only creation is creator-whitelisted.

expect_fail mpush $ntoken transfer "[\"$owner\",\"$owner\",[{\"amount\":1,\"symbol\":{\"nid\":6000000000}}],\"self transfer\"]" -p $owner
expect_fail mpush $ntoken transfer "[\"$buyer\",\"$owner\",[{\"amount\":999,\"symbol\":{\"nid\":6000000000}}],\"overdraw\"]" -p $buyer
expect_fail mpush $ntoken transfer "[\"$owner\",\"$buyer\",[{\"amount\":1,\"symbol\":{\"nid\":9999999999}}],\"missing symbol\"]" -p $owner
expect_fail mpush $ntoken create "[\"$owner\",1,{\"nid\":6000000001},\"bad-direct-create-hash\",\"$owner\"]" -p $owner
mpush $ntoken transfer "[\"$owner\",\"$buyer\",[{\"amount\":1,\"symbol\":{\"nid\":6000000000}}],\"direct transfer allowed\"]" -p $owner
mpush $ntoken transfer "[\"$buyer\",\"$owner\",[{\"amount\":1,\"symbol\":{\"nid\":6000000000}}],\"direct transfer return\"]" -p $buyer
expect_fail mpush $pay_token transfer "[\"$owner\",\"$song_reg\",\"10.0000 CISUM\",\"BADTYPE:1:6000000001:bad-song-hash\"]" -p $owner
expect_fail mpush $pay_token transfer "[\"$owner\",\"$song_reg\",\"10.0000 CISUM\",\"SONGCREATE:1:6000000001:old-song-hash\"]" -p $owner
expect_fail mpush $pay_token transfer "[\"$owner\",\"$song_reg\",\"10.0000 CISUM\",\"SONGCREATE:2:6000000001:bad-amount-song-hash:bad-music-hash:ipfs://bad-song.mp3\"]" -p $owner
expect_fail mpush $pay_token transfer "[\"$owner\",\"$song_reg\",\"1010.0000 CISUM\",\"SONGCREATE:101:6000000001:too-many-song-hash:too-many-music-hash:ipfs://too-many-song.mp3\"]" -p $owner
expect_fail mpush $pay_token transfer "[\"$buyer\",\"$song_reg\",\"10.0000 CISUM\",\"SONGBUY:2:6000000000\"]" -p $buyer
expect_fail mpush $pay_token transfer "[\"$buyer\",\"$song_reg\",\"10.0000 CISUM\",\"SONGBUY:1:9999999999\"]" -p $buyer
expect_fail mpush $song_reg setglobal "[100,\"10.0000 FAKE\"]" -p $song_reg
expect_fail mpush $tba_reg createtba "[\"$owner\",\"$ntoken\",{\"amount\":999,\"symbol\":{\"nid\":6000000000}}]" -p $owner
expect_fail mpush $tba_account deposittoken "[\"$owner\",1,\"$pay_token\",\"1.0000 CISUM\",\"late deposit after sale\"]" -p $owner
expect_fail mpush $mart listnft "[\"$owner\",{\"amount\":999,\"symbol\":{\"nid\":6000000000}},\"$pay_token\",\"1.0000 CISUM\"]" -p $owner
expect_fail mpush $mart listnft "[\"$owner\",{\"amount\":1,\"symbol\":{\"nid\":6000000000}},\"fake.token\",\"1.00000000 FAKE\"]" -p $owner
expect_fail mpush $mart buy "[\"$buyer\",1]" -p $buyer
expect_fail mpush $mart buy "[\"$buyer\",999]" -p $buyer

assert_nft_balance "$ntoken" "$owner" 6000000000 27
assert_nft_balance "$ntoken" "$buyer" 6000000000 4
assert_nft_balance "$ntoken" "$tba_account" 6000000000 1
assert_nft_balance "$ntoken" "$song_reg" 6000000000 68

echo "Guardrail checks passed."
