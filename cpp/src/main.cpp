#include <stdint.h>

#include "avr_io.hpp"
#include "freertos.hpp"
#include "leds.hpp"
#include "adc.hpp"
#include "display.hpp"

int main() {
	xTaskCreate(leds::task, "leds", 64, NULL, 1, NULL);
	xTaskCreate(adc::task, "adc", 64, NULL, 1, NULL);
	vTaskStartScheduler();
	while (1);
	return 0;
}