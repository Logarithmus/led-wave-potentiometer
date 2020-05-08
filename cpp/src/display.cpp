#include "display.hpp"

#include "freertos.hpp"
#include "tm1637/tm1637.hpp"
#include <util/delay.h>
#include <stdlib.h>

namespace display {

	void task(void*) {
		TM1637 display;
		display.clear();
		display.setBrightness(2);

		uint8_t prev_value = 0;
		while (true) {
			uint8_t value = (uint8_t) ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000));
			if (value != prev_value) {
				display.setNumberPad(value, 3, 4);
				prev_value = value;
			}
		}
	}	
}