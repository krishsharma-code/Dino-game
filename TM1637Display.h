#ifndef __TM1637DISPLAY__
#define __TM1637DISPLAY__

#include <inttypes.h>

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000

#define DEFAULT_BIT_DELAY  100

// Local driver for the four-digit score display used by the Dino game.
class TM1637Display {

public:
  TM1637Display(uint8_t pinClk, uint8_t pinDIO, unsigned int bitDelay = DEFAULT_BIT_DELAY);
  void setBrightness(uint8_t brightness, bool on = true);
  void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);
  void clear();
  void showNumberDec(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
  void showNumberDecEx(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
  void showNumberHexEx(uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
  uint8_t encodeDigit(uint8_t digit);

protected:
   void bitDelay();
   void start();
   void stop();
   bool writeByte(uint8_t b);
   void showDots(uint8_t dots, uint8_t* digits);
   void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

private:
	uint8_t m_pinClk;
	uint8_t m_pinDIO;
	uint8_t m_brightness;
	unsigned int m_bitDelay;
};

#endif
