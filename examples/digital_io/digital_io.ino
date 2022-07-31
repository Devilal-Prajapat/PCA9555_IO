#include "pca9555.h"

#define PAUSE   1000

void setup()
{
  Serial.begin(115200);
  pca9555.init();
  pca9555.set_address(0x20); // Not Required Default address is 0x20
  pca9555.write_io(CONFIG_P0, B00000000); //defines all pins on Port0 are outputs
  pca9555.write_io(CONFIG_P1, B00000000); //defines all pins on Port1 are outputs
  pca9555.write_io(OUT_P0, B00000000); //clears all relays
  pca9555.write_io(OUT_P1, B00000000); //clears all relays
}

void loop()
{
  Serial.print("Writing\r\n");
  for (uint16_t i = PIN_0; i <= PIN_15 ; i++)
  {
    pca9555.digital_write(i, SET);
    Serial.print(" ");
    Serial.print(pca9555.get_output());
    delay(PAUSE);
    pca9555.digital_write(i, RESET);
    Serial.print(" ");
    Serial.print(pca9555.get_output());
    delay(PAUSE);
  }

#if 0
  for (int i = PIN_15; i >= PIN_0; i--)
  {
    pca9555.digital_write(i, RESET);
    Serial.print(" ");
    Serial.print(pca9555.get_output());
    delay(PAUSE);
  }
  delay(PAUSE);
#endif
}