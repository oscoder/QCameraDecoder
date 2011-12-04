TEMPLATE = app
TARGET = QCameraDecoder

SOURCES += \
    main.cpp \
    button.cpp \
    camerawidget.cpp \
    cameravideosurface.cpp

HEADERS += \
    button.h \
    camerawidget.h \
    cameravideosurface.h \
    videoif.h

RESOURCES += \
    res.qrc
ICON = icons/cameramms_icon.svg

symbian: {
    # Because landscape orientation lock
    LIBS += -lcone -leikcore -lavkon
    TARGET.UID3 = 0xEF642F0E
    TARGET = QCameraDecoder
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x20000 0x8000000

    # Self-signing capabilities
    TARGET.CAPABILITY += NetworkServices \
        ReadUserData \
        WriteUserData \
        LocalServices \
        UserEnvironment

    # QtMobility Messaging module needs these
#    DEFINES += MESSAGING_ENABLED

    contains(DEFINES,MESSAGING_ENABLED) {
        # Additional capabilities that needs Open Signed Online signing
        TARGET.CAPABILITY += ReadDeviceData WriteDeviceData
    }
}

!contains(DEFINES,MESSAGING_ENABLED) {
    warning(Qt Mobility Messaging disabled!)
}

QT += gui core

CONFIG += mobility
MOBILITY = multimedia \
    systeminfo







