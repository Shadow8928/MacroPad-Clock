#include "Timer.h"

static const unsigned long MAX_SECONDS = 99UL * 60UL + 59UL;

void Timer::begin(unsigned long defaultSeconds) {
  _defaultSeconds   = defaultSeconds;
  _remainingSeconds = defaultSeconds;
  _running  = false;
  _finished = false;
  _lastTickMs = millis();
}

void Timer::update() {
  if (!_running || _finished) return;

  unsigned long now = millis();
  if (now - _lastTickMs >= 1000UL) {
    // Handle possible multi-second gaps (e.g. after a long ISR/display draw)
    unsigned long elapsedSec = (now - _lastTickMs) / 1000UL;
    _lastTickMs += elapsedSec * 1000UL;

    if (elapsedSec >= _remainingSeconds) {
      _remainingSeconds = 0;
      _running  = false;
      _finished = true;
    } else {
      _remainingSeconds -= elapsedSec;
    }
  }
}

void Timer::start() {
  if (_finished) return;      // must reset before restarting a finished timer
  _running = true;
  _lastTickMs = millis();
}

void Timer::pause() {
  _running = false;
}

void Timer::toggleRunning() {
  if (_running) pause();
  else start();
}

void Timer::reset() {
  _remainingSeconds = _defaultSeconds;
  _running  = false;
  _finished = false;
  _lastTickMs = millis();
}

void Timer::adjust(long deltaSeconds) {
  long newVal = (long)_remainingSeconds + deltaSeconds;
  if (newVal < 0) newVal = 0;
  if ((unsigned long)newVal > MAX_SECONDS) newVal = MAX_SECONDS;
  _remainingSeconds = (unsigned long)newVal;

  // Adjusting a finished timer clears the finished/alarm state so the
  // user can re-arm it without a full reset.
  if (_finished && _remainingSeconds > 0) {
    _finished = false;
  }
}
