#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

void main_task(__unused void *params) {
	if(cyw43_arch_init()) {
		printf("Failed to initialize cyw43");
		return;
	}

}


int main() {
	stdio_init_all();

	TaskHandle_t task;
	xTaskCreate(main_task, "MainThread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &task);
	vTaskStartScheduler();
}
