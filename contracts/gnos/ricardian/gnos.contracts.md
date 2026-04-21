GNOS Protocol Ricardian Contract

This contract implements the GNOS protocol for NFT and TBA (Token Bound Account) management on the Flon blockchain.

Key Terms:
- NFT: Non-Fungible Token representing content (songs, MVs, bundles)
- TBA: Token Bound Account holding assets associated with an NFT
- Bundle: Collection of deliverable assets from a TBA
- Manifest: Snapshot of deliverable assets with hash for integrity

Actions:
- createsong: Register a new song
- createmv: Register a new music video
- mint: Mint an NFT for registered content
- transfernft: Transfer NFT ownership
- createtba: Create a TBA for an NFT
- deposit: Add assets to a TBA
- genmanifest: Generate a bundle manifest
- listnft/listbundle: List NFT or bundle for sale
- cancel: Cancel a listing
- buy: Purchase via token transfer with memo

All actions require proper authorization and validation as per the protocol rules.