chimp_CXXFLAGS = -I$(top_srcdir)/src

bin_PROGRAMS = chimp

chimp_LDADD = -larmadillo

chimp_SOURCES = core/logging.cc \
                core/logging.h \
                service/dataset_manager.h \
                service/dataset_manager.cc \
                service/model_manager.h \
                service/model_manager.cc \
                db/abstract_dataset.h \
                db/abstract_dataset.cc \
                db/dataset.h \
                db/dataset.cc \
                ml/abstract_model.h \
                ml/abstract_model.cc \
                ml/linear_regression.h \
                ml/linear_regression.cc \
                transport/abstract_response.h \
                transport/abstract_response.cc \
                transport/response.h \
                transport/response.cc \
                transport/command/abstract_command.h \
                transport/command/abstract_command.cc \
                transport/command/ping.h \
                transport/command/ping.cc \
                transport/command/dsnew.h \
                transport/command/dsnew.cc \
                transport/command/dsappend.h \
                transport/command/dsappend.cc \
                transport/command/dslist.h \
                transport/command/dslist.cc \
                transport/command/shutdown.h \
                transport/command/shutdown.cc \
                transport/server.h \
                transport/server.cc \
                transport/client.h \
                transport/client.cc \
                main.cc
