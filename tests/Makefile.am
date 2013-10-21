AM_CPPFLAGS = -I$(top_builddir)/include \
              -I$(top_srcdir)/src


bin_PROGRAMS = test_core

test_core_LDFLAGS = -lcheck
test_core_SOURCES = ../src/ch_list.c \
                    ../src/ch_list.h \
                    ../src/ch_string.c \
                    ../src/ch_string.h \
                    test_core.c
