#!/bin/bash

set -euo pipefail

song_reg=song.reg
pay_token=cisum.token
payment_token=cisum.token

owner=flonian
buyer=flonian

# 允许普通用户把 CISUM 转给 song.reg 这类消费合约
mpush $payment_token addconsumewl "[\"$song_reg\"]" -p $payment_token

# 注册歌曲，并创建歌曲 NFT：
# 当前 global 价格是 10.0000 CISUM
# 创建 30 个给 creator，所以付款 30 * 10 = 300.0000 CISUM SONGCREATE:<issue_count>:<nid>
mpush $song_reg regsong "[\"$owner\",6000000008,\"song-9-json-hash\",\"song-9-music-hash\",\"ipfs://song-3.mp3\"]" -p $owner
mpush $pay_token transfer "[\"$owner\",\"$song_reg\",\"300.0000 CISUM\",\"SONGCREATE:30:6000000008\"]" -p $owner

# 修改当前全局价格为 20.0000 CISUM
mpush $song_reg setglobal "[100,\"20.0000 CISUM\"]" -p $song_reg

# 按当前价格购买 4 个：
# 4 * 20 = 80.0000 CISUM
mpush $pay_token transfer "[\"$buyer\",\"$song_reg\",\"80.0000 CISUM\",\"SONGBUY:4:6000000000\"]" -p $buyer

# 继续按当前价格购买剩余 68 个：
# 68 * 20 = 1360.0000 CISUM
mpush $pay_token transfer "[\"$buyer\",\"$song_reg\",\"1360.0000 CISUM\",\"SONGBUY:68:6000000000\"]" -p $buyer
