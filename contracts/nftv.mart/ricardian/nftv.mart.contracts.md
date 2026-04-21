# NFTV Market

Marketplace orders use `gnos.ntoken` `nasset` quantities and an explicit payment token contract plus `asset` price. Listing escrows the NFT into `nftv.mart`; cancelling returns it to the seller; `buy` transfers payment from the buyer, settles fees, and transfers the NFT to the buyer. Fee configuration is managed directly by `nftv.mart::setfee`.
