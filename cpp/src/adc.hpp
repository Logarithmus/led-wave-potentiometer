#ifndef MY_ADC_HPP
#define MY_ADC_HPP

#include <stdint.h>

namespace adc {
	uint8_t get();
	void task(void*);
}

#endif