#!/bin/bash

sudo apt-get update
sudo apt-get -y install autoconf automake libtool build-essential git valgrind libbsd-dev vim cmake libgtest-dev libmsgpack-dev python-setuptools libarmadillo-dev libboost-math-dev
sudo easy_install msgpack-python
cd /tmp && wget "http://libuv.org/dist/v0.10.19/libuv-v0.10.19.tar.gz" && tar zxf libuv-v0.10.19.tar.gz && cd libuv-v0.10.19 && make && sudo cp -r include/* /usr/local/include && sudo cp libuv.* /usr/local/lib
cd /usr/src/gtest && sudo cmake . && sudo make && sudo mv libg* /usr/lib/ && cd -

sudo ldconfig
