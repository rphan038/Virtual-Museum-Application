QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

HEADERS += ../project/curator.h
SOURCES += ../project/curator.cpp

HEADERS += ../project/museumpage.h
SOURCES += ../project/museumpage.cpp

HEADERS += ../project/dbtable.h
SOURCES += ../project/dbtable.cpp

HEADERS += ../project/dbtool.h
SOURCES += ../project/dbtool.cpp

HEADERS += ../project/account.h
SOURCES += ../project/account.cpp

HEADERS += ../project/admin.h
SOURCES += ../project/admin.cpp

HEADERS += ../project/viewer.h
SOURCES += ../project/viewer.cpp

HEADERS += ../project/comment.h
SOURCES += ../project/comment.cpp

HEADERS += ../project/sqlite3.h
SOURCES += ../project/sqlite3.c


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
