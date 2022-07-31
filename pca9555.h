#ifndef PCA9555_H
#define PCA9555_H

#define PCA9555_ADDR 0x20
// COMMAND BYTE TO REGISTER RELATIONSHIP FROM PCA9555 DATA SHEET
// At reset, the device's ports are inputs with a high value resistor pull-ups to VDD
// If relays turning on during power up are a problem. Add a pull down resistor to each relay transistor base.

#define IN_P0      0x00 // Read Input port0
#define IN_P1      0x01 // Read Input port1
#define OUT_P0     0x02 // Write Output port0
#define OUT_P1     0x03 // Write Output port1
#define INV_P0     0x04 // Input Port Polarity Inversion port0 if B11111111 is written input polarity is inverted
#define INV_P1     0x05 // Input Port Polarity Inversion port1 if B11111111 is written input polarity is inverted
#define CONFIG_P0  0x06 // Configuration port0 configures the direction of the I/O pins 0 is output 1 is input
#define CONFIG_P1  0x07 // Configuration port1 configures the direction of the I/O pins 0 is output 1 is input

#define SET     1
#define RESET   0

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

class PCA9555
{
    private:
        /* data */
        static uint16_t output_reg;
	uint8_t I2C_Addr;
    public:
        PCA9555();
        ~PCA9555();
        void init(void);
        void write_io(int command, int value);
        uint16_t read_io(uint8_t reg);
        void digital_write(uint8_t pin, uint8_t pin_state);
        uint8_t digital_read(uint8_t pin);
        uint16_t get_output(void);    
		uint8_t get_address(void);
		void set_address(uint8_t addr);    
};

extern PCA9555 pca9555;

#endif /* end PCA9555_H */

