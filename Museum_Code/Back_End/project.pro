TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        account.cpp \
        admin.cpp \
        comment.cpp \
        curator.cpp \
        main.cpp \
        museumpage.cpp \
        dbtable.cpp \
        dbtool.cpp \
        viewer.cpp \
        sqlite3.c

HEADERS += \
    account.h \
    admin.h \
    comment.h \
    curator.h \
    museumpage.h \
    dbtable.h \
    dbtool.h \
    viewer.h \
    sqlite3.h
