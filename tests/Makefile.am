test_unit_CXXFLAGS = -I$(top_srcdir)/src

noinst_PROGRAMS = test_unit

test_unit_SOURCES = ../src/db/abstract_dataset.h \
                    ../src/db/abstract_dataset.cc \
                    ../src/db/dataset.h \
                    ../src/db/dataset.cc \
                    ../src/ml/abstract_model.h \
                    ../src/ml/abstract_model.cc \
                    ../src/ml/linear_regression.h \
                    ../src/ml/linear_regression.cc \
                    ../src/service/dataset_manager.h \
                    ../src/service/dataset_manager.cc \
                    ../src/service/model_manager.h \
                    ../src/service/model_manager.cc \
                    unit/db/dataset.cc \
                    unit/service/dataset_manager.cc \
                    unit/service/model_manager.cc \
                    unit/ml/linear_regression.cc \
                    test_unit.cc
test_unit_LDADD = -lgtest -lpthread -larmadillo

TESTS = test_unit
