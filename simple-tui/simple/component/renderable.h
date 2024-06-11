#ifndef _COMPONENT_RENDERED_
#define _COMPONENT_RENDERED_

#include "../canvas.h"

class Renderable {
public:
	virtual const Canvas& render() = 0;
};

#endif