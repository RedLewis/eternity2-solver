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

  const std::chrono::high_resolution_clock::time_point timeStart;
  std::chrono::high_resolution_clock::time_point timeLastFPSCount;
  std::chrono::high_resolution_clock::time_point timeLastUpdate;

public:

  FPSTimer() : timeStart(std::chrono::high_resolution_clock::now()) {
    timeLastFPSCount = timeLastUpdate = timeStart;
  }

  float update(bool printFPS = false) {
    auto timeCurrent = std::chrono::high_resolution_clock::now();
    auto timeElapsedMS = std::chrono::duration_cast<std::chrono::microseconds>(timeCurrent - timeLastFPSCount).count();

    nbrFrames += 1;
    if (countIntervalMS > 0 && timeElapsedMS >= countIntervalMS) {
        FPS = 1000000.f / ((float)timeElapsedMS / (float)nbrFrames);
        if (printFPS) {
        //std::cout << "FPS = " << std::fixed << std::setprecision(2) << 1000000 / ((float)timeElapsedMS / (float)nbrFrames) << std::endl;
        std::cout << "FPS = " << roundf(FPS) << std::endl;
        }
        nbrFrames = 0;
        timeLastFPSCount = timeCurrent;
    }

    timeElapsedMS = std::chrono::duration_cast<std::chrono::microseconds>(timeCurrent - timeLastUpdate).count();
    timeLastUpdate = timeCurrent;
    return timeElapsedMS / 1000000.f;
  }

  float getTimeSinceLastUpdate() const {
      auto timeCurrent = std::chrono::high_resolution_clock::now();
      auto timeElapsedMS = std::chrono::duration_cast<std::chrono::microseconds>(timeCurrent - timeLastUpdate).count();
      return timeElapsedMS / 1000000.f;
  }

  float getTimeSinceStart() const {
      auto timeCurrent = std::chrono::high_resolution_clock::now();
      auto timeElapsedMS = std::chrono::duration_cast<std::chrono::microseconds>(timeCurrent - timeStart).count();
      return timeElapsedMS / 1000000.f;
  }

};

#endif
