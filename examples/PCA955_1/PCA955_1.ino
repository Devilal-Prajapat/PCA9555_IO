#include <Wire.h>

//  with no jumpers the full address is   0 1 0 0 1 1 1    1 0 0 A2 A1 A0  0x27 is the default address for the DFR0013 board with no jumpers.
#define PCA9555 0x20 
// COMMAND BYTE TO REGISTER RELATIONSHIP FROM PCA9555 DATA SHEET
// At reset, the device's ports are inputs with a high value resistor pull-ups to VDD
// If relays turning on during power up are a problem. Add a pull down resistor to each relay transistor base.

#define IN_P0 0x00 // Read Input port0
#define IN_P1 0x01 // Read Input port1
#define OUT_P0 0x02 // Write Output port0
#define OUT_P1 0x03 // Write Output port1
#define INV_P0 0x04 // Input Port Polarity Inversion port0 if B11111111 is written input polarity is inverted
#define INV_P1 0x05 // Input Port Polarity Inversion port1 if B11111111 is written input polarity is inverted
#define CONFIG_P0 0x06 // Configuration port0 configures the direction of the I/O pins 0 is output 1 is input
#define CONFIG_P1 0x07 // Configuration port1 configures the direction of the I/O pins 0 is output 1 is input

#define SET     1
#define RESET   0
union {
	struct {
		uint8_t low_byte;                  // low order byte
		uint8_t high_byte;                 // high order byte
	}u8_data;
	uint16_t data;
}u16_data;

uint16_t output_reg = 0;;
enum{
	PIN_0=0,
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7,
	PIN_8,
	PIN_9,
	PIN_10,
	PIN_11,
	PIN_12,
	PIN_13,
	PIN_14,
	PIN_15,
};

#define PAUSE 1000

void setup()
{ 
  Wire.begin();
  Serial.begin(9600);
  i2c_scanner();
  
//  Wire.begin(PCA9555); // join i2c bus (address optional for master) tried to get working
  write_io(CONFIG_P0, B00000000); //defines all pins on Port0 are outputs
  write_io(CONFIG_P1, B00000000); //defines all pins on Port1 are outputs
  write_io(OUT_P0, B00000000); //clears all relays
  write_io(OUT_P1, B00000000); //clears all relays
  delay (PAUSE);
}

void loop()
{
    Serial.print ("Writing\r\n");
	digital_write(PIN_0, SET);
	delay(PAUSE);
	digital_write(PIN_0, RESET);
	delay(PAUSE);
}

void i2c_scanner(void)
{
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;
  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);
    } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
  delay(5000);
}


// one byte write
void write_io(int command, int value)
{
  Wire.beginTransmission(PCA9555);
  Wire.write(command);
  Wire.write(value);
  Wire.endTransmission();
}

// i2c one byte read
uint16_t read_io(uint8_t reg)
{
	uint16_t inputData;
	Wire.beginTransmission(PCA9555);
	Wire.write(reg);
	Wire.endTransmission();
	if (Wire.requestFrom((int)PCA9555, 1) != 1)
    {

        return 256;   // error code is above normal data range
    }
    inputData = Wire.read();
    return inputData;
}

void digital_write(uint8_t pin, uint8_t pin_state)
{
	if(pin > 15)
		return;
	if(pin_state == 1)
	{
		output_reg |= (1 << pin);
	}
	else
	{
		output_reg &= ~(1 << pin);
	}
	write_io(OUT_P0, output_reg & 0xFF);
	write_io(OUT_P1, (output_reg >> 8) & 0xFF);	
}

uint8_t digital_read(uint8_t pin)
{
	uint16_t pin_state;
	pin_state = read_io(IN_P0);
	pin_state |= (read_io(IN_P1) << 8);
	
	if(pin > 15)
		return 0xFF;
	
	if((pin_state & (1 << pin)) > 0)
	{
		return SET;
	} else
	{
		return RESET;
	}	
}
