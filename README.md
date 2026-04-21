# GNOS Contracts

GNOS 协议已拆成多合约结构：

- `song.reg`: 歌曲注册
- `mv.reg`: MV 注册
- `gnos.ntoken`: 对齐 `ticket.cvnft` 的 `nasset/nsymbol` NFT 资产，支持 `transfer`
- `tba.reg`: NFT 与 TBA 映射
- `tba.account`: TBA 资产容器
- `nftv.mart`: NFT / Bundle 市场
- `gnos.stake`: 质押收益池

公共协议结构放在 `contracts/libs/base/include/gnos.protocol/gnos.protocol.hpp`。
