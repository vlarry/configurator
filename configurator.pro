#-------------------------------------------------
#
# Project created by QtCreator 2017-11-05T15:31:39
#
#-------------------------------------------------

QT += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = configurator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
           QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += c++17

SOURCES += \
        main.cpp \
        configuratorwindow.cpp \
        qcell.cpp \
        cterminal.cpp \
        qcustomplot.cpp \
        cversionsoftware.cpp \
        cmatrixpurposemodel.cpp \
        cheadertable.cpp \
        cserialportsetting.cpp \
        cstatusbar.cpp \
        ctablewidgetitem.cpp \
        ceventlistjournals.cpp \
        cprogressbarwidget.cpp \
        cfilterdialog.cpp \
        cfilter.cpp \
        cheaderjournal.cpp \
        cjournalwidget.cpp \
        cjournaltable.cpp \
        cjournalpropertymodel.cpp \
        cjournalpropertydelegate.cpp \
        HierarchicalHeaderView.cpp \
        ProxyModelWithHeaderModels.cpp \
        clineedit.cpp \
        cmatrix.cpp \
        cvaribalelist.cpp \
        cvariablewidget.cpp \
        cindicatorstate.cpp \
        cindicatorcell.cpp \
        cdockpanelitemctrl.cpp \
        cmonitorpurpose.cpp \
        coutputall.cpp \
        coutputallcell.cpp \
        ctimefilterdialog.cpp \
        cdebuginfo.cpp \
        cstatusinfo.cpp \
        classes/modbus/modbusdataunit.cpp \
        classes/modbus/modbus.cpp \
        classes/modbus/connect.cpp \
        classes/popup/popup.cpp \
        classes/menubar/menubar.cpp \
        classes/menubar/widgetmenu.cpp \
        classes/tabwidget/tabwidget.cpp \
        classes/calibrationwidget/calibrationwidget.cpp \
        classes/tableviews/purposetableview.cpp \
        classes/tabledevicemenu/devicemenumodel.cpp \
        classes/menubar/toolbutton.cpp \
        classes/user/userdialog.cpp \
        classes/interval/intervaldialog.cpp \
        classes/widget/widget.cpp \
        classes/terminal/terminalwindow.cpp \
    classes/widget/dockwidget.cpp

HEADERS += \
        configuratorwindow.h \
        qcell.h \
        cterminal.h \
        qcustomplot.h \
        cversionsoftware.h \
        cmatrixpurposemodel.h \
        cheadertable.h \
        cserialportsetting.h \
        cstatusbar.h \
        ctablewidgetitem.h \
        ceventlistjournals.h \
        cprogressbarwidget.h \
        cfilterdialog.h \
        cfilter.h \
        cheaderjournal.h \
        cjournalwidget.h \
        cjournaltable.h \
        cjournalpropertymodel.h \
        cjournalpropertydelegate.h \
        HierarchicalHeaderView.h \
        ProxyModelWithHeaderModels.h \
        clineedit.h \
        cmatrix.h \
        cvaribalelist.h \
        cvariablewidget.h \
        cindicatorstate.h \
        cindicatorcell.h \
        cdockpanelitemctrl.h \
        cmonitorpurpose.h \
        coutputall.h \
        coutputallcell.h \
        ctimefilterdialog.h \
        cdebuginfo.h \
        cstatusinfo.h \
        classes/modbus/modbusdataunit.h \
        classes/modbus/modbus.h \
        classes/modbus/connect.h \
        classes/popup/popup.h \
        classes/menubar/menubar.h \
        classes/menubar/widgetmenu.h \
        classes/tabwidget/tabwidget.h \
        classes/calibrationwidget/calibrationwidget.h \
        classes/tableviews/purposetableview.h \
        classes/tabledevicemenu/devicemenumodel.h \
        classes/menubar/toolbutton.h \
        classes/user/userdialog.h \
        classes/interval/intervaldialog.h \
        classes/widget/widget.h \
        classes/terminal/terminalwindow.h \
    classes/widget/dockwidget.h

FORMS += \
        configuratorwindow.ui \
        qcell.ui \
        cterminal.ui \
        versionform.ui \
        serialportsetting.ui \
        statusbar.ui \
        eventlistjournals.ui \
        progressbarwidget.ui \
        filterdialog.ui \
        headerjournal.ui \
        journalwidget.ui \
        indicatorstate.ui \
        cindicatorcell.ui \
        monitorpurpose.ui \
        outputall.ui \
        outputallcell.ui \
        timefilterdialog.ui \
        debuginfo.ui \
        statusinfo.ui \
        classes/menubar/menubar.ui \
        classes/menubar/widgetmenu.ui \
        classes/calibrationwidget/calibrationwidget.ui \
        classes/user/userdialog.ui \
        classes/interval/intervaldialog.ui \
    classes/widget/dockwidget.ui

INCLUDEPATH += "classes/modbus" \
               "classes/popup" \
               "classes/menubar" \
               "classes/tabwidget" \
               "classes/calibrationwidget" \
               "classes/excel" \
               "classes/tableviews" \
               "classes/tabledevicemenu" \
               "classes/user" \
               "classes/interval" \
               "classes/widget" \
               "classes/terminal"

include(classes/excel/xlsx/qtxlsx.pri)

win32::RC_FILE = \
                 resource/icon.rc

RESOURCES += \
             resource.qrc
