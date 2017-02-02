TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

!win32 {
    # -march=native selects all the SSE/AVX instructions available on the processor.
    # For a Sandy Bridge processor, the flowing flags are selected:
    # -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 -mavx
    # QMAKE_CXXFLAGS += -march=native
    QMAKE_CXXFLAGS += -msse4
}

SOURCES += main.cpp \
    cell.cpp \
    board.cpp \
    e2squarepieces.cpp

HEADERS += \
    cell.h \
    aligned.h \
    board.h \
    e2squarepieces.h
