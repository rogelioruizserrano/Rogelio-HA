#ifndef _ESP32_HUB75_MATRIXPANEL_I2S_DMA_H
#define _ESP32_HUB75_MATRIXPANEL_I2S_DMA_H

#include <Arduino.h>

class MatrixPanel_I2S_DMA {
 public:
  MatrixPanel_I2S_DMA(int dummy) {}
  MatrixPanel_I2S_DMA(struct HUB75_I2S_CFG cfg) {}
  void begin() {}
  void setBrightness8(uint8_t) {}
  void fillScreenRGB888(uint8_t, uint8_t, uint8_t) {}
  void drawPixel(int, int, uint16_t) {}
  void setCursor(int, int) {}
  void setTextColor(uint16_t) {}
  void print(const char*) {}
  void showDMABuffer() {}
  uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { return (r << 11) | (g << 5) | b; }
};

typedef struct {
  struct {
    int r1, g1, b1, r2, g2, b2, a, b, c, d, lat, oe, clk;
  } gpio;
} HUB75_I2S_CFG;

#endif