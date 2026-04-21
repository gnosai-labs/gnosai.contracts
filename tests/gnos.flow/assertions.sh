#!/bin/bash

set -euo pipefail

MCLI_BIN=${MCLI_BIN:-mcli}

require_jq() {
  if ! command -v jq >/dev/null 2>&1; then
    echo "jq is required for flow assertions"
    exit 1
  fi
}

table_json() {
  local contract=$1
  local scope=$2
  local table=$3
  $MCLI_BIN get table "$contract" "$scope" "$table"
}

assert_table() {
  local contract=$1
  local scope=$2
  local table=$3
  local filter=$4
  local label=$5

  require_jq
  if ! table_json "$contract" "$scope" "$table" | jq -e "$filter" >/dev/null; then
    echo "assertion failed: $label"
    echo "table: $contract $scope $table"
    echo "filter: $filter"
    table_json "$contract" "$scope" "$table"
    exit 1
  fi
}

assert_nft_balance() {
  local ntoken=$1
  local owner=$2
  local nid=$3
  local amount=$4

  assert_table "$ntoken" "$owner" accounts \
    ".rows[] | select((.balance.symbol.nid | tostring) == \"$nid\" and .balance.amount == $amount)" \
    "$owner has $amount of nft nid $nid"
}
