#!/bin/bash
set -x
echo cp linux trade API files
cp ../../lib/debug/libEESTraderApi.so "trade/C++/linux64_debug/" -f
echo cp linuxquote API files
cp ../../lib/debug/libEESQuoteApi.so "quote/C++/linux64_debug/" -f
