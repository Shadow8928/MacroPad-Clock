#pragma once
#include <Arduino.h>
#include "Timer.h"

class Display {
public:
  bool begin();  // returns false if the OLED doesn't ACK on the bus

  void render(int selectedTimerIndex, Timer timers[], int numTimers, bool muted);

private:
  bool _connected = false;
};
