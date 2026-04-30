# GNOS Flow

1. 部署模块合约
2. 配置 `nftv.mart` 分账参数
3. 调用 `song.reg::regsong` 登记歌曲 `nid/token_url/music_hash/music_url`
4. 向 `song.reg` 转账 CISUM，memo `SONGCREATE:<issue_count>:<nid>`，合约读取已登记歌曲信息并铸造 NFT
5. 其他用户向 `song.reg` 转账 CISUM，memo `SONGBUY:<buy_count>:<nid>`，购买指定 NFT 的剩余库存
6. 注册 MV `mv.reg::createmv`
7. `gnos.ntoken` 配置 transfer 白名单，仅允许市场/TBA 托管路径转移 NFT
8. 使用 `nftv.mart::listnft` 上架单个 NFT，显式指定白名单支付币合约和价格
9. 买家调用 `nftv.mart::buy` 完成单 NFT 成交
10. 创建 TBA `tba.reg::createtba`
11. 使用 `tba.account::deposittoken` 存入普通 token，使用 NFT transfer memo `deposit:<tba_id>` 存入 `gnos.ntoken`
12. 上架 bundle `nftv.mart::listbundle`，显式指定白名单支付币合约和价格
13. 买家调用 `nftv.mart::buy` 完成 bundle 成交
