/*
 * LucasKanadeTracking.h
 *
 *  Created on: Jan 7, 2010
 *      Author: sushil
 */

#include "OpencvIncludes.h"
#include "MoveMouse.h"

#ifndef LUCASKANADETRACKING_H_
#define LUCASKANADETRACKING_H_


class LucasKanadeTracking {
public:
	LucasKanadeTracking();
	bool track(IplImage* img, CvPoint *center, int *add_remove_pt, bool *isIrisSet);
	virtual ~LucasKanadeTracking();
};

#endif /* LUCASKANADETRACKING_H_ */
