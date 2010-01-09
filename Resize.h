/*
 * Resize.h
 *
 *  Created on: Nov 25, 2009
 *      Author: sushil
 */
#include "OpencvIncludes.h"
#ifndef RESIZE_H_
#define RESIZE_H_

class Resize {
private:
	void resize(IplImage* originalImage, IplImage* resizedImage, int width, int height, int depth, int channels);
public:
	Resize();
	void resize(IplImage* img, int width, int height);
	void resize(IplImage* img, int width, int height, int depth, int channels);
	void resize_IntoNew(IplImage* originalImage, IplImage* resizedImage, int width, int height, int depth, int channels);

	virtual ~Resize();
};

#endif /* RESIZE_H_ */
