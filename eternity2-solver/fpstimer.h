#ifndef __FPSTIMER__
#define __FPSTIMER__

#include <chrono>
#include <iomanip>
#include <iostream>
#include <cmath>

class FPSTimer {

  unsigned int countIntervalMS = 1000000; //1s time interval between fps calculation
  unsigned int nbrFrames = 0;
  float FPS = 0;
  bool printFPS;


  std::chrono::high_resolution_clock::time_point timeStart;
  std::chrono::high_resolution_clock::time_point timeLastUpdate;

public:

  FPSTimer(bool printFPS = false) : printFPS(printFPS) {
    timeStart = timeLastUpdate = std::chrono::high_resolution_clock::now();
  }

  float update() {
    auto timeCurrent = std::chrono::high_resolution_clock::now();
    auto timeElapsedMS = std::chrono::duration_cast<std::chrono::microseconds>(timeCurrent - timeStart).count();

    nbrFrames += 1;
    if (countIntervalMS > 0 && timeElapsedMS >= countIntervalMS) {
        FPS = 1000000.f / ((float)timeElapsedMS / (float)nbrFrames);
        if (printFPS) {
        //std::cout << "FPS = " << std::fixed << std::setprecision(2) << 1000000 / ((float)timeElapsedMS / (float)nbrFrames) << std::endl;
        std::cout << "FPS = " << roundf(FPS) << std::endl;
        }
        nbrFrames = 0;
        timeStart = timeCurrent;
    }

    timeElapsedMS = std::chrono::duration_cast<std::chrono::microseconds>(timeCurrent - timeLastUpdate).count();
    timeLastUpdate = timeCurrent;
    return timeElapsedMS / 1000000.f;
  }

};

#endif
