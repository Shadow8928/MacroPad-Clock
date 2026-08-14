#include "Display.h"
#include "Config.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

static Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

bool Display::begin() {
  _connected = oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR);
  if (_connected) {
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);
    oled.display();
  }
  return _connected;
}

static void formatTime(unsigned long totalSeconds, char* buf, size_t bufLen) {
  unsigned long mins = totalSeconds / 60;
  unsigned long secs = totalSeconds % 60;
  snprintf(buf, bufLen, "%02lu:%02lu", mins, secs);
}

void Display::render(int selectedTimerIndex, Timer timers[], int numTimers, bool muted) {
  if (!_connected) return;

  Timer& t = timers[selectedTimerIndex];
  char timeBuf[8];
  formatTime(t.remaining(), timeBuf, sizeof(timeBuf));

  oled.clearDisplay();

  // Top row: which timer is selected, run state, mute state
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.print("Timer ");
  oled.print(selectedTimerIndex + 1);
  oled.print(t.isFinished() ? "  DONE" : (t.isRunning() ? "  RUN" : "  PAUSED"));
  if (muted) {
    oled.setCursor(OLED_WIDTH - 12, 0);
    oled.print("M");
  }

  // Big time readout
  oled.setTextSize(3);
  oled.setCursor(10, 10);
  oled.print(timeBuf);

  // Small dots along the bottom showing which of the 6 timers
  // are currently running, so background timers stay visible.
  int dotSpacing = OLED_WIDTH / numTimers;
  for (int i = 0; i < numTimers; i++) {
    int x = i * dotSpacing + dotSpacing / 2;
    int y = OLED_HEIGHT - 3;
    if (timers[i].isFinished()) {
      oled.fillRect(x - 2, y - 2, 4, 4, SSD1306_WHITE);
    } else if (timers[i].isRunning()) {
      oled.drawRect(x - 2, y - 2, 4, 4, SSD1306_WHITE);
    }
  }

  oled.display();
}
