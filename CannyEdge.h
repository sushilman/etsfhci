/*
 * CannyEdge.h
 *
 *  Created on: Dec 21, 2009
 *      Author: sushil
 */
#include "OpencvIncludes.h"
#ifndef CANNYEDGE_H_
#define CANNYEDGE_H_

class CannyEdge {
public:
	CannyEdge();
	IplImage* detectEdge(IplImage* img);
	virtual ~CannyEdge();
};

#endif /* CANNYEDGE_H_ */
