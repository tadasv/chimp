#!/bin/bash

sudo apt-get update -qq
sudo apt-get -qq install autoconf automake libtool build-essential git libbsd-dev libgtest-dev cmake
cd /tmp && git clone https://github.com/tadasv/csv_parser.git && cd csv_parser && ./autogen.sh && ./configure && make && sudo make install
cd /tmp && wget "http://libuv.org/dist/v0.10.19/libuv-v0.10.19.tar.gz" && tar zxf libuv-v0.10.19.tar.gz && cd libuv-v0.10.19 && make && sudo cp -r include/* /usr/local/include && sudo cp libuv.* /usr/local/lib
cd /usr/src/gtest && sudo cmake . && sudo make && sudo mv libg* /usr/lib/ && cd -
sudo ldconfig
