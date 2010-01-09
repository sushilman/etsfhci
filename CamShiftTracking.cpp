/*
 * CamShiftTracking.cpp
 *
 *  Created on: Jan 6, 2010
 *      Author: sushil
 */

/**
 * TODO:
 * main function returns only cvBox2D from which we can extract x,y,angle,size
 *
 * TODO:
 * make a separate function for calculation of x,y coordinates
 * this functions should consider angles and sizes as well
 * look into the function cvEllipseBox(..) to get the idea how ellipse is created
 * This function should be in class that calls this object
 */

#include "CamShiftTracking.h"

IplImage *hsv = 0, *hue = 0, *mask = 0, *backproject = 0, *histimg = 0;
CvHistogram *hist = 0;

int backproject_mode = 0;
int select_object = 0;
int track_object = 0;
int show_hist = 1;
CvPoint origin;
//CvRect selection;
CvRect track_window;
CvBox2D track_box;
CvConnectedComp track_comp;
int hdims = 16;
float hranges_arr[] = {0,180};
float* hranges = hranges_arr;
//int vmin = 10, vmax = 256, smin = 30;

//vmax=100 when bright light
/*
 * Bright light condition
 * 100 for iris, ?? for eye
 */
/*
 * Under Light condition
 */
// 50 for iris, 95 for eye
int vmin = 0, vmax = 100, smin = 0;

float X=0, Y=0;
float angle=0;


CvScalar CamShiftTracking::hsv2rgb( float hue )
{
    int rgb[3], p, sector;
    static const int sector_data[][3]=
        {{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}};
    hue *= 0.033333333333333333333333333333333f;
    sector = cvFloor(hue);
    p = cvRound(255*(hue - sector));
    p ^= sector & 1 ? 255 : 0;

    rgb[sector_data[sector][0]] = 255;
    rgb[sector_data[sector][1]] = 0;
    rgb[sector_data[sector][2]] = p;

    return cvScalar(rgb[2], rgb[1], rgb[0],0);
}

CvBox2D CamShiftTracking::track( IplImage* image, CvRect selection){
	CamShiftTracking camshift;
	select_object=1;
	track_object=-1;
	origin=cvPoint(0,0);

///////////////////////////////

	int i, bin_w, c;
	//frame = cvQueryFrame( capture );

//
//	frame=cvCloneImage(image);
//        if( !frame )
//            return 0;
        cout<<"Test1"<<endl;
	if( image ){
		/* allocate all the buffers */
//		image = cvCreateImage( cvGetSize(frame), 8, 3 );
//		image->origin = frame->origin;
		hsv = cvCreateImage( cvGetSize(image), 8, 3 );
		hue = cvCreateImage( cvGetSize(image), 8, 1 );
		mask = cvCreateImage( cvGetSize(image), 8, 1 );
		backproject = cvCreateImage( cvGetSize(image), 8, 1 );
		hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );
		histimg = cvCreateImage( cvSize(320,200), 8, 3 );
		cvZero( histimg );
		cout<<"Test1.1"<<endl;
	}
	//cvCopy( frame, image, 0 );
	cout<<"Test2"<<endl;
	cvCvtColor( image, hsv, CV_BGR2HSV );
	cout<<"Test3"<<endl;
	if( track_object !=0 ){
		int _vmin = vmin, _vmax = vmax;

		cvInRangeS( hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),
					cvScalar(180,256,MAX(_vmin,_vmax),0), mask );
		cvSplit( hsv, hue, 0, 0, 0 );

		if( track_object < 0 )
		{
			float max_val = 0.f;
			cvSetImageROI( hue, selection );
			cvSetImageROI( mask, selection );
			cvCalcHist( &hue, hist, 0, mask );
			cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );
			cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 );
			cvResetImageROI( hue );
			cvResetImageROI( mask );
			track_window = selection;
			track_object = 1;

			cvZero( histimg );
			bin_w = histimg->width / hdims;
			for( i = 0; i < hdims; i++ )
			{
				int val = cvRound( cvGetReal1D(hist->bins,i)*histimg->height/255 );
				CvScalar color = camshift.hsv2rgb(i*180.f/hdims);
				cvRectangle( histimg, cvPoint(i*bin_w,histimg->height),
							 cvPoint((i+1)*bin_w,histimg->height - val),
							 color, -1, 8, 0 );
			}
		}
		cout<<"Test4"<<endl;
		cvCalcBackProject( &hue, backproject, hist );
		cvAnd( backproject, mask, backproject, 0 );
		cvCamShift( backproject, track_window,
					cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),
					&track_comp, &track_box );
		track_window = track_comp.rect;
		cout<<"Test5"<<endl;
		if( backproject_mode )
			cvCvtColor( backproject, image, CV_GRAY2BGR );
		if( !image->origin )
			track_box.angle = -track_box.angle;


//	cout<<"x:"<<track_box.center.x<< "y:"<<track_box.center.y<<endl;


	cvEllipseBox( image, track_box, CV_RGB(255,0,0), 1, CV_AA, 0 );
	}
	cout<<"Test6"<<endl;
//	if( select_object && selection.width > 0 && selection.height > 0 ){
//		cvSetImageROI( image, selection );
//		cvXorS( image, cvScalarAll(255), image, 0 );
//		cvResetImageROI( image );
//	}
	cout<<"Test7"<<endl;
	cvShowImage( "CamShiftDemo", image );
	cvShowImage( "Histogram", histimg );

	c = cvWaitKey(10);
	if( (char) c == 27 )
		cout<<"c pressed";
		//return; //break;
	switch( (char) c ){
	case 'b':
		backproject_mode ^= 1;
		break;
	case 'c':
		track_object = 0;
		cvZero( histimg );
		break;
	case 'h':
		show_hist ^= 1;
		if( !show_hist )
			cvDestroyWindow( "Histogram" );
		else
			cvNamedWindow( "Histogram", 1 );
		break;
	default:
		;
	}

	//cvReleaseImage(&image);
	cvReleaseImage(&hsv);
	cvReleaseImage(&hue);
	cvReleaseImage(&mask);
	cvReleaseImage(&backproject);
	cvReleaseHist(&hist);
	cvReleaseImage(&histimg);

    return track_box;
}

CamShiftTracking::CamShiftTracking() {
	// TODO Auto-generated destructor stub
}

CamShiftTracking::~CamShiftTracking() {
	// TODO Auto-generated destructor stub
}
