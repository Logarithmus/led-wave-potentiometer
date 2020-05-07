#include <avr/sfr_defs.h>
#include <avr/iom128.h>
#include <avr/io.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#ifndef NULL
#define NULL	(void *) 0
#endif

void vLED_1 ( void *pvParameters );
void vLED_2 ( void *pvParameters );

int main ( void ) {

	xTaskCreate ( vLED_1, (const char *) "LED 1", 256, NULL, 1, NULL );
	xTaskCreate ( vLED_2, (const char *) "LED 2", 256, NULL, 1, NULL );
	vTaskStartScheduler();

	while (1);
	return 0;
 }

void vLED_1 ( void *pvParameters ) {
	DDRG |= _BV ( DDG3 );
	while (1) {
		vTaskDelay ( 1000 );
		PORTG ^= _BV ( PG3 );
	}
}

void vLED_2 ( void *pvParameters ) {
	DDRG |= _BV ( DDG4 );
	while (1) {
		vTaskDelay ( 500 );
		PORTG ^= _BV ( PG4 );
	}
}

void vApplicationStackOverflowHook ( TaskHandle_t xTask, char *pcTaskName ) {
}
