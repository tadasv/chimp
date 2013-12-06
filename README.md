Chimp [![Build Status](https://travis-ci.org/tadasv/chimp.png?branch=master)](https://travis-ci.org/tadasv/chimp)
=====

**This project is in a VERY early stage**

Chimp is (or will be) a real-time machine learning service. Chimp aims to be self-contained,
something like memcache, so that it could be easily deployed to build more complex systems
on top of it.

Key Constraints
---------------

* Real-time. Getting back predictions should be nearly instantaneous.
* Asynchronous. Nothing should ever block.
* Extensible. Ability to easily extend ML functionality via plugins or other means.
