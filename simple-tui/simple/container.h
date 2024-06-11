#ifndef _CONTAINER_
#define _CONTAINER_

#include "component/selectable.h"
#include <conio.h>

class Container {
public:
	Container(std::initializer_list<Selectable*> components) :
		components(components) {
		self.components.at(self.currentComponent)->select();
	}

	void readInput() {
		char ch = _getch();

		switch (ch) {
		case 'j':
			if (self.currentComponent < self.components.size() - 1) {
				self.components.at(self.currentComponent)->release();
				++self.currentComponent;
				self.components.at(self.currentComponent)->select();
			}
			break;
		case 'k':
			if (self.currentComponent > 0) {
				self.components.at(self.currentComponent)->release();
				--self.currentComponent;
				self.components.at(self.currentComponent)->select();
			}
			break;
		}
	}

private:
	szt currentComponent = 0;
	std::vector<Selectable*> components;
};

#endif