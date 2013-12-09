test_unit_CXXFLAGS = -I$(top_srcdir)/src

noinst_PROGRAMS = test_unit

test_unit_SOURCES = ../src/db/row.h \
                    ../src/db/row.cc \
                    ../src/db/dataset.h \
                    ../src/db/dataset.cc \
                    ../src/service/dataset_manager.cc \
                    unit/db/row.cc \
                    unit/db/dataset.cc \
                    unit/service/dataset_manager.cc \
                    test_unit.cc
test_unit_LDADD = -lgtest -lpthread

TESTS = test_unit
