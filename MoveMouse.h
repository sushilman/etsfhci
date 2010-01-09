/*
 * MoveMouse.h
 *
 *  Created on: Nov 27, 2009
 *      Author: sushil
 */

#ifndef MOVEMOUSE_H_
#define MOVEMOUSE_H_
#include <X11/Xlib.h>
class MoveMouse {
public:
	MoveMouse();
	void moveMouse(int delta_x,int delta_y);
	virtual ~MoveMouse();
};

#endif /* MOVEMOUSE_H_ */
