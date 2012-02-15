load(qt_module)

TARGET = QtPublishSubscribe
QPRO_PWD = $PWD

CONFIG += module
MODULE_PRI = ../../modules/qt_publishsubscribe.pri

QT = core

DEFINES += QT_BUILD_PUBLISHSUBSCRIBE_LIB QT_MAKEDLL

load(qt_module_config)

PUBLIC_HEADERS = qpublishsubscribeglobal.h \
                 qvaluespace.h \
                 qvaluespacepublisher.h \
                 qvaluespacesubscriber.h

PRIVATE_HEADERS = qvaluespace_p.h \
                  qvaluespacemanager_p.h \
                  qvaluespacesubscriber_p.h

SOURCES = qvaluespace.cpp \
          qvaluespacemanager.cpp \
          qvaluespacepublisher.cpp \
          qvaluespacesubscriber.cpp

unix {
    linux-* {
        contains(config_test_gconf, yes) {
            PRIVATE_HEADERS += gconfitem_p.h \
                               gconflayer_p.h

            SOURCES += gconflayer.cpp \
                       gconfitem.cpp

            CONFIG += link_pkgconfig
            PKGCONFIG += gobject-2.0 gconf-2.0
        } else {
            DEFINES += QT_NO_GCONFLAYER
        }

        contains(QT_CONFIG,jsondbcompat) {
            QT += jsondbcompat jsondbcompat-private
            PRIVATE_HEADERS += jsondblayer_p.h
            SOURCES += jsondblayer.cpp
        } else {
            DEFINES += QT_NO_JSONDBLAYER
        }
    }
}

win32: {
    PRIVATE_HEADERS += qsystemreadwritelock_p.h \
                       registrylayer_win_p.h
    SOURCES += qsystemreadwritelock_win.cpp \
               registrylayer_win.cpp

    LIBS += -ladvapi32
}

HEADERS = qtpublishsubscribeversion.h $$PUBLIC_HEADERS $$PRIVATE_HEADERS
