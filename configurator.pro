#-------------------------------------------------
#
# Project created by QtCreator 2017-11-05T15:31:39
#
#-------------------------------------------------

QMAKE_EXTRA_TARGETS += before_build makefilehook
 
makefilehook.target = $(MAKEFILE)
makefilehook.depends = .beforebuild
 
PRE_TARGETDEPS += .beforebuild
 
before_build.target = .beforebuild
before_build.depends = FORCE
before_build.commands = chcp 1251

QT += core gui serialport serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = configurator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        configuratorwindow.cpp \
        qpanel.cpp \
        qcell.cpp \
        cmodbus.cpp \
        cdataunittype.cpp

HEADERS += \
        configuratorwindow.h \
        qpanel.h \
        qcell.h \
        cmodbus.h \
        cdataunittype.h

FORMS += \
        configuratorwindow.ui \
        qcell.ui

win32::RC_FILE = \
                 resource/icon.rc

RESOURCES += \
             resource.qrc
