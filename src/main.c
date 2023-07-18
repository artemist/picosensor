#include "FreeRTOS.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "secret.h"
#include "sht30.h"
#include "task.h"

void sample_task(void *params);
void upload_task(void *params);

void upload_task(__unused void *params) {
	if (cyw43_arch_init()) {
		printf("Failed to initialize cyw43");
		vTaskDelete(NULL);
	}

	cyw43_arch_enable_sta_mode();
	printf("Connecting to Wi-Fi network %s...\n", WIFI_SSID);
	if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD,
										   CYW43_AUTH_WPA2_AES_PSK, 30000)) {
		printf("Failed to connect.\n");
		vTaskDelete(NULL);
	} else {
		printf("Connected.\n");
	}

	// We can't start the sample task until NTP sync has happened
	TaskHandle_t task;
	xTaskCreate(sample_task, "SampleTask", configMINIMAL_STACK_SIZE, NULL,
				tskIDLE_PRIORITY + 2, &task);
}

void sample_task(void *params) {}

int main() {
	stdio_init_all();
	printf("owo");

	TaskHandle_t task;
	xTaskCreate(upload_task, "UploadTask", configMINIMAL_STACK_SIZE, NULL,
				tskIDLE_PRIORITY + 1, &task);
	vTaskStartScheduler();
}
