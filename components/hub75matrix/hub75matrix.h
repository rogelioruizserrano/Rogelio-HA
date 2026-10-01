#include "esphome.h"
#include "esphome/components/display/display_buffer.h"
#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"

using namespace esphome;

#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 1

class HUB75MatrixDisplay : public PollingComponent, public display::DisplayBuffer {
 public:
  MatrixPanel_I2S_DMA *dma_display;

  HUB75MatrixDisplay() : PollingComponent(50), display::DisplayBuffer(PANEL_RES_X, PANEL_RES_Y) {}

  void setup() override {
    HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN);
    mxconfig.gpio.r1 = 2;
    mxconfig.gpio.g1 = 3;
    mxconfig.gpio.b1 = 4;
    mxconfig.gpio.r2 = 5;
    mxconfig.gpio.g2 = 6;
    mxconfig.gpio.b2 = 7;
    mxconfig.gpio.a = 8;
    mxconfig.gpio.b = 9;
    mxconfig.gpio.c = 10;
    mxconfig.gpio.d = 1;
    mxconfig.gpio.lat = 18;
    mxconfig.gpio.oe = 19;
    mxconfig.gpio.clk = 20;

    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->begin();
    dma_display->setBrightness8(50);
  }

  void update() override {
    dma_display->fillScreenRGB888(0, 0, 0);
    dma_display->setCursor(5, 10);
    dma_display->setTextColor(dma_display->color565(255, 255, 0));
    dma_display->print("¡Hola ESPHome!");
    dma_display->showDMABuffer();
  }

  void draw_absolute_pixel_internal(int x, int y, Color color) override {
    dma_display->drawPixel(x, y, dma_display->color565(color.r, color.g, color.b));
  }
};