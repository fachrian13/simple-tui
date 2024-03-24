#ifndef _COMPONENT_BASE_
#define _COMPONENT_BASE_

#include "../canvas.h"

class Base {
public:
	virtual const Canvas& render() = 0;
};

#endif