test_core_CXXFLAGS = -I$(top_srcdir)/src
test_protocol_CXXFLAGS = -I$(top_srcdir)/src

noinst_PROGRAMS = test_core

test_core_SOURCES = ../src/ch_row.cc \
                    ../src/ch_row.h \
                    ../src/ch_table.cc \
                    ../src/ch_table.h \
                    test_core.cc
test_core_LDADD = -lgtest -lpthread

TESTS = test_core
