#ifndef WS2812_h
#define WS2812_h

#include <Arduino.h>
#define DIRECTION_NONE 0
#define DIRECTION_CCW 1
#define DIRECTION_CW 2

#define NEO_GRB  ((1 << 6) | (1 << 4) | (0 << 2) | (2)) // 0x52

class WS2812 {
  public:
  /**
   * @brief Instantiate the WS2812 library.
   *
   * @param[in] pixels The number of pixels the string contains.
   */
  WS2812(uint16_t n = 1, uint8_t t = NEO_GRB);
  WS2812(void);
  ~WS2812();
  /**
   * @brief Start the WS2812 library.
   *
   * Initialize SPI and turn the entire strip off.
   */
  void begin();
  /**
   * @brief Send buffer to strip.
   *
   * Send the RGB buffer to the strip.
   * @param[in] buffer The RGB buffer.
   * @param[in] length The length of the buffer.
   */
  void sendBuffer(uint8_t (*ptr)[3], uint8_t len);
  /**
   * @brief TODO Description
   *
   * TODO Description
   * @param[in] TODO.
   */
  void show(void);
  void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
  void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
  void setPixelColor(uint16_t n, uint32_t c);
  void fill(uint32_t c=0, uint16_t first=0, uint16_t count=0);
  void setBrightness(uint8_t);
  void clear();
  void updateLength(uint16_t n);
  void updateType(uint8_t t);
  uint8_t *getPixels(void) const;
  uint8_t getBrightness(void) const;
  uint8_t sine8(uint8_t) const;
  uint8_t gamma8(uint8_t);
  int8_t getPin(void) { return pin; };
  uint16_t numPixels(void) const;
  static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
  static uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
  uint32_t getPixelColor(uint16_t n) const;
  inline bool canShow(void) { return (micros() - endTime) >= 300L; }
  uint32_t   gamma32(uint32_t x);
  uint32_t   ColorHSV(uint16_t hue, uint8_t sat=255, uint8_t val=255);
  /**
   * @brief Set entire strip to the RGB color.
   *
   * @param[in] red 8 bit red value.
   * @param[in] green 8 bit green value.
   * @param[in] blue 8 bit blue value.
   */
  void color(uint8_t r, uint8_t g, uint8_t b);
  /**
   * @brief Turn the strip off.
   */
  void off(){ color(0, 0, 0); };
  private:
    boolean begun;         // true if begin() previously called
    uint16_t numLEDs;      // Number of RGB LEDs in strip
    uint16_t numBytes;      // Size of 'pixels' buffer below (3 or 4 bytes/pixel)
    int8_t pin;           // Output pin number (-1 if not yet set)
    uint8_t brightness;
    uint8_t *pixels;        // Holds LED color values (3 or 4 bytes each)
    uint8_t rOffset;       // Index of red byte within each 3- or 4-byte pixel
    uint8_t gOffset;       // Index of green byte
    uint8_t bOffset;       // Index of blue byte
    uint8_t wOffset;       // Index of white byte (same as rOffset if no white)
    uint32_t endTime;       // Latch timing reference
    uint16_t _pixels;
};

#endif
