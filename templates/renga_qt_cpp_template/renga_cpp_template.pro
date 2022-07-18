QT -= gui

TEMPLATE = lib
DEFINES += RENGA_CPP_TEMPLATE_LIBRARY

CONFIG += c++11
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

SOURCES += \
    renga_cpp_template.cpp

HEADERS += \
    rengacomapi.tlh \
    renga_cpp_template_global.h \
    renga_cpp_template.hpp
DISTFILES += \
    Renga_cpp_template.rndesc \
    logo.png \
    rengacomapi.tlh

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
