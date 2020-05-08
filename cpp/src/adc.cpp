#include "adc.hpp"

#include <avr/io.h>
#include "freertos.hpp"
#include "mpu_wrappers.h"

namespace {
	void init() {
		// REFS  - select AVcc (== Vcc, normally 3.3 - 5 V) as reference for ADC
		// ADLAR - left-adjustment of the result, 8-bits precision is enough
		// MUX   - select channel 0 (pin ADCas ADC input
		ADMUX =   0b0110'0000;

	    // ADATE - auto trigger enabled
	    // ADIF  - conversion complete interrupt flag, unused because ADIE is disabled
	    // ADIE  - conversion complete interrupt, disabled
	    // ADPS  - ADC prescaler division factor, set to 8 (1 < 0b011 == 8)
	    ADCSRA =  0b0000'0110;

	    // ACME  - analog comparator, unused
	    // ADTS  - trigger source, 0b000 for free running mode
	    ADCSRB =  0b0000'0000;

	    // ADC5..0 - disable channels to save power (3..1 are disabled)
	    // 5..4 are used for display, 0 is used for potentiometer
	    DIDR0 =   0b0000'1110;

	    // ADEN  - ADC enable
	    ADCSRA |= (1 << ADEN);
	}
}

namespace adc {
	uint8_t get() {
		ADCSRA |= (1 << ADSC);
		//While conversion is in progress
		while (ADCSRA & (1 << ADSC));
		return ADCH;
	}

	void task(void*) {
		const auto leds_task = xTaskGetHandle("leds");
		const auto display_task = xTaskGetHandle("display");
		init();
		while (true) {
			const uint8_t value = get();
			xTaskNotify(leds_task, value, eSetValueWithoutOverwrite);
			xTaskNotify(display_task, value, eSetValueWithoutOverwrite);
			ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000));
		}
	}
}