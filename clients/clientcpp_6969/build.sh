#!/bin/bash
# exit when any command fails
set -e

# make sure we're in the current dir
cd "$(dirname "$0")"
make clean
make client