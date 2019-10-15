# `WS2812`






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
[`public  WS2812(uint16_t n,uint8_t t)`](#class_w_s2812_1a14f6c4f36209c7935a1a32c57a7ab806) | Instantiate the [WS2812](#class_w_s2812) library.
[`public  WS2812(void)`](#class_w_s2812_1af27bb254744dbb9f9cca3da909bf3dfc) | 
[`public  ~WS2812()`](#class_w_s2812_1a58973dedd9cbc5c3fd3397f07f9a720f) | 
[`public void begin()`](#class_w_s2812_1a832a5585b1ac0f3897442446966494a7) | Start the [WS2812](#class_w_s2812) library.
[`public void sendBuffer(uint8_t(*) ptr,uint8_t len)`](#class_w_s2812_1aef7d068a0cc3c7411f2b3ad194bc8308) | Send buffer to strip.
[`public void show(void)`](#class_w_s2812_1ad4f265961902a11e874eb3fe513af12f) | TODO Description.
[`public void setPixelColor(uint16_t n,uint8_t r,uint8_t g,uint8_t b)`](#class_w_s2812_1a129a80f5b2e3672883269cd2a2673df6) | 
[`public void setPixelColor(uint16_t n,uint8_t r,uint8_t g,uint8_t b,uint8_t w)`](#class_w_s2812_1a6f9bbbcb9348a091dd6e3966cc9137ec) | 
[`public void setPixelColor(uint16_t n,uint32_t c)`](#class_w_s2812_1aa939c9c9db22173bfde6318e7ce0b5fb) | 
[`public void fill(uint32_t c,uint16_t first,uint16_t count)`](#class_w_s2812_1a44a9fec36effa24e882e13b9ceca29b1) | 
[`public void setBrightness(uint8_t)`](#class_w_s2812_1a733f34932b2abee81f8509eeededf5d9) | 
[`public void clear()`](#class_w_s2812_1aceeaba644d2cc545b84d2807732b6461) | 
[`public void updateLength(uint16_t n)`](#class_w_s2812_1a3d0664c2e63c2c3e71b6dd8e89890b0c) | 
[`public void updateType(uint8_t t)`](#class_w_s2812_1a8c864c9c238c46d88f7201ee98edb279) | 
[`public uint8_t * getPixels(void) const`](#class_w_s2812_1a6e6043389b826180dad8663ead9d8ce5) | 
[`public uint8_t getBrightness(void) const`](#class_w_s2812_1a330f62082225f2350717419f5d2fd528) | 
[`public uint8_t sine8(uint8_t) const`](#class_w_s2812_1a6bfc768ec1a60a0f7ecc11be1b420fb7) | 
[`public uint8_t gamma8(uint8_t) const`](#class_w_s2812_1a350a5be1f0f3cf1edef7f8d8a6a59070) | 
[`public inline int8_t getPin(void)`](#class_w_s2812_1ad2b9ac664f39e6bde2369998fce92011) | 
[`public uint16_t numPixels(void) const`](#class_w_s2812_1a11883fac8018facae0ba793500417d8f) | 
[`public uint32_t getPixelColor(uint16_t n) const`](#class_w_s2812_1a26a569ce9f8f5e66155007980546c52b) | 
[`public inline bool canShow(void)`](#class_w_s2812_1adc97a8c792f90ebfdf318b7197373fa7) | 
[`public void color(uint8_t r,uint8_t g,uint8_t b)`](#class_w_s2812_1ab5c01e177a1c2158e96852380364134a) | Set entire strip to the RGB color.
[`public inline void off()`](#class_w_s2812_1ab4502db06773f603fd27f01f73d1872d) | Turn the strip off.

## Members


### <a name="class_w_s2812_1a14f6c4f36209c7935a1a32c57a7ab806"></a>`public  WS2812(uint16_t n,uint8_t t)`

Instantiate the [WS2812](#class_w_s2812) library.

#### Parameters
* `pixels` The number of pixels the string contains.

### <a name="class_w_s2812_1af27bb254744dbb9f9cca3da909bf3dfc"></a>`public  WS2812(void)`





### <a name="class_w_s2812_1a58973dedd9cbc5c3fd3397f07f9a720f"></a>`public  ~WS2812()`





### <a name="class_w_s2812_1a832a5585b1ac0f3897442446966494a7"></a>`public void begin()`

Start the [WS2812](#class_w_s2812) library.

Initialize SPI and turn the entire strip off.

### <a name="class_w_s2812_1aef7d068a0cc3c7411f2b3ad194bc8308"></a>`public void sendBuffer(uint8_t(*) ptr,uint8_t len)`

Send buffer to strip.

Send the RGB buffer to the strip. 
#### Parameters
* `buffer` The RGB buffer. 


* `length` The length of the buffer.

### <a name="class_w_s2812_1ad4f265961902a11e874eb3fe513af12f"></a>`public void show(void)`

TODO Description.

TODO Description 
#### Parameters
* `TODO.`

### <a name="class_w_s2812_1a129a80f5b2e3672883269cd2a2673df6"></a>`public void setPixelColor(uint16_t n,uint8_t r,uint8_t g,uint8_t b)`





### <a name="class_w_s2812_1a6f9bbbcb9348a091dd6e3966cc9137ec"></a>`public void setPixelColor(uint16_t n,uint8_t r,uint8_t g,uint8_t b,uint8_t w)`





### <a name="class_w_s2812_1aa939c9c9db22173bfde6318e7ce0b5fb"></a>`public void setPixelColor(uint16_t n,uint32_t c)`





### <a name="class_w_s2812_1a44a9fec36effa24e882e13b9ceca29b1"></a>`public void fill(uint32_t c,uint16_t first,uint16_t count)`





### <a name="class_w_s2812_1a733f34932b2abee81f8509eeededf5d9"></a>`public void setBrightness(uint8_t)`





### <a name="class_w_s2812_1aceeaba644d2cc545b84d2807732b6461"></a>`public void clear()`





### <a name="class_w_s2812_1a3d0664c2e63c2c3e71b6dd8e89890b0c"></a>`public void updateLength(uint16_t n)`





### <a name="class_w_s2812_1a8c864c9c238c46d88f7201ee98edb279"></a>`public void updateType(uint8_t t)`





### <a name="class_w_s2812_1a6e6043389b826180dad8663ead9d8ce5"></a>`public uint8_t * getPixels(void) const`





### <a name="class_w_s2812_1a330f62082225f2350717419f5d2fd528"></a>`public uint8_t getBrightness(void) const`





### <a name="class_w_s2812_1a6bfc768ec1a60a0f7ecc11be1b420fb7"></a>`public uint8_t sine8(uint8_t) const`





### <a name="class_w_s2812_1a350a5be1f0f3cf1edef7f8d8a6a59070"></a>`public uint8_t gamma8(uint8_t) const`





### <a name="class_w_s2812_1ad2b9ac664f39e6bde2369998fce92011"></a>`public inline int8_t getPin(void)`





### <a name="class_w_s2812_1a11883fac8018facae0ba793500417d8f"></a>`public uint16_t numPixels(void) const`





### <a name="class_w_s2812_1a26a569ce9f8f5e66155007980546c52b"></a>`public uint32_t getPixelColor(uint16_t n) const`





### <a name="class_w_s2812_1adc97a8c792f90ebfdf318b7197373fa7"></a>`public inline bool canShow(void)`





### <a name="class_w_s2812_1ab5c01e177a1c2158e96852380364134a"></a>`public void color(uint8_t r,uint8_t g,uint8_t b)`

Set entire strip to the RGB color.

#### Parameters
* `red` 8 bit red value. 


* `green` 8 bit green value. 


* `blue` 8 bit blue value.

### <a name="class_w_s2812_1ab4502db06773f603fd27f01f73d1872d"></a>`public inline void off()`

Turn the strip off.




