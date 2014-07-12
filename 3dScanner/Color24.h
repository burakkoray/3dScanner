#ifndef COLOR24_H
#define COLOR24_H

#include <stdint.h>

class Color24{
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;

	Color24(){}

	Color24(uint8_t r, uint8_t g, uint8_t b){
		this->r= r;
		this->g= g;
		this->b= b;
	}
};

#endif
