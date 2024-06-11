#ifndef _COMPONENT_SELECTABLE_
#define _COMPONENT_SELECTABLE_

class Selectable {
public:
	virtual void select() = 0;
	virtual void release() = 0;
};

#endif