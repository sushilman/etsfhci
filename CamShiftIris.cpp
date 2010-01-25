
#include "CamShiftIris.h"

IplImage *hsv1 = 0, *h=0,*s=0,*v=0, *hue1 = 0, *mask1 = 0, *backproject1 = 0, *histimg1 = 0;
CvHistogram *hist1= 0;

int backproject1_mode = 0;
int select_object1 = 0;
int track_object1 = 0;
int show_hist1= 1;
CvPoint origin1;
//CvRect selection;
CvRect track_window1;
CvBox2D track_box1;
CvConnectedComp track_comp1;
int hdims1 = 16;
float hranges_arr1[] = {0,180};
float* hranges1 = hranges_arr1;
//int vmin = 10, vmax1 = 256, smin1 = 30;

//vmax1=100 when bright light
/*
 * Bright light condition
 * 100 for iris, ?? for eye
 */
/*
 * Under Light condition
 */
// 50 for iris, 80 for eye
int vmin1 = 0, vmax1 = 35, smin1 = 0;

float X1=0, Y1=0;
float angle1=0;

void CamShiftIris::setVmax(int value){
	vmax1=value;
}

CvScalar CamShiftIris::hsvrgb( float hue1 )
{
    int rgb[3], p, sector;
    static const int sector_data[][3]=
        {{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}};
    hue1 *= 0.033333333333333333333333333333333f;
    sector = cvFloor(hue1);
    p = cvRound(255*(hue1 - sector));
    p ^= sector & 1 ? 255 : 0;

    rgb[sector_data[sector][0]] = 255;
    rgb[sector_data[sector][1]] = 0;
    rgb[sector_data[sector][2]] = p;

    return cvScalar(rgb[2], rgb[1], rgb[0],0);
}

CvBox2D CamShiftIris::track( IplImage* image, CvRect selection, bool isIris){
	CamShiftIris camshift;
	select_object1=1;
	track_object1=-1;
	origin1=cvPoint(0,0);

///////////////////////////////

	int i, bin_w, c;
	//frame = cvQueryFrame( capture );

//
//	frame=cvCloneImage(image);
//        if( !frame )
//            return 0;
	if( image ){
		/* allocate all the buffers */
//		image = cvCreateImage( cvGetSize(frame), 8, 3 );
//		image->origin = frame->origin;
		hsv1 = cvCreateImage( cvGetSize(image), 8, 3 );
		h = cvCreateImage( cvGetSize(image), 8, 1 );
		s = cvCreateImage( cvGetSize(image), 8, 1 );
		v = cvCreateImage( cvGetSize(image), 8, 1);
		hue1 = cvCreateImage( cvGetSize(image), 8, 1 );
		mask1 = cvCreateImage( cvGetSize(image), 8, 1 );
		backproject1 = cvCreateImage( cvGetSize(image), 8, 1 );
		hist1= cvCreateHist( 1, &hdims1, CV_HIST_ARRAY, &hranges1, 1 );
		histimg1 = cvCreateImage( cvSize(320,200), 8, 3 );
		cvZero( histimg1 );
	}
	cvCvtColor( image, hsv1, CV_BGR2HSV );
	///////////////////Equalize v in hsv///////////
	cvSplit( hsv1, h, s, v, 0 );
	cvEqualizeHist(v,v);
	cvMerge(h,s,v,0,hsv1);
	///////////////////Equalize v in hsv///////////

	if( track_object1 !=0 ){
		int _vmin1 = vmin1, _vmax1 = vmax1;

		cvInRangeS( hsv1, cvScalar(0,smin1,MIN(_vmin1,_vmax1),0),
					cvScalar(180,256,MAX(_vmin1,_vmax1),0), mask1 );
		cvSplit( hsv1, hue1, 0, 0, 0 );

		if( track_object1 < 0 ){
			float max_val = 0.f;
			cvSetImageROI( hue1, selection );
			cvSetImageROI( mask1, selection );
			cvCalcHist( &hue1, hist1, 0, mask1 );
			cvGetMinMaxHistValue( hist1, 0, &max_val, 0, 0 );
			cvConvertScale( hist1->bins, hist1->bins, max_val ? 255. / max_val : 0., 0 );
			cvResetImageROI( hue1 );
			cvResetImageROI( mask1 );
			track_window1 = selection;
			track_object1 = 1;

			cvZero( histimg1 );
			bin_w = histimg1->width / hdims1;
			for( i = 0; i < hdims1; i++ )
			{
				int val = cvRound( cvGetReal1D(hist1->bins,i)*histimg1->height/255 );
				CvScalar color = camshift.hsvrgb(i*180.f/hdims1);
				cvRectangle( histimg1, cvPoint(i*bin_w,histimg1->height),
							 cvPoint((i+1)*bin_w,histimg1->height - val),
							 color, -1, 8, 0 );
			}
		}
		cvCalcBackProject( &hue1, backproject1, hist1);
		cvAnd( backproject1, mask1, backproject1, 0 );
		try{
		cvCamShift( backproject1, track_window1,
					cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),
					&track_comp1, &track_box1 );
		}catch(...){
			cvReleaseImage(&hsv1);
			cvReleaseImage(&h);
			cvReleaseImage(&s);
			cvReleaseImage(&v);
			cvReleaseImage(&hue1);
			cvReleaseImage(&mask1);
			cvReleaseImage(&backproject1);
			cvReleaseHist(&hist1);
			cvReleaseImage(&histimg1);
		}
		track_window1 = track_comp1.rect;
		if( backproject1_mode )
			cvCvtColor( backproject1, image, CV_GRAY2BGR );
		if( !image->origin )
			track_box1.angle = -track_box1.angle;
		if(isIris)
			cvEllipseBox( image, track_box1, CV_RGB(255,0,0), 3, CV_AA, 0 );
	}
	cvShowImage( "CamShift Tracking", image );
	//cvShowImage( "Histogram", histimg1 );

//	c = cvWaitKey(10);
//	if( (char) c == 27 )
//		cout<<"esc pressed";
//		//return; //break;
//	switch( (char) c ){
//	case 'b':
//		backproject1_mode ^= 1;
//		break;
//	case 'c':
//		track_object1 = 0;
//		cvZero( histimg1 );
//		break;
//	case 'h':
//		show_hist1^= 1;
//		if( !show_hist1)
//			cvDestroyWindow( "Histogram" );
//		else
//			cvNamedWindow( "Histogram", 1 );
//		break;
//	default:
//		;
//	}

	//cvReleaseImage(&image);
	cvReleaseImage(&hsv1);
	cvReleaseImage(&h);
	cvReleaseImage(&s);
	cvReleaseImage(&v);
	cvReleaseImage(&hue1);
	cvReleaseImage(&mask1);
	cvReleaseImage(&backproject1);
	cvReleaseHist(&hist1);
	cvReleaseImage(&histimg1);

    return track_box1;
}

CamShiftIris::CamShiftIris() {
	// TODO Auto-generated destructor stub
}

CamShiftIris::~CamShiftIris() {
	// TODO Auto-generated destructor stub
}
