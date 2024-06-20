#!/bin/bash

set -e

echo $(pwd)

cd TestsBinaries

python3 print_logs.py
