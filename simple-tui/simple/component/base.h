#ifndef _COMPONENT_BASE_
#define _COMPONENT_BASE_

#include "../canvas.h"

struct Base {
	virtual const Canvas& render() = 0;
};

#endif