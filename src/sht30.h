#pragma once

#include <stdint.h>
#include <time.h>

struct sht30_result {
	time_t sample_time;
	float temp_c;
	float relative_humidity;
};

void sht30_reset_all();

uint16_t sht30_get_firmware_version();
void sht30_start_capture();
struct sht30_result sht30_fetch_data();
