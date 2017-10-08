TEMPLATE = app
TARGET = Babel

QT = core gui
QT += network multimedia

INCLUDEPATH += include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DISTFILES += \
    assets/babel_icon.rc

RC_FILE = assets/babel_icon.rc

HEADERS += \
    include/IpWidget.h \
    include/MainWindow.h \
    include/TableModel.h \
    include/CmdHandler.h \
    include/client.h \
    include/audio.h \
    include/clientudp.h \
    include/StartupDialog.h

SOURCES += \
    srcs/IpWidget.cpp \
    srcs/MainWindow.cpp \
    srcs/TableModel.cpp \
    srcs/main.cpp \
    srcs/CmdHandler.cpp \
    srcs/client.cpp \
    srcs/audio.cpp \
    srcs/clientudp.cpp \
    srcs/StartupDialog.cpp

FORMS += \
    assets/audio.ui

