# Interface Deploy folders
folder_qml.source = interface/qml
folder_qml.target = interface

folder_js.source = interface/js
folder_js.target = interface

folder_assets.source = interface/assets
folder_assets.target = interface

DEPLOYMENTFOLDERS = folder_qml folder_js folder_assets

INCLUDEPATH += ../../src/multimedia ../../src/multimedia/audio
CONFIG += release app_bundle
QT += quick qml xml multimedia sql network widgets

win32 {
    LIBS_DIR = $$_PRO_FILE_PWD_/lib/win32/
    PJSIP_DIR = $$_PRO_FILE_PWD_/lib/win32/pjsip
    PJSIP_TARGET = i386-Win32-vc8-Release.lib
    PJSIP_LIB_PJLIB_DIR = $$PJSIP_DIR/pjlib/lib
    PJSIP_LIB_PJLIBUTIL_DIR = $$PJSIP_DIR/pjlib-util/lib
    PJSIP_LIB_PJMEDIA_DIR = $$PJSIP_DIR/pjmedia/lib
    PJSIP_LIB_PJNATH_DIR = $$PJSIP_DIR/pjnath/lib
    PJSIP_LIB_PJSIP_DIR = $$PJSIP_DIR/pjsip/lib
    PJSIP_LIB_PJTHIRD_DIR = $$PJSIP_DIR/third_party/lib
}

mac {
    PJSIP_DIR = $$_PRO_FILE_PWD_/lib/mac/pjsip
    LIBS_DIR = $$_PRO_FILE_PWD_/lib/win32/
    PJSIP_TARGET = i386-apple-darwin12.4.0.a
    PJSIP_LIB_PJLIB_DIR = $$PJSIP_DIR/pjlib/lib
    PJSIP_LIB_PJLIBUTIL_DIR = $$PJSIP_DIR/pjlib-util/lib
    PJSIP_LIB_PJMEDIA_DIR = $$PJSIP_DIR/pjmedia/lib
    PJSIP_LIB_PJNATH_DIR = $$PJSIP_DIR/pjnath/lib
    PJSIP_LIB_PJSIP_DIR = $$PJSIP_DIR/pjsip/lib
    PJSIP_LIB_PJTHIRD_DIR = $$PJSIP_DIR/third_party/lib
    ICON = $$_PRO_FILE_PWD_/othos_icon.icns
    QMAKE_CXXFLAGS += -ObjC++
}

SOURCEDIR = ../src

DEFINES -= UNICODE

INCLUDEPATH +=  $$SOURCEDIR \
    $$PJSIP_DIR \
    $$PJSIP_DIR/pjsip/include \
    $$PJSIP_DIR/pjmedia/include \
    $$PJSIP_DIR/pjmedia/include/pjmedia-audiodev \
    $$PJSIP_DIR/pjmedia/include/pjmedia \
    $$PJSIP_DIR/pjmedia/include/pjmedia-codec \
    $$PJSIP_DIR/pjmedia/include/pjmedia-videodev \
    $$PJSIP_DIR/pjnath/include \
    $$PJSIP_DIR/pjlib/include \
    $$PJSIP_DIR/pjlib-util/include \
    $$PJSIP_DIR/third_party/openssl/include/openssl \
    $$LIBS_DIR/ffmpeg/include \
    $$LIBS_DIR/sdl/include \

win32: LIBS += -lwinmm \
    -lole32 \
    $$PJSIP_LIB_PJTHIRD_DIR/libbaseclasses-$$PJSIP_TARGET \
    -L$$LIBS_DIR \
    -lstrmbase \
    -loleaut32 \
    -lws2_32 \
    -lwsock32 \
    -lgdi32 \
    -lIphlpapi \
    -ldsound \
    -lnetapi32 \
    -lmswsock \
    -lws2_32 \
    -lodbc32 \
    -lodbccp32 \
    -lole32 \
    -luser32 \
    -lgdi32 \
    -ladvapi32 \
    -L$$LIBS_DIR/openssl/lib \
    -lssleay32MT \
    -llibeay32MT \
    -L$$LIBS_DIR/ffmpeg/lib \
    -lavutil \
    -lavcodec \
    -lavdevice \
    -lavfilter \
    -lavformat \
    -lswscale \
    -L$$LIBS_DIR/sdl/lib \
    -lsdl2 \


#PJSIP Libs
LIBS += $$PJSIP_LIB_PJSIP_DIR/libpjsua-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJSIP_DIR/libpjsip-ua-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJSIP_DIR/libpjsip-simple-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJSIP_DIR/libpjsip-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJMEDIA_DIR/libpjmedia-codec-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJMEDIA_DIR/libpjmedia-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJMEDIA_DIR/libpjmedia-codec-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJMEDIA_DIR/libpjmedia-audiodev-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJMEDIA_DIR/libpjmedia-videodev-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJNATH_DIR/libpjnath-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJLIBUTIL_DIR/libpjlib-util-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJLIB_DIR/libpj-$$PJSIP_TARGET \

#PJSIP Third Party
LIBS += $$PJSIP_LIB_PJTHIRD_DIR/libportaudio-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJTHIRD_DIR/libgsmcodec-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJTHIRD_DIR/libilbccodec-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJTHIRD_DIR/libspeex-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJTHIRD_DIR/libresample-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJTHIRD_DIR/libmilenage-$$PJSIP_TARGET \
        $$PJSIP_LIB_PJTHIRD_DIR/libsrtp-$$PJSIP_TARGET \

mac: LIBS += \
        -framework ForceFeedback \
        -framework IOkit \
        -framework Carbon \
        -framework VideoDecodeAcceleration \
        -framework VideoToolbox \
        /usr/lib/libz.dylib \
        /usr/lib/libiconv.dylib \
        /usr/lib/libbz2.dylib \
        $$PJSIP_LIB_PJTHIRD_DIR/libopus.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libssl.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libcrypto.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libSDL2main.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libSDL2.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libSDL2_test.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libavcodec.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libx264.125.dylib \
        $$PJSIP_LIB_PJTHIRD_DIR/libavdevice.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libavfilter.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libavformat.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libavresample.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libavutil.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libpostproc.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libswresample.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libswscale.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libmp3lame.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libx264.dylib \
        $$PJSIP_LIB_PJTHIRD_DIR/libxvidcore.a \
        $$PJSIP_LIB_PJTHIRD_DIR/liblzma.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libfaac.a \
        $$PJSIP_LIB_PJTHIRD_DIR/libyasm.a \

RESOURCES += \
    resources.qrc

OTHER_FILES += othos_icon.icns \
                client.crt \
    interface/qml/screens/audio.qml

HEADERS += \
    src/headers/controller.h \
    src/headers/telephony.h \
    src/headers/settingsmanager.h \
    src/headers/screensettings.h \
    src/headers/screenphone.h \
    src/headers/screenlog.h \
    src/headers/screencontacts.h \
    src/headers/screenaccounts.h \
    src/headers/dbtablemobel.h \
    src/headers/database.h \
    src/headers/videoviewer.h \
    src/headers/fileio.h

SOURCES += src/main.cpp \
    src/controller.cpp \
    src/telephony.cpp \
    src/settingsmanager.cpp \
    src/screensettings.cpp \
    src/screenphone.cpp \
    src/screenlog.cpp \
    src/screencontacts.cpp \
    src/screenaccounts.cpp \
    src/dbtablemobel.cpp \
    src/database.cpp \
    src/videoviewer.cpp \
    src/fileio.cpp

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()
