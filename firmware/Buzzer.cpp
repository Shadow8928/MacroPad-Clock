#include "Buzzer.h"
#include "Config.h"

void Buzzer::begin(uint8_t pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  _on = false;
  _lastToggleMs = millis();
}

void Buzzer::update(bool alarmActive, bool muted) {
  if (!alarmActive || muted) {
    if (_on) {
      digitalWrite(_pin, LOW);
      _on = false;
    }
    _lastToggleMs = millis();
    return;
  }

  unsigned long now = millis();
  unsigned long interval = _on ? BUZZER_ON_MS : BUZZER_OFF_MS;
  if (now - _lastToggleMs >= interval) {
    _on = !_on;
    digitalWrite(_pin, _on ? HIGH : LOW);
    _lastToggleMs = now;
  }
}
