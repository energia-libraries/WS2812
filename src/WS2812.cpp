#include "WS2812.h"
#include <SPI.h>

#if defined(__MSP430G2553)
#define SPIDIV     SPI_CLOCK_DIV2       // 16 MHz/2 gives 125 ns for each on bit in byte
#define SPILONG    0b11111100           // 750 ns (acceptable "on" range 550 to 850 ns)
#define SPISHORT   0b11100000           // 375 ns (acceptable "on" range 200 to 500 ns)
#elif defined(__MSP430F5529)
#define SPIDIV     SPI_CLOCK_DIV4       // 25.6 MHz/4 gives 156.25 ns for each on bit in byte
#define SPILONG    0b11110000           // 625 ns (acceptable "on" range 550 to 850 ns)
#define SPISHORT   0b11000000           // 312.5 ns (acceptable "on" range 200 to 500 ns)
#elif defined(__MSP432P401R__)
#define SPIDIV     3                    // 16 MHz/2 gives 125 ns for each on bit in byte
#define SPILONG    0b00001111           // 750 ns (acceptable "on" range 550 to 850 ns)
#define SPISHORT   0b00000011           // 375 ns (acceptable "on" range 200 to 500 ns)
#elif defined(BOARD_CC1310_LAUNCHXL)
#define SPIDIV     3                    // 16 MHz/2 gives 125 ns for each on bit in byte
#define SPILONG    0b00001111           // 750 ns (acceptable "on" range 550 to 850 ns)
#define SPISHORT   0b00000011           // 375 ns (acceptable "on" range 200 to 500 ns)
#elif defined(ENERGIA_EK_TM4C123GXL)
#define SPIDIV     8                    // 80 MHz/4 gives 125 ns for each on bit in byte
#define SPILONG    0b00111111           // 750 ns (acceptable "on" range 550 to 850 ns)
#define SPISHORT   0b00000111           // 375 ns (acceptable "on" range 200 to 500 ns)
#elif defined(ENERGIA_EK_TM4C1294XL)
#define SPIDIV     12                   // 80 MHz/4 gives 125 ns for each on bit in byte
#define SPILONG    0b00111111           // 750 ns (acceptable "on" range 550 to 850 ns)
#define SPISHORT   0b00000111           // 375 ns (acceptable "on" range 200 to 500 ns)
#elif defined(ENERGIA_ARCH_CC3200)
#define SPIDIV     SPI_CLOCK_DIV2       // 80 MHz/4 gives 125 ns for each on bit in byte
#define SPILONG    0b11111100           // 750 ns (acceptable "on" range 550 to 850 ns)
#define SPISHORT   0b11100000           // 375 ns (acceptable "on" range 200 to 500 ns)
#else
#error This microcontroller is not supported
#endif

WS2812::WS2812(uint16_t n, uint8_t t)
{
  updateType(t);
  updateLength(n);
}

WS2812::~WS2812() {
  if(pixels)   free(pixels);
}

void WS2812::begin()
{
  uint16_t i;

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPIDIV);

  delayMicroseconds(50);
  begun = true;
}

void WS2812::updateLength(uint16_t n) {
  if(pixels) free(pixels); // Free existing data (if any)

  // Allocate new data -- note: ALL PIXELS ARE CLEARED
  numBytes = n * ((wOffset == rOffset) ? 3 : 4);
  if((pixels = (uint8_t *)malloc(numBytes))) {
    memset(pixels, 0, numBytes);
    numLEDs = n;
  } else {
    numLEDs = numBytes = 0;
  }
}

void WS2812::updateType(uint8_t t) {
  boolean oldThreeBytesPerPixel = (wOffset == rOffset); // false if RGBW

  wOffset = (t >> 6) & 0b11; // See notes in header file
  rOffset = (t >> 4) & 0b11; // regarding R/G/B/W offsets
  gOffset = (t >> 2) & 0b11;
  bOffset =  t       & 0b11;

  // If bytes-per-pixel has changed (and pixel data was previously
  // allocated), re-allocate to new size.  Will clear any data.
  if(pixels) {
    boolean newThreeBytesPerPixel = (wOffset == rOffset);
    if(newThreeBytesPerPixel != oldThreeBytesPerPixel) updateLength(numLEDs);
  }
}

void WS2812::show(void) {
  uint16_t i, j=0;
  uint8_t *p, bit;
#if defined(__MSP430__)
  uint8_t buf[8];
#else
  uint8_t buf[8 * numBytes];
#endif
  if(!pixels) return;

  // Data latch = 300+ microsecond pause in the output stream.  Rather than
  // put a delay at the end of the function, the ending time is noted and
  // the function will simply hold off (if needed) on issuing the
  // subsequent round of data until the latch time has elapsed.  This
  // allows the mainline code to start generating the next frame of data
  // rather than stalling for the latch.
  while(!canShow());

  p = &pixels[0];

  for(i=0; i < numBytes; i++) {
    uint8_t pix = *p++;
    for (bit = 0; bit < 8; bit++) {
      if (pix & (0x80 >> bit)) {
        buf[bit + (j * 8)] = SPILONG;
      } else {
        buf[bit + (j * 8)] = SPISHORT;
      }
    }
#if defined(__MSP430__)
    SPI.transmit(buf, 8);
#else
      j++;
#endif
  }
#if !defined(__MSP430__)
  SPI.transfer(buf, 8 * numBytes);
#endif
}

uint16_t WS2812::numPixels(void) const {
  return numLEDs;
}

uint8_t *WS2812::getPixels(void) const {
  return pixels;
}

uint8_t WS2812::getBrightness(void) const {
  return brightness - 1;
}

void WS2812::clear() {
  memset(pixels, 0, numBytes);
}
/* Table containing 8-bit unsigned sine wave (0-255).
   Copy & paste this snippet into a Python REPL to regenerate:
import math
for x in range(256):
    print("{:3},".format(int((math.sin(x/128.0*math.pi)+1.0)*127.5+0.5))),
    if x&15 == 15: print
*/
static const uint8_t _sineTable[256] = {
  128,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,
  176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,
  218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,
  245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,
  255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,
  245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,
  218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,
  176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,
  128,124,121,118,115,112,109,106,103,100, 97, 93, 90, 88, 85, 82,
   79, 76, 73, 70, 67, 65, 62, 59, 57, 54, 52, 49, 47, 44, 42, 40,
   37, 35, 33, 31, 29, 27, 25, 23, 21, 20, 18, 17, 15, 14, 12, 11,
   10,  9,  7,  6,  5,  5,  4,  3,  2,  2,  1,  1,  1,  0,  0,  0,
    0,  0,  0,  0,  1,  1,  1,  2,  2,  3,  4,  5,  5,  6,  7,  9,
   10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35,
   37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76,
   79, 82, 85, 88, 90, 93, 97,100,103,106,109,112,115,118,121,124};

/* Similar to above, but for an 8-bit gamma-correction table.
   Copy & paste this snippet into a Python REPL to regenerate:
import math
gamma=2.6
for x in range(256):
    print("{:3},".format(int(math.pow((x)/255.0,gamma)*255.0+0.5))),
    if x&15 == 15: print
*/
static const uint8_t _gammaTable[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
    3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  7,
    7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12,
   13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20,
   20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29,
   30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42,
   42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
   58, 59, 60, 61, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 75,
   76, 77, 78, 80, 81, 82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96,
   97, 99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,
  122,124,125,127,129,130,132,134,136,137,139,141,143,145,146,148,
  150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,
  182,184,186,188,191,193,195,197,199,202,204,206,209,211,213,215,
  218,220,223,225,227,230,232,235,237,240,242,245,247,250,252,255};

uint8_t WS2812::sine8(uint8_t x) const {
  return pgm_read_byte(&_sineTable[x]); // 0-255 in, 0-255 out
}

uint8_t WS2812::gamma8(uint8_t x) {
  return pgm_read_byte(&_gammaTable[x]); // 0-255 in, 0-255 out
}

// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.
uint32_t WS2812::Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

// Convert separate R,G,B,W into packed 32-bit WRGB color.
// Packed format is always WRGB, regardless of LED strand color order.
uint32_t WS2812::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

uint32_t WS2812::getPixelColor(uint16_t n) const {
  if(n >= numLEDs) return 0; // Out of bounds, return no color.

  uint8_t *p;

  if(wOffset == rOffset) { // Is RGB-type device
    p = &pixels[n * 3];
    if(brightness) {
      // Stored color was decimated by setBrightness().  Returned value
      // attempts to scale back to an approximation of the original 24-bit
      // value used when setting the pixel color, but there will always be
      // some error -- those bits are simply gone.  Issue is most
      // pronounced at low brightness levels.
      return (((uint32_t)(p[rOffset] << 8) / brightness) << 16) |
             (((uint32_t)(p[gOffset] << 8) / brightness) <<  8) |
             ( (uint32_t)(p[bOffset] << 8) / brightness       );
    } else {
      // No brightness adjustment has been made -- return 'raw' color
      return ((uint32_t)p[rOffset] << 16) |
             ((uint32_t)p[gOffset] <<  8) |
              (uint32_t)p[bOffset];
    }
  } else {                 // Is RGBW-type device
    p = &pixels[n * 4];
    if(brightness) { // Return scaled color
      return (((uint32_t)(p[wOffset] << 8) / brightness) << 24) |
             (((uint32_t)(p[rOffset] << 8) / brightness) << 16) |
             (((uint32_t)(p[gOffset] << 8) / brightness) <<  8) |
             ( (uint32_t)(p[bOffset] << 8) / brightness       );
    } else { // Return raw color
      return ((uint32_t)p[wOffset] << 24) |
             ((uint32_t)p[rOffset] << 16) |
             ((uint32_t)p[gOffset] <<  8) |
              (uint32_t)p[bOffset];
    }
  }
}

// A 32-bit variant of gamma8() that applies the same function
// to all components of a packed RGB or WRGB value.
uint32_t WS2812::gamma32(uint32_t x) {
  uint8_t *y = (uint8_t *)&x;
  // All four bytes of a 32-bit value are filtered even if RGB (not WRGB),
  // to avoid a bunch of shifting and masking that would be necessary for
  // properly handling different endianisms (and each byte is a fairly
  // trivial operation, so it might not even be wasting cycles vs a check
  // and branch for the RGB case). In theory this might cause trouble *if*
  // someone's storing information in the unused most significant byte
  // of an RGB value, but this seems exceedingly rare and if it's
  // encountered in reality they can mask values going in or coming out.
  for(uint8_t i=0; i<4; i++) y[i] = gamma8(y[i]);
  return x; // Packed 32-bit return
}

uint32_t WS2812::ColorHSV(uint16_t hue, uint8_t sat, uint8_t val) {

  uint8_t r, g, b;

  // Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
  // 0 is not the start of pure red, but the midpoint...a few values above
  // zero and a few below 65536 all yield pure red (similarly, 32768 is the
  // midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
  // each for red, green, blue) really only allows for 1530 distinct hues
  // (not 1536, more on that below), but the full unsigned 16-bit type was
  // chosen for hue so that one's code can easily handle a contiguous color
  // wheel by allowing hue to roll over in either direction.
  hue = (hue * 1530L + 32768) / 65536;
  // Because red is centered on the rollover point (the +32768 above,
  // essentially a fixed-point +0.5), the above actually yields 0 to 1530,
  // where 0 and 1530 would yield the same thing. Rather than apply a
  // costly modulo operator, 1530 is handled as a special case below.

  // So you'd think that the color "hexcone" (the thing that ramps from
  // pure red, to pure yellow, to pure green and so forth back to red,
  // yielding six slices), and with each color component having 256
  // possible values (0-255), might have 1536 possible items (6*256),
  // but in reality there's 1530. This is because the last element in
  // each 256-element slice is equal to the first element of the next
  // slice, and keeping those in there this would create small
  // discontinuities in the color wheel. So the last element of each
  // slice is dropped...we regard only elements 0-254, with item 255
  // being picked up as element 0 of the next slice. Like this:
  // Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
  // Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
  // Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
  // and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
  // the constants below are not the multiples of 256 you might expect.

  // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
  if(hue < 510) {         // Red to Green-1
    b = 0;
    if(hue < 255) {       //   Red to Yellow-1
      r = 255;
      g = hue;            //     g = 0 to 254
    } else {              //   Yellow to Green-1
      r = 510 - hue;      //     r = 255 to 1
      g = 255;
    }
  } else if(hue < 1020) { // Green to Blue-1
    r = 0;
    if(hue <  765) {      //   Green to Cyan-1
      g = 255;
      b = hue - 510;      //     b = 0 to 254
    } else {              //   Cyan to Blue-1
      g = 1020 - hue;     //     g = 255 to 1
      b = 255;
    }
  } else if(hue < 1530) { // Blue to Red-1
    g = 0;
    if(hue < 1275) {      //   Blue to Magenta-1
      r = hue - 1020;     //     r = 0 to 254
      b = 255;
    } else {              //   Magenta to Red-1
      r = 255;
      b = 1530 - hue;     //     b = 255 to 1
    }
  } else {                // Last 0.5 Red (quicker than % operator)
    r = 255;
    g = b = 0;
  }

  // Apply saturation and value to R,G,B, pack into 32-bit result:
  uint32_t v1 =   1 + val; // 1 to 256; allows >>8 instead of /255
  uint16_t s1 =   1 + sat; // 1 to 256; same reason
  uint8_t  s2 = 255 - sat; // 255 to 0
  return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
          (((((g * s1) >> 8) + s2) * v1) & 0xff00)       |
         ( ((((b * s1) >> 8) + s2) * v1)           >> 8);
}

void WS2812::fill(uint32_t c, uint16_t first, uint16_t count) {
  uint16_t i, end;

  if(first >= numLEDs) {
    return; // If first LED is past end of strip, nothing to do
  }

  // Calculate the index ONE AFTER the last pixel to fill
  if(count == 0) {
    // Fill to end of strip
    end = numLEDs;
  } else {
    // Ensure that the loop won't go past the last pixel
    end = first + count;
    if(end > numLEDs) end = numLEDs;
  }

  for(i = first; i < end; i++) {
    this->setPixelColor(i, c);
  }
}

// Adjust output brightness; 0=darkest (off), 255=brightest.  This does
// NOT immediately affect what's currently displayed on the LEDs.  The
// next call to show() will refresh the LEDs at this level.  However,
// this process is potentially "lossy," especially when increasing
// brightness.  The tight timing in the WS2811/WS2812 code means there
// aren't enough free cycles to perform this scaling on the fly as data
// is issued.  So we make a pass through the existing color data in RAM
// and scale it (subsequent graphics commands also work at this
// brightness level).  If there's a significant step up in brightness,
// the limited number of steps (quantization) in the old data will be
// quite visible in the re-scaled version.  For a non-destructive
// change, you'll need to re-render the full strip data.  C'est la vie.
void WS2812::setBrightness(uint8_t b) {
  // Stored brightness value is different than what's passed.
  // This simplifies the actual scaling math later, allowing a fast
  // 8x8-bit multiply and taking the MSB.  'brightness' is a uint8_t,
  // adding 1 here may (intentionally) roll over...so 0 = max brightness
  // (color values are interpreted literally; no scaling), 1 = min
  // brightness (off), 255 = just below max brightness.
  uint8_t newBrightness = b + 1;
  if(newBrightness != brightness) { // Compare against prior value
    // Brightness has changed -- re-scale existing data in RAM
    uint8_t  c,
            *ptr           = pixels,
             oldBrightness = brightness - 1; // De-wrap old brightness value
    uint16_t scale;
    if(oldBrightness == 0) scale = 0; // Avoid /0
    else if(b == 255) scale = 65535 / oldBrightness;
    else scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;
    for(uint16_t i=0; i<numBytes; i++) {
      c      = *ptr;
      *ptr++ = (c * scale) >> 8;
    }
    brightness = newBrightness;
  }
}

void WS2812::setPixelColor(
 uint16_t n, uint8_t r, uint8_t g, uint8_t b) {

  if(n < numLEDs) {
    if(brightness) { // See notes in setBrightness()
      r = (r * brightness) >> 8;
      g = (g * brightness) >> 8;
      b = (b * brightness) >> 8;
    }
    uint8_t *p;
    if(wOffset == rOffset) { // Is an RGB-type strip
      p = &pixels[n * 3];    // 3 bytes per pixel
    } else {                 // Is a WRGB-type strip
      p = &pixels[n * 4];    // 4 bytes per pixel
      p[wOffset] = 0;        // But only R,G,B passed -- set W to 0
    }
    p[rOffset] = r;          // R,G,B always stored
    p[gOffset] = g;
    p[bOffset] = b;
  }
}

void WS2812::setPixelColor(
 uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {

  if(n < numLEDs) {
    if(brightness) { // See notes in setBrightness()
      r = (r * brightness) >> 8;
      g = (g * brightness) >> 8;
      b = (b * brightness) >> 8;
      w = (w * brightness) >> 8;
    }
    uint8_t *p;
    if(wOffset == rOffset) { // Is an RGB-type strip
      p = &pixels[n * 3];    // 3 bytes per pixel (ignore W)
    } else {                 // Is a WRGB-type strip
      p = &pixels[n * 4];    // 4 bytes per pixel
      p[wOffset] = w;        // Store W
    }
    p[rOffset] = r;          // Store R,G,B
    p[gOffset] = g;
    p[bOffset] = b;
  }
}

// Set pixel color from 'packed' 32-bit RGB color:
void WS2812::setPixelColor(uint16_t n, uint32_t c) {
  if(n < numLEDs) {
    uint8_t *p,
      r = (uint8_t)(c >> 16),
      g = (uint8_t)(c >>  8),
      b = (uint8_t)c;
    if(brightness) { // See notes in setBrightness()
      r = (r * brightness) >> 8;
      g = (g * brightness) >> 8;
      b = (b * brightness) >> 8;
    }
    if(wOffset == rOffset) {
      p = &pixels[n * 3];
    } else {
      p = &pixels[n * 4];
      uint8_t w = (uint8_t)(c >> 24);
      p[wOffset] = brightness ? ((w * brightness) >> 8) : w;
    }
    p[rOffset] = r;
    p[gOffset] = g;
    p[bOffset] = b;
  }
}

void WS2812::sendBuffer(uint8_t (*ptr)[3], uint8_t len)
{
  uint8_t buf[24*len];
  uint16_t i, j=0, k, n;
  uint8_t bit;

  for (i = 0; i < len; i++) {
    for(k=0; k < 3; k++) {

      for (bit = 0; bit < 8; bit++) {
        n = 2;
        if(k == 0) n = 1;
        if(k == 1) n = 0;
        if (ptr[i][n] & (0x80 >> bit)) {
          buf[j] = SPILONG;
        } else {
          buf[j] = SPISHORT;
        }
        j++;
      }
    }
  }
#if defined(__MSP430__)
//  k = 0;
//  for(i = 0; i < 24*len; i++){
//    Serial.print(buf[i], HEX);
//    Serial.print(":");
//    k++;
//    if(k == 24) {
//      k = 0;
//      Serial.println();
//    }
//  }
  SPI.transmit(buf, 24 * len);
#else
  SPI.transfer(buf, 24 * len);
#endif

  delayMicroseconds(50);
}

void WS2812::color(uint8_t r, uint8_t g, uint8_t b)
{
  uint8_t color[_pixels][3];
  uint8_t i;

  for (i = 0; i < _pixels; i++) {
      color[i][0] = r;
      color[i][1] = g;
      color[i][2] = b;
  }
  sendBuffer(color, _pixels);
}
