#include <Arduino.h>
#include <Wire.h>
#include "pca9555.h"

union {
	struct {
		uint8_t low_byte;                  // low order byte
		uint8_t high_byte;                 // high order byte
	}u8_data;
	uint16_t data;
}u16_data;

uint16_t PCA9555 :: output_reg = 0;

PCA9555::PCA9555()
{
    I2C_Addr = PCA9555_ADDR;
    output_reg = 0;
}

PCA9555::~PCA9555()
{
    output_reg = 0;
}

/*!
 *	Init I2C Wire Interface
 */

void PCA9555::init(void)
{  
   Wire.begin();
}
/*!
 *	set I2C address
 *	Defualt is 0x20
 */
void  PCA9555:: set_address(uint8_t addr)
{
	I2C_Addr = addr;
}  

/*!
 *	get I2C address
 *	Defualt is 0x20
 */
uint8_t PCA9555:: get_address(void)
{
	return I2C_Addr;
}    

void PCA9555::write_io(int command, int value)
{
#if 1
  Wire.beginTransmission(I2C_Addr);
  Wire.write(command);
  Wire.write(value);
  Wire.endTransmission();
#endif
}

// i2c one byte read
uint16_t PCA9555 :: read_io(uint8_t reg)
{
	uint16_t inputData;
#if 1
	Wire.beginTransmission(I2C_Addr);
	Wire.write(reg);
	Wire.endTransmission();
	if (Wire.requestFrom((int)I2C_Addr, 1) != 1)
    {

        return 256;   // error code is above normal data range
    }
    inputData = Wire.read();
 #endif

    return inputData;
}

void PCA9555 :: digital_write(uint8_t pin, uint8_t pin_state)
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
#if 0
	Wire.beginTransmission(I2C_Addr);
	Wire.write(OUT_P0);
	Wire.write(output_reg & 0xFF);
	Wire.endTransmission();

	Wire.beginTransmission(I2C_Addr);
	Wire.write(OUT_P1);
	Wire.write ((output_reg >> 8) & 0xFF);
	Wire.endTransmission();

#else
	write_io(OUT_P0, output_reg & 0xFF);
	write_io(OUT_P1, (output_reg >> 8) & 0xFF);	
#endif	
}

uint8_t PCA9555 :: digital_read(uint8_t pin)
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

uint16_t PCA9555::get_output()
{
    return output_reg;
}

PCA9555 pca9555;