#include <stdint.h>

#include "FreeRTOS.h"
#include "hardware/timer.h"
#include "semphr.h"

static uint64_t time_offset;
// uint64_t isn't atomic on this platform so we need a mutex
static SemaphoreHandle_t offset_mutex;

void time_init() {
	time_offset = 0;
	offset_mutex = xSemaphoreCreateMutex();
}

uint64_t time_get() {
	uint64_t time = time_us_64();
	xSemaphoreTake(offset_mutex, portMAX_DELAY);
	time += time_offset;
	xSemaphoreGive(offset_mutex);
	return time;
}

void time_set_sntp(uint32_t sec, uint32_t us) {
	uint64_t time_since_boot = time_us_64();
	uint64_t ntp_time = (uint64_t)sec * 1000000 + us;
	xSemaphoreTake(offset_mutex, portMAX_DELAY);
	time_offset = ntp_time - time_since_boot;
	xSemaphoreGive(offset_mutex);
}
