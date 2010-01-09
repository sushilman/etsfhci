/*
 * MoveMouse.cpp
 *
 *  Created on: Nov 27, 2009
 *      Author: sushil
 */

#include "MoveMouse.h"

MoveMouse::MoveMouse() {
	// TODO Auto-generated constructor stub

}

void MoveMouse::moveMouse(int delta_x,int delta_y){
    Display *display = XOpenDisplay(0);
    // move pointer relative to current position
    // move pointer to absolute position
    Window root = DefaultRootWindow(display);
    XWarpPointer(display, None, root, 0, 0, 0, 0, delta_x,delta_y);
    XCloseDisplay(display);
}

MoveMouse::~MoveMouse() {
	// TODO Auto-generated destructor stub
}
