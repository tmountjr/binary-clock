#include <iostream>
#include <fstream>

uint8_t _NeoPixelGammaTable[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3,
    3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6,
    6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10,
    11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17,
    17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 31, 32, 33, 34, 34, 35,
    36, 37, 38, 38, 39, 40, 41, 42, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 80, 81,
    82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96, 97, 99, 100, 102,
    103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125,
    127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152,
    154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182,
    184, 186, 188, 191, 193, 195, 197, 199, 202, 204, 206, 209, 211, 213, 215,
    218, 220, 223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252,
    255};

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  const long dividend = out_max - out_min;
  const long divisor = in_max - in_min;
  const long delta = x - in_min;

  return (delta * dividend + (divisor / 2)) / divisor + out_min;
}

uint32_t gamma32(uint32_t x)
{
  uint8_t *y = (uint8_t *)&x;
  // All four bytes of a 32-bit value are filtered even if RGB (not WRGB),
  // to avoid a bunch of shifting and masking that would be necessary for
  // properly handling different endianisms (and each byte is a fairly
  // trivial operation, so it might not even be wasting cycles vs a check
  // and branch for the RGB case). In theory this might cause trouble *if*
  // someone's storing information in the unused most significant byte
  // of an RGB value, but this seems exceedingly rare and if it's
  // encountered in reality they can mask values going in or coming out.
  for (uint8_t i = 0; i < 4; i++)
    y[i] = _NeoPixelGammaTable[y[i]];
  return x; // Packed 32-bit return
}

uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val)
{

  uint8_t r, g, b;

  hue = (hue * 1530L + 32768) / 65536;
  // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
  if (hue < 510)
  { // Red to Green-1
    b = 0;
    if (hue < 255)
    { //   Red to Yellow-1
      r = 255;
      g = hue; //     g = 0 to 254
    }
    else
    {                //   Yellow to Green-1
      r = 510 - hue; //     r = 255 to 1
      g = 255;
    }
  }
  else if (hue < 1020)
  { // Green to Blue-1
    r = 0;
    if (hue < 765)
    { //   Green to Cyan-1
      g = 255;
      b = hue - 510; //     b = 0 to 254
    }
    else
    {                 //   Cyan to Blue-1
      g = 1020 - hue; //     g = 255 to 1
      b = 255;
    }
  }
  else if (hue < 1530)
  { // Blue to Red-1
    g = 0;
    if (hue < 1275)
    {                 //   Blue to Magenta-1
      r = hue - 1020; //     r = 0 to 254
      b = 255;
    }
    else
    { //   Magenta to Red-1
      r = 255;
      b = 1530 - hue; //     b = 255 to 1
    }
  }
  else
  { // Last 0.5 Red (quicker than % operator)
    r = 255;
    g = b = 0;
  }

  // Apply saturation and value to R,G,B, pack into 32-bit result:
  uint32_t v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
  uint16_t s1 = 1 + sat;  // 1 to 256; same reason
  uint8_t s2 = 255 - sat; // 255 to 0
  return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
         (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
         (((((b * s1) >> 8) + s2) * v1) >> 8);
}

uint32_t convertToHSV(uint8_t counter, uint8_t counter_max)
{
  uint16_t hue = map(map(counter, 0, counter_max, 0, 360), 360, 0, 0, 65535);
  return gamma32(ColorHSV(hue, 255, 255));
}

int main()
{
  std::ofstream secondsMinutesFile("seconds_minutes.bin", std::ios::binary);
  std::ofstream hoursFile("hours.bin", std::ios::binary);

  if (secondsMinutesFile && hoursFile)
  {
    for (int i = 0; i < 60; i++)
    {
      uint32_t color = convertToHSV(i, 59);
      secondsMinutesFile.write((const char*)&color, sizeof(uint32_t));

      if (i < 24) {
        color = convertToHSV(i, 23);
        hoursFile.write((const char*)&color, sizeof(uint32_t));
      }
    }
    std::cout << "HSV values written to files successfully!\n";
  }
  else
  {
    std::cerr << "Error opening files!\n";
  }

  return 0;
}
