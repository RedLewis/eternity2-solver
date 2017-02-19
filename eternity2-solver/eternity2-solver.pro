TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp

!win32 {
    # -march=native selects all the SSE/AVX instructions available on the processor.
    # For a Sandy Bridge processor, the flowing flags are selected:
    # -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 -mavx
    # QMAKE_CXXFLAGS += -march=native
    QMAKE_CXXFLAGS += -msse4
}
QMAKE_LIBS += -lpython2.7
SOURCES += main.cpp \
    board.cpp \
    population.cpp \
    e2tiles.cpp \
    tile.cpp
    tile.cpp

HEADERS += \
    aligned.h \
    board.h \
    population.h \
    e2tiles.h \
    tile.h \
    fpstimer.h \
    matplotlibcpp.h
    tile.h
