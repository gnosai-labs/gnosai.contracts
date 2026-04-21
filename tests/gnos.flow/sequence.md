# GNOS Flow

1. 部署模块合约
2. 配置 `nftv.mart` 分账参数
3. 向 `song.reg` 转账 CISUM，memo `SONGCREATE:<issue_count>:<nid>:<token_uri_or_hash>`，自动注册歌曲并铸造 NFT
4. 其他用户向 `song.reg` 转账 CISUM，memo `SONGBUY:<buy_count>:<nid>`，购买指定 NFT 的剩余库存
5. 注册 MV `mv.reg::createmv`
6. `gnos.ntoken` 配置 transfer 白名单，仅允许市场/TBA 托管路径转移 NFT
7. 使用 `nftv.mart::listnft` 上架单个 NFT，显式指定白名单支付币合约和价格
8. 买家调用 `nftv.mart::buy` 完成单 NFT 成交
9. 创建 TBA `tba.reg::createtba`
10. 使用 `tba.account::deposittoken` 存入普通 token，使用 NFT transfer memo `deposit:<tba_id>` 存入 `gnos.ntoken`
11. 上架 bundle `nftv.mart::listbundle`，显式指定白名单支付币合约和价格
12. 买家调用 `nftv.mart::buy` 完成 bundle 成交
