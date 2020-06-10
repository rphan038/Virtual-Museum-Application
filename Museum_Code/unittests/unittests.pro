TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

# if you have pthread errors, comment/uncomment the next line
LIBS += -pthread

SOURCES += ../gtest/googletest/src/gtest-all.cc

INCLUDEPATH +=  ../gtest                                   \
                ../gtest/googletest                        \
                ../gtest/googletest/include                \
                ../gtest/googletest/include/gtest

HEADERS += ../project/comment.h
SOURCES += ../project/comment.cpp
HEADERS += ../project/account.h
SOURCES += ../project/account.cpp
HEADERS += ../project/admin.h
SOURCES += ../project/admin.cpp
HEADERS += ../project/curator.h
SOURCES += ../project/curator.cpp
HEADERS += ../project/dbtable.h
SOURCES += ../project/dbtable.cpp
HEADERS += ../project/dbtool.h
SOURCES += ../project/dbtool.cpp
HEADERS += ../project/museumpage.h
SOURCES += ../project/museumpage.cpp
HEADERS += ../project/viewer.h
SOURCES += ../project/viewer.cpp
HEADERS += ../project/sqlite3.h
SOURCES += ../project/sqlite3.c
#
#HEADERS += ../tools/log.h
#SOURCES += ../tools/log.cpp


### add any other source or header files you may need
