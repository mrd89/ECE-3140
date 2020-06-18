#ifndef RGB_MATRIX_PANEL_H
#define RGB_MATRIX_PANEL_H

#include "DigitalOut.h"
#include "BusOut.h"
#include "Adafruit_GFX.h"
 
#ifndef _BV
#define _BV(bit) (1<<(bit))
#endif
 
#ifdef DEBUG
#define log_debug(format,...)   std::printf(format,##__VA_ARGS__)
#else
#define log_debug(...)
#endif
 
class RGBmatrixPanel : public Adafruit_GFX
{
 
public:
    // Constructor for 32x32 panel (adds 'd' pin):
    RGBmatrixPanel(DigitalOut& r1,DigitalOut& r2,DigitalOut& g1,DigitalOut& g2,DigitalOut& b1,DigitalOut& b2,DigitalOut& a,DigitalOut& b,DigitalOut& c,DigitalOut& d,DigitalOut& sclk, DigitalOut& latch,DigitalOut& oe);
 
    /** @fn void RGBmatrixPanel::begin(void)
    *   @bref Attach a updateDisplay() to be called by the Ticker(every 100us)
    */
    void begin(void);
    /** @fn void RGBmatrixPanel::drawPixel(int16_t x, int16_t y, uint16_t c)
    *   @bref drawPixel
    */
    virtual void drawPixel(int16_t x,int16_t y,uint16_t c);
    /** @fn void RGBmatrixPanel::fillScreen(uint16_t c)
    *   @bref fillScreen
    *   @param  c   fill screen 16bit color 0x0000 ~ 0xFFFF
    */
    virtual void fillScreen(uint16_t c);
    /** @fn void RGBmatrixPanel::updateDisplay(void)
    *   @param c    updateDisplay\n
    *               This method is called by the interrupt start at begin().
    */
    void updateDisplay(void);
    /** @fn void RGBmatrixPanel::swapBuffers(bool copy)
    *   @param  copy swap buffer (if you use double-buffer)
    */
    void swapBuffers(bool copy);
    /** @fn void RGBmatrixPanel::dumpMatrix(void)
    *   @bref dump to default USB Serial\n
    *         Declaration is required to use.(#define DEBUG)
    */
    void dumpMatrix(void);
 
    uint8_t *backBuffer(void);
 
    /** @fn uint16_t RGBmatrixPanel::Color333(uint8_t r, uint8_t g, uint8_t b)
    *   @bref up convert to 16bit color from 9bit color.
    *   @return 16bit(uint16_t) color value
    */
    uint16_t Color333(uint8_t r, uint8_t g, uint8_t b);
    /** @fn uint16_t RGBmatrixPanel::Color444(uint8_t r, uint8_t g, uint8_t b)
    *   @bref up convert to 16bit color from 12bit color.
    *   @param r 0~7
    *   @param g 0~7
    *   @param b 0~7
    *   @return 16bit(uint16_t) color value
    */
    uint16_t Color444(uint8_t r, uint8_t g, uint8_t b);
    /** @fn uint16_t RGBmatrixPanel::Color888(uint8_t r, uint8_t g, uint8_t b)
    *   @bref down convert to 16bit color from 24bit color.
    *   @return 16bit(uint16_t) color value
    */
    uint16_t Color888(uint8_t r, uint8_t g, uint8_t b);
    /** @fn uint16_t RGBmatrixPanel::Color888(uint8_t r, uint8_t g, uint8_t b, bool gflag)
    *   @bref down convert to 16bit color from 24bit color using the gamma value table.
    *   @return 16bit(uint16_t) color value
    */
    uint16_t Color888(uint8_t r, uint8_t g, uint8_t b, bool gflag);
    /** @fn uint16_t RGBmatrixPanel::ColorHSV(long hue, uint8_t sat, uint8_t val, bool gflag)
    *   @bref convert to 16bit color from (unsigned integer)HSV color using the gamma value table.
    *   @param hue 0~1536(decimal value)
    *   @param sat 0~255(decimal value) Does not make sense that it is not a multiple of 32.
    *   @param val 0~255(decimal value) Does not make sense that it is not a multiple of 32.
    *   @return 16bit(uint16_t) color value
    */
    uint16_t ColorHSV(long hue, uint8_t sat, uint8_t val, bool gflag)
    ;
    /** @fn uint16_t RGBmatrixPanel::ColorHSV(float hue, float sat, float val, bool gflag)
    *   @bref convert to 16bit color from (float)HSV color using the gamma value table.
    *   @param hue Normalized value from 0.0 to 1.0
    *   @param sat Normalized value from 0.0 to 1.0
    *   @param val Normalized value from 0.0 to 1.0
    *   @return 16bit(uint16_t) color value
    */
    uint16_t ColorHSV(float hue, float sat, float val, bool gflag);
private:
    uint8_t *matrixbuff[2];
    uint8_t nRows;
    uint8_t backindex;
    bool swapflag;
 
    // Init/alloc code common to both constructors:
    void init(uint8_t rows, bool dbuf);
 
    BusOut _dataBus;
    BusOut _rowBus;
    DigitalOut _d,_sclk, _latch, _oe;

    // Counters/pointers for interrupt handler:
    uint8_t row, plane;
    uint8_t *buffptr;
};

#endif
