QT -= gui

#TARGET = QuiLib
TEMPLATE = lib
DEFINES += QT6_CPP_LIBRARY

CONFIG += c++14

#Вывод в консоль лога в верной кодировке
#https://blog.mgsxx.com/?p=2979
QMAKE_EXTRA_TARGETS += before_build makefilehook

makefilehook.target = $(MAKEFILE)
makefilehook.depends = .beforebuild

PRE_TARGETDEPS += .beforebuild

before_build.target = .beforebuild
before_build.depends = FORCE
before_build.commands = chcp 1251

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += \
    $$(RENGA_SDK)/C++/Include \
    $$(RENGA_SDK)/tlb
HEADERS += \
        rengacomapi.tlh \ #Подключаем com-библиотеку Renga (заголовочный файл)
        qt6_cpp_global.h \
        init_app.h

DISTFILES += \
    /rengacomapi.tli \ #Подключаем com-библиотеку Renga (реализующий файл)
    hello_renga_qt6.rndesc
SOURCES += \
    dllmain.cpp \
    init_app.cpp

TRANSLATIONS += \
    qt6_cpp_ru_RU.ts

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
