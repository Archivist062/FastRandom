#!/bin/sh

if [ $(./build/apps/garbage_out | dieharder -g 200 -a | grep PASSED | wc -l) -lt 98 ]; then
    exit 1
fi
exit 0