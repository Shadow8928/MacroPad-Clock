#pragma once
#include <Arduino.h>

class Timer {
public:
  void begin(unsigned long defaultSeconds);

  // Call every loop() iteration; decrements remaining time once per
  // real second while running. Non-blocking (millis-based).
  void update();

  void start();
  void pause();
  void toggleRunning();
  void reset();

  // Adjust remaining time by deltaSeconds (positive or negative).
  // Clamped to [0, 99*60+59] seconds (99:59 display ceiling).
  void adjust(long deltaSeconds);

  bool isRunning() const   { return _running; }
  bool isFinished() const  { return _finished; }
  unsigned long remaining() const { return _remainingSeconds; }

private:
  unsigned long _remainingSeconds = 0;
  unsigned long _defaultSeconds   = 0;
  unsigned long _lastTickMs       = 0;
  bool _running  = false;
  bool _finished = false;
};
