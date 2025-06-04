#!/usr/bin/env bash
set -xe

compile () {
  make -k all 
}
run() {
  ELA_TMP="$(mktemp)"
  ./bin/elalang 2> "$ELA_TMP"
  bat "$ELA_TMP" --language=LLVM
}
compile && run
