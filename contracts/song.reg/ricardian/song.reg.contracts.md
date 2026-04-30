# Song Registry

## transfer notification

`song.reg` accepts `cisum.token::transfer` notifications for song creation and primary purchases.

Song creation uses three colon-separated fields: `SONGCREATE:<issue_count>:<nid>`. The supplied `nid` must already exist in the `songs` table from `regsong`; `song.reg` reads that row's `creator`, `token_url`, `music_hash`, and `music_url` before creating the NFT. The `nid` is used directly as the `gnos.ntoken` `nsymbol.nid`; `song.reg` does not auto-generate it. The stored `token_url` is used as the token URI on `gnos.ntoken`, and the stored creator receives the paid `issue_count` NFT units. Each NFT unit costs the configured `global.nft_price`; if the transferred payment quantity does not equal `issue_count * global.nft_price`, the transfer is rejected.

Primary purchases use exactly three colon-separated fields: `SONGBUY:<buy_count>:<nid>`. The `nid` must already exist, `song.reg` must still hold enough inventory for that NFT, and the transferred payment quantity must equal `buy_count * global.nft_price` at the time of purchase. Purchased NFT units are transferred from `song.reg` to the transfer sender. If `global.nft_price` changes later, subsequent purchases use the new price.

## setglobal

Sets the maximum NFT supply for each song and the price of each NFT unit: `setglobal(nfts_per_issue, nft_price)`. The price must be a positive `CISUM` asset. The contract must be configured before it can process creation or purchase transfers. A song creates and issues `nfts_per_issue`; the paid `issue_count` is transferred to the creator, and must not exceed that maximum.

## regsong

Registers song metadata before NFT creation: `regsong(creator, nid, token_url, music_hash, music_url)`. The creator must authorize the action. The `nid`, `token_url`, `music_hash`, and `music_url` are required, and the `nid` must not already exist as a song or NFT.
