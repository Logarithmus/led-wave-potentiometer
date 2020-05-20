#include "leds.hpp"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "freertos.hpp"
#include "mpu_wrappers.h"

namespace {
	inline void init() {
		DDRB = 0b1100'0011;
		DDRD = 0b1111'1100;
	}

	inline uint8_t get_bit(uint16_t bits, uint8_t i) {
		return (bits >> i) & 1;
	}

	void set(uint16_t bits) {
		PORTD = (get_bit(bits, 9) << 2)
			| (get_bit(bits, 8) << 3)
			| (get_bit(bits, 7) << 4) 
			| (get_bit(bits, 4) << 5) 
			| (get_bit(bits, 3) << 6) 
			| (get_bit(bits, 2) << 7);
		PORTB = (get_bit(bits, 6) << 6)
			| (get_bit(bits, 5) << 7)
			| (get_bit(bits, 1) << 0) 
			| (get_bit(bits, 0) << 1);
	}
}

namespace leds {
	void task(void*) {
		constexpr uint8_t LEDS_COUNT = 10;
		//constexpr uint16_t WAVE_START = 1 << (LEDS_COUNT - 1);
		constexpr uint16_t MIN_DELAY = 200;
		constexpr uint16_t MAX_DELAY = 5000;

		const auto adc_task = xTaskGetHandle("adc");
		init();
		uint8_t direction = -1;
		uint16_t wave = 0;
		uint8_t i = 9;
		while (true) {
			uint16_t speed = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000));
			uint16_t period_ms = MAX_DELAY - ((MAX_DELAY - MIN_DELAY) / UINT8_MAX) * speed;
			uint16_t delay = pdMS_TO_TICKS(period_ms / (LEDS_COUNT * 2));
			
			if ((wave & 1023) == 1023) {
				direction = 1;
				wave ^= 1;
			} else if (wave == 0) {
				direction = -1;
				// wave ^= (1 << 9);
			}
			wave ^= (1 << i);
			i += direction;
			set(wave);
			xTaskNotifyGive(adc_task);
			vTaskDelay(delay);
		}
	}
}