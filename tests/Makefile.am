AM_CPPFLAGS = -I$(top_srcdir)/src


noinst_PROGRAMS = test_core

test_core_LDFLAGS = -lcheck
test_core_SOURCES = ../src/ch_list.c \
                    ../src/ch_list.h \
                    ../src/ch_string.c \
                    ../src/ch_string.h \
                    ../src/ch_hash_table.c \
                    ../src/ch_hash_table.h \
                    ../src/ch_row.c \
                    ../src/ch_row.h \
                    ../src/ch_table.c \
                    ../src/ch_table.h \
                    ../src/murmur.c \
                    ../src/murmur.h \
                    test_core.c

TESTS = test_core
