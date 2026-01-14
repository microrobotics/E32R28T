# XPT2046 Resitive Touch Controller 
The XPT2046 is a low-power resistive touch controller IC commonly used with TFT displays. It communicates via SPI, converts touch pressure and X/Y position into digital data, supports 12-bit resolution, and is widely compatible with microcontrollers like Arduino and ESP32.

## Recommended Arduino Library

XPT2046_Touchscreen by Paul Stoffregen

<img width="262" height="320" alt="image" src="https://github.com/user-attachments/assets/d533dfbc-8aa5-4cf8-b7bf-8cfe122f2afe" />

## SPI Controllers on the ESP32
The ESP32 has 4 SPI controllers, but only 2 are available for general use:

# All 4 SPI Controllers:

SPI0 (SPI1) - Reserved for flash memory (can't use)
SPI1 - Also reserved for flash memory (can't use)
HSPI (SPI2) - ✅ Available for your projects
VSPI (SPI3) - ✅ Available for your projects

Our Touch Example use VSPI 
