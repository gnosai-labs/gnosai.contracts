# Song Registry

## transfer notification

`song.reg` accepts `cisum.token::transfer` notifications for song creation and primary purchases.

Song creation uses exactly four colon-separated fields: `SONGCREATE:<issue_count>:<nid>:<token_uri_or_hash>`. The supplied `nid` is used directly as the `gnos.ntoken` `nsymbol.nid`; `song.reg` does not auto-generate it. For now the fourth field is stored directly as the token URI; later it can be treated as a hash that resolves to the URI. Each NFT unit costs the configured `global.nft_price`; if the transferred payment quantity does not equal `issue_count * global.nft_price`, the transfer is rejected. The transfer sender becomes the song creator.

Primary purchases use exactly three colon-separated fields: `SONGBUY:<buy_count>:<nid>`. The `nid` must already exist, `song.reg` must still hold enough inventory for that NFT, and the transferred payment quantity must equal `buy_count * global.nft_price` at the time of purchase. Purchased NFT units are transferred from `song.reg` to the transfer sender. If `global.nft_price` changes later, subsequent purchases use the new price.

## setglobal

Sets the maximum NFT supply for each song and the price of each NFT unit: `setglobal(nfts_per_issue, nft_price)`. The price must be a positive `CISUM` asset. The contract must be configured before it can process creation or purchase transfers. A song creates and issues `nfts_per_issue`; the paid `issue_count` is transferred to the creator, and must not exceed that maximum.
