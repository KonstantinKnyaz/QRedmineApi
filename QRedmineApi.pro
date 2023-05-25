QT -= gui
QT += network

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    qredmineapi.cpp \
    redminerestapisender.cpp

HEADERS += \
    IssueStruct.h \
    UploadStruct.h \
    WikiStruct.h \
    qredmineapi.h \
    redminerestapisender.h

unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
