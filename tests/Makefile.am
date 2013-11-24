test_core_CXXFLAGS = -I$(top_srcdir)/src
test_protocol_CXXFLAGS = -I$(top_srcdir)/src

noinst_PROGRAMS = test_core test_protocol

test_core_SOURCES = ../src/ch_list.cc \
                    ../src/ch_list.h \
                    ../src/ch_string.cc \
                    ../src/ch_string.h \
                    ../src/ch_hash_table.cc \
                    ../src/ch_hash_table.h \
                    ../src/ch_row.cc \
                    ../src/ch_row.h \
                    ../src/ch_table.cc \
                    ../src/ch_table.h \
                    ../src/murmur.cc \
                    ../src/murmur.h \
                    test_core.cc
test_core_LDADD = -lgtest -lpthread

test_protocol_SOURCES = ../src/ch_protocol.h \
                        ../src/ch_protocol.cc \
                        test_protocol.cc
test_protocol_LDADD = -lgtest -lpthread -lmsgpack

TESTS = test_core test_protocol
