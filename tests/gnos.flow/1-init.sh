#!/bin/bash

set -euo pipefail

song_reg=song.reg
mv_reg=mv1.reg
ntoken=gnos.ntoken
tba_reg=tba1.reg
tba_account=tba1.account
mart=nftv1.mart
dao=gnos1.dao
stake=gnos1.stake

payment_token=cisum.token
owner=flonian
buyer=mywallet2
platform=flonian

mreg flon $song_reg $owner
mtran $owner $song_reg "100 FLON"
mset $song_reg song.reg
mcli set account permission $song_reg active --add-code

mreg flon $mv_reg $owner
mtran $owner $mv_reg "100 FLON"
mset $mv_reg mv.reg
mcli set account permission $mv_reg active --add-code

mreg flon $ntoken $owner
mtran $owner $ntoken "100 FLON"
mset $ntoken gnos.ntoken
mcli set account permission $ntoken active --add-code

mreg flon $tba_reg $owner
mtran $owner $tba_reg "100 FLON"
mset $tba_reg tba.reg
mcli set account permission $tba_reg active --add-code

mreg flon $tba_account $owner
mtran $owner $tba_account "100 FLON"
mset $tba_account tba.account
mcli set account permission $tba_account active --add-code

mreg flon $mart $owner
mtran $owner $mart "100 FLON"
mset $mart nftv.mart
mcli set account permission $mart active --add-code

mreg flon $dao $owner
mtran $owner $dao "100 FLON"
mset $dao gnos.dao
mcli set account permission $dao active --add-code

mreg flon $stake $owner
mtran $owner $stake "100 FLON"
mset $stake gnos.stake
mcli set account permission $stake active --add-code

mpush $song_reg setglobal "[100,\"10.0000 CISUM\"]" -p $song_reg
mpush $dao setfee "[100,500,400,\"$platform\",\"$stake\"]" -p $dao
mpush $mart setpaytoken "[\"$payment_token\",true]" -p $mart
mpush $payment_token addconsumewl "[\"$song_reg\"]" -p $payment_token
mpush $ntoken setcreator "[\"$song_reg\",true]" -p $ntoken




