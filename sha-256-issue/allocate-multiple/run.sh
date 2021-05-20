#!/usr/bin/env bash
cd "$(dirname "${BASH_SOURCE[0]}")"
set -euxo pipefail

clang -g3 -o a.out main.c sha-256.c
./a.out
