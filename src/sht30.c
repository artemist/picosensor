#include <math.h>
#include <stdio.h>

#include "hardware/i2c.h"
#include "pico/stdlib.h"

struct sht30_result {
	float temp_c;
	float relative_humidity;
};

static struct i2c_inst *i2c = i2c1;
const static uint32_t sda_pin = 2;
const static uint32_t scl_pin = 3;
const static uint8_t addr = 0x44;
const static uint8_t crc8_table[] = {
	0,	 49,  98,  83,	196, 245, 166, 151, 185, 136, 219, 234, 125, 76,  31,
	46,	 67,  114, 33,	16,	 135, 182, 229, 212, 250, 203, 152, 169, 62,  15,
	92,	 109, 134, 183, 228, 213, 66,  115, 32,	 17,  63,  14,	93,	 108, 251,
	202, 153, 168, 197, 244, 167, 150, 1,	48,	 99,  82,  124, 77,	 30,  47,
	184, 137, 218, 235, 61,	 12,  95,  110, 249, 200, 155, 170, 132, 181, 230,
	215, 64,  113, 34,	19,	 126, 79,  28,	45,	 186, 139, 216, 233, 199, 246,
	165, 148, 3,   50,	97,	 80,  187, 138, 217, 232, 127, 78,	29,	 44,  2,
	51,	 96,  81,  198, 247, 164, 149, 248, 201, 154, 171, 60,	13,	 94,  111,
	65,	 112, 35,  18,	133, 180, 231, 214, 122, 75,  24,  41,	190, 143, 220,
	237, 195, 242, 161, 144, 7,	  54,  101, 84,	 57,  8,   91,	106, 253, 204,
	159, 174, 128, 177, 226, 211, 68,  117, 38,	 23,  252, 205, 158, 175, 56,
	9,	 90,  107, 69,	116, 39,  22,  129, 176, 227, 210, 191, 142, 221, 236,
	123, 74,  25,  40,	6,	 55,  100, 85,	194, 243, 160, 145, 71,	 118, 37,
	20,	 131, 178, 225, 208, 254, 207, 156, 173, 58,  11,  88,	105, 4,	  53,
	102, 87,  192, 241, 162, 147, 189, 140, 223, 238, 121, 72,	27,	 42,  193,
	240, 163, 146, 5,	52,	 103, 86,  120, 73,	 26,  43,  188, 141, 222, 239,
	130, 179, 224, 209, 70,	 119, 36,  21,	59,	 10,  89,  104, 255, 206, 157,
	172};

static uint8_t crc8(uint8_t msb, uint8_t lsb) {
	return crc8_table[lsb ^ crc8_table[msb ^ 0xff]];
}

static void sht30_reset_all() {
	// Generall call recet
	uint8_t cmd[] = {0x06};
	i2c_write_blocking(i2c, 0x00, cmd, sizeof(cmd), false);
	// Datasheet says max hard reset time is 1ms, soft reset is 1.5ms
	sleep_ms(2);
}

static uint16_t sht30_get_firmware_version() {
	uint8_t cmd[] = {0xd1, 0x00};
	i2c_write_blocking(i2c, addr, cmd, sizeof(cmd), true);
	uint8_t resp[3];
	i2c_read_blocking(i2c, addr, resp, sizeof(resp), false);
	if (crc8(resp[0], resp[1]) != resp[2]) return 0xffff;
	return (uint16_t)resp[0] << 8 | resp[1];
}

static void sht30_start_capture() {
	// High repeatability, 1 sample per second
	uint8_t cmd[] = {0x21, 0x30};
	i2c_write_blocking(i2c, addr, cmd, sizeof(cmd), false);
};

static struct sht30_result sht30_fetch_data() {
	uint8_t cmd[] = {0xe0, 0x00};
	i2c_write_blocking(i2c, addr, cmd, sizeof(cmd), true);
	uint8_t resp[6];
	i2c_read_blocking(i2c, addr, resp, sizeof(resp), false);

	float temp_c, relative_humidity;
	if (crc8(resp[0], resp[1]) == resp[2])
		temp_c = -45 + 175 * (float)(resp[0] << 8 | resp[1]) / 0xffff;
	else
		temp_c = NAN;

	if (crc8(resp[3], resp[4]) == resp[5])
		relative_humidity = 100 * (float)(resp[3] << 8 | resp[4]) / 0xffff;
	else
		relative_humidity = NAN;

	struct sht30_result result = {.temp_c = temp_c,
								  .relative_humidity = relative_humidity};
	return result;
}

int main() {
	stdio_init_all();

	i2c_init(i2c, 100 * 1000);
	gpio_set_function(sda_pin, GPIO_FUNC_I2C);
	gpio_set_function(scl_pin, GPIO_FUNC_I2C);
	gpio_pull_up(sda_pin);
	gpio_pull_up(scl_pin);

	sleep_ms(2000);

	sht30_reset_all();

	sht30_start_capture();

	while (true) {
		sleep_ms(1000);
		struct sht30_result result = sht30_fetch_data();
		printf("{\"temperature\": %f, \"humidity\": %f}\n", result.temp_c,
			   result.relative_humidity);
	}
	return 0;
}
