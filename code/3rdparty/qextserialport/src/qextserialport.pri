INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

PUBLIC_HEADERS         += $$PWD/qextserialport.h \
                          $$PWD/qextserialenumerator.h \
                          $$PWD/qextserialport_global.h

HEADERS                += $$PUBLIC_HEADERS \
                          $$PWD/qextserialport_p.h \
                          $$PWD/qextserialenumerator_p.h \

SOURCES                += $$PWD/qextserialport.cpp \
                          $$PWD/qextserialenumerator.cpp \
    $$PWD/qextserialport_unix.cpp
unix {
    SOURCES            +=
    linux* {
        SOURCES        += $$PWD/qextserialenumerator_linux.cpp
    } else:macx {
        SOURCES        +=
    } else {
        SOURCES        += $$PWD/qextserialenumerator_unix.cpp
    }
}
win32:SOURCES          +=

linux*{
    !qesp_linux_udev:DEFINES += QESP_NO_UDEV
    qesp_linux_udev: LIBS += -ludev
}

macx:LIBS              += -framework IOKit -framework CoreFoundation
win32:LIBS             += -lsetupapi -ladvapi32 -luser32

# moc doesn't detect Q_OS_LINUX correctly, so add this to make it work
linux*:DEFINES += __linux__
