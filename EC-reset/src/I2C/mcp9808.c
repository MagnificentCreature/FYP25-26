#include "CH57x_common.h"
#include "app_i2c.h"

// MCP9808 Default 7-bit I2C Address (assuming A0, A1, A2 are tied to GND)
#define MCP9808_I2C_ADDR    0x18 
// Ambient Temperature Register Pointer
#define MCP9808_REG_TAMBIENT 0x05

uint16_t MCP9808_ReadRawTemperature(void)
{
    uint8_t reg_ptr = MCP9808_REG_TAMBIENT;
    uint8_t rx_data[2] = {0, 0};
    int ret;

    // 1. Write the register pointer. 
    ret = i2c_write_to(MCP9808_I2C_ADDR, &reg_ptr, 1, true, true);
    if (ret < 0) {
        PRINT("I2C Write Error: %d\n", ret);
        return 0xFFFF; // Return 0xFFFF as an obvious error value
    }

    // 2. Read 2 bytes of temperature data.
    ret = i2c_read_from(MCP9808_I2C_ADDR, rx_data, 2, true, 1000);
    if (ret < 2) {
        PRINT("I2C Read Error: %d\n", ret);
        return 0xFFFF; 
    }

    // 3. Just pack the 2 bytes into a 16-bit integer and return it
    // rx_data[0] is the Upper Byte, rx_data[1] is the Lower Byte
    return (uint16_t)((rx_data[0] << 8) | rx_data[1]);
}

uint16_t MCP9808_ReadAndPrintRawTemperature(void)
{
    uint8_t reg_ptr = MCP9808_REG_TAMBIENT;
    uint8_t rx_data[2] = {0, 0};
    int ret;

    // 1. Write the register pointer. 
    ret = i2c_write_to(MCP9808_I2C_ADDR, &reg_ptr, 1, true, true);
    if (ret < 0) {
        PRINT("I2C Write Error: %d\n", ret);
        return 0xFFFF; // Return 0xFFFF as an obvious error value
    }

    // 2. Read 2 bytes of temperature data.
    ret = i2c_read_from(MCP9808_I2C_ADDR, rx_data, 2, true, 1000);
    if (ret < 2) {
        PRINT("I2C Read Error: %d\n", ret);
        return 0xFFFF; 
    }

    // 3. Process the raw bytes using pure integer math JUST for the UART debug print
    uint8_t upper_byte = rx_data[0];
    uint8_t lower_byte = rx_data[1];

    // Isolate the 13-bit data for math
    uint16_t math_temp = ((upper_byte & 0x1F) << 8) | lower_byte;
    int is_negative = 0;

    if (math_temp & 0x1000) {
        is_negative = 1;
        math_temp = 8192 - math_temp; 
    }

    int temp_int = math_temp / 16;
    int temp_frac = (math_temp % 16) * 625; 

    if (is_negative) {
        PRINT("Sensor Debug: -%d.%04d C\n", temp_int, temp_frac);
    } else {
        PRINT("Sensor Debug: %d.%04d C\n", temp_int, temp_frac);
    }

    // 4. Return the raw, unadulterated bytes packed into a 16-bit integer for the radio payload
    return (uint16_t)((upper_byte << 8) | lower_byte);
}

float MCP9808_ReadTemperature(void)
{
    uint8_t reg_ptr = MCP9808_REG_TAMBIENT;
    uint8_t rx_data[2] = {0, 0};
    int ret;

    // 1. Write the register pointer. 
    ret = i2c_write_to(MCP9808_I2C_ADDR, &reg_ptr, 1, true, true);
    if (ret < 0) {
        return -999.0f; // Return an obvious error value
    }

    // 2. Read 2 bytes of temperature data.
    ret = i2c_read_from(MCP9808_I2C_ADDR, rx_data, 2, true, 1000);
    if (ret < 2) {
        return -999.0f; // Return an obvious error value
    }

    // 3. Process the raw bytes using pure integer math for safe printing
    uint8_t upper_byte = rx_data[0];
    uint8_t lower_byte = rx_data[1];

    uint16_t raw_temp = ((upper_byte & 0x1F) << 8) | lower_byte;
    int is_negative = 0;

    if (raw_temp & 0x1000) {
        is_negative = 1;
        raw_temp = 8192 - raw_temp; 
    }

    int temp_int = raw_temp / 16;
    int temp_frac = (raw_temp % 16) * 625; 

    // 4. Calculate and return the actual float for our packet payload
    float tempC = (float)raw_temp * 0.0625f;
    if (is_negative) {
        tempC = -tempC;
    }
    
    return tempC;
}

float MCP9808_ReadAndPrintTemperature(void)
{
    uint8_t reg_ptr = MCP9808_REG_TAMBIENT;
    uint8_t rx_data[2] = {0, 0};
    int ret;

    // 1. Write the register pointer. 
    ret = i2c_write_to(MCP9808_I2C_ADDR, &reg_ptr, 1, true, true);
    if (ret < 0) {
        PRINT("I2C Write Error: %d\n", ret);
        return -999.0f; // Return an obvious error value
    }

    // 2. Read 2 bytes of temperature data.
    ret = i2c_read_from(MCP9808_I2C_ADDR, rx_data, 2, true, 1000);
    if (ret < 2) {
        PRINT("I2C Read Error: %d\n", ret);
        return -999.0f; // Return an obvious error value
    }

    // 3. Process the raw bytes using pure integer math for safe printing
    uint8_t upper_byte = rx_data[0];
    uint8_t lower_byte = rx_data[1];

    uint16_t raw_temp = ((upper_byte & 0x1F) << 8) | lower_byte;
    int is_negative = 0;

    if (raw_temp & 0x1000) {
        is_negative = 1;
        raw_temp = 8192 - raw_temp; 
    }

    int temp_int = raw_temp / 16;
    int temp_frac = (raw_temp % 16) * 625; 

    // 4. Print the result directly
    if (is_negative) {
        PRINT("Current Temperature: -%d.%04d C\n", temp_int, temp_frac);
    } else {
        PRINT("Current Temperature: %d.%04d C\n", temp_int, temp_frac);
    }

    // 5. Calculate and return the actual float for our packet payload
    float tempC = (float)raw_temp * 0.0625f;
    if (is_negative) {
        tempC = -tempC;
    }   
    return tempC;
}