#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

// I2C (TWI) functions
void TWI_init() {
    TWSR = 0x00;
    TWBR = 72; // ~100kHz
}

void TWI_start() {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void TWI_stop() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    _delay_ms(1);
}

void TWI_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

// PCF8574 address (0x27 << 1)
#define LCD_ADDR 0x4E

// Control bits
#define EN 0x04
#define RS 0x01

void lcd_pulse(uint8_t data) {
    TWI_write(data | EN);
    _delay_us(1);
    TWI_write(data & ~EN);
    _delay_us(50);
}

void lcd_send_nibble(uint8_t nibble, uint8_t control) {
    uint8_t data = (nibble & 0xF0) | control | 0x08; // backlight ON
    lcd_pulse(data);
}

void lcd_send_byte(uint8_t byte, uint8_t control) {
    lcd_send_nibble(byte & 0xF0, control);
    lcd_send_nibble((byte << 4) & 0xF0, control);
}

void lcd_command(uint8_t cmd) {
    TWI_start();
    TWI_write(LCD_ADDR);
    lcd_send_byte(cmd, 0);
    TWI_stop();
    _delay_ms(2);
}

void lcd_data(uint8_t data) {
    TWI_start();
    TWI_write(LCD_ADDR);
    lcd_send_byte(data, RS);
    TWI_stop();
}

void lcd_init() {
    _delay_ms(50);

    lcd_command(0x33);
    lcd_command(0x32);
    lcd_command(0x28); // 4-bit, 2-line
    lcd_command(0x0C); // display ON
    lcd_command(0x06); // entry mode
    lcd_command(0x01); // clear
    _delay_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    uint8_t addr = (row == 0) ? 0x80 : 0xC0;
    lcd_command(addr + col);
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_data(*str++);
    }
}

int main(void) {
    TWI_init();
    lcd_init();

    lcd_set_cursor(0, 0);
    lcd_print("M Abdullah Aif");

    // lcd_set_cursor(0, 1);
    // lcd_print("Abdullah");

    while (1) {
        // infinite loop
    }
}