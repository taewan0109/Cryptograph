TEMPLATE = app

QT += qml quick sql
CONFIG += c++11

HEADERS += sqlcontactmodel.h \
    RSA.h \
    sqlconversationmodel.h

SOURCES += main.cpp \
    RSA.cpp \
    sqlcontactmodel.cpp \
    sqlconversationmodel.cpp

RESOURCES += \
    image/image.qrc \
    qml.qrc \
    image/image.qrc

target.path = D:/coding/Qt_Project/Chat_Client
INSTALLS += target


INSTALLER = Chat_Client
INPUT = $$PWD/config/config.xml $$PWD/packages
