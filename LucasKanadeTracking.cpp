/*
 * LucasKanadeTracking.cpp
 *
 *  Created on: Jan 7, 2010
 *      Author: sushil
 */

#include "LucasKanadeTracking.h"


/*
 * For Lucas Kanade
 */
IplImage *image = 0, *grey = 0, *prev_grey = 0, *pyramid = 0, *prev_pyramid = 0, *swap_temp;

int win_size = 10;
const int MAX_COUNT = 500;
CvPoint2D32f* points[2] = {0,0}, *swap_points;
char* status = 0;
int counter = 0;
int need_to_init = 0;
int night_mode = 0;
int flags = 0;
//int add_remove_pt = 0;
CvPoint pt,ptx1,ptx2,pty1,pty2;//,ptref1,ptref2;
/*
 *
 */
bool isFirstRun=true;
LucasKanadeTracking::LucasKanadeTracking() {
	// TODO Auto-generated constructor stub

}

bool LucasKanadeTracking::track(IplImage* img, CvPoint *center, int *add_remove_pt, bool *isIrisSet){
	//if(pt.x==0)
			pt=*center;
		/* each time +20 and -20 is not gonna help anything..
		 * we must use pointers similarly 'pt' and pass it thru argument
		 *
		 */
		//ptref1=cvPoint(pt.x+20,pt.y);
		//ptref1=cvPoint(pt.x+20,pt.y);

		/*
		 * Adding nearby points to control wandering of the points
		 */
	/*	if(isFirstRun){
			ptx1=cvPoint(pt.x+1,pt.y);
			ptx2=cvPoint(pt.x-1,pt.y);
			pty1=cvPoint(pt.x,pt.y+1);
			pty2=cvPoint(pt.x,pt.y-1);
			isFirstRun=false;
		}else{
			if(abs(pt.x-ptx1.x)>2 || abs(pt.x-ptx2.x)>2 || abs(pt.y-pty1.y)>2 || abs(pt.y-pty2.y)>2){
				pt=cvPoint(points[0][0].x,points[0][0].y);

				ptx1=cvPoint(pt.x+1,pt.y);
				ptx2=cvPoint(pt.x-1,pt.y);
				pty1=cvPoint(pt.x,pt.y+1);
				pty2=cvPoint(pt.x,pt.y-1);
			}
		}*/

		MoveMouse m;

		if(counter==0){
			isIrisSet=false;
			isFirstRun=true;
		}else if(counter>1){
			counter=1;
		}
		////////////////.////////////
		IplImage* frame= cvCloneImage(img);
		int  k, c;

		if( !image ){
			/* allocate all the buffers */
			image = cvCreateImage( cvGetSize(frame), 8, 3 );
			image->origin = frame->origin;
			grey = cvCreateImage( cvGetSize(frame), 8, 1 );
			prev_grey = cvCreateImage( cvGetSize(frame), 8, 1 );
			pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );
			prev_pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );
			points[0] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
			points[1] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
			status = (char*)cvAlloc(MAX_COUNT);
			flags = 0;
		}

		try{
			if(image && frame)
				cvCopy( frame, image, 0 );
		}catch(...){
			cerr<<"Err 1"<<endl;
		}

		cvCvtColor( image, grey, CV_BGR2GRAY );
		//cvEqualizeHist(image,image);
		//cvShowImage("Trackable eye",grey);
		//cvEqualizeHist(grey,grey);
		//cvShowImage("Trackable eyeHist",grey);
		if( night_mode )
			cvZero( image );

		if( need_to_init ){
			/* automatic initialization */
			IplImage* eig = cvCreateImage( cvGetSize(grey), 8, 1 );
			IplImage* temp = cvCreateImage( cvGetSize(grey), 8, 1 );
			double quality = 0.5; // 0.01
			double min_distance = 10;

			counter = MAX_COUNT;
			cvGoodFeaturesToTrack( grey, eig, temp, points[1], &counter, quality, min_distance, 0, 3, 0, 0.04 );

			cvFindCornerSubPix( grey, points[1], counter,
			cvSize(win_size,win_size), cvSize(-1,-1),

			cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
			cvReleaseImage( &eig );
			cvReleaseImage( &temp );

			add_remove_pt = 0;
		}else if( counter > 0 ){
			cvCalcOpticalFlowPyrLK( prev_grey, grey, prev_pyramid, pyramid,
			points[0], points[1], counter, cvSize(win_size,win_size), 6, status, 0,
			cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), flags );

			/* original parameters*/
			//cvCalcOpticalFlowPyrLK( prev_grey, grey, prev_pyramid, pyramid,
			//		points[0], points[1], counter, cvSize(win_size,win_size), 3, status, 0,
			//		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), flags );

			flags |= CV_LKFLOW_PYR_A_READY;
			for(int i = k = 0; i < counter; i++ ){
				if( add_remove_pt )	{
					double dx = pt.x - points[1][i].x;
					double dy = pt.y - points[1][i].y;

					if( dx*dx + dy*dy <= 25 ){
						add_remove_pt = 0;
						continue;
					}
				}

				if( !status[i] )
					continue;

				points[1][k++] = points[1][i];
				//cvCircle( image, cvPointFrom32f(points[1][i]), 3, CV_RGB(0,255,0), -1, 8,0);
				cvCircle( image, cvPointFrom32f(points[1][i]), 3, CV_RGB(0,255,0), -1, 8,0);
				cvLine( image, cvPointFrom32f(points[0][0]), cvPointFrom32f(points[1][0]), CV_RGB(255,0,0), 2 );

				////Calculation of relative(with eyewindow) position tracking point//////////
				// size of whole frame image(ROI)
				int imageHeight=img->height;
				int imageWidth=img->width;
				int prevX=points[0][0].x;
				int prevY=points[0][0].y;
				int currX=points[1][0].x;
				int currY=points[1][0].y;
				int scrx=imageWidth - currX;
				int scry=imageHeight - currY;
				int dx=prevX-currX;
				int dy=prevY-currY;

	/////////Get The color information of point being tracked
	// Some more ideas required !!!
				try{
					CvScalar c;
					c = cvGetAt(img,currY,currX-10); //row i, column j


					//the following are doubles
					//if(isFirstRun){
	//					cout<<"B:"<<c.val[0]; //Blue
	//					cout<<"G:"<<c.val[1]; //Green
	//					cout<<"R:"<<c.val[2]<<endl; //Red
						if(c.val[0]>120 || c.val[1]>120 || c.val[2]>120){
							//cout<<"resetting value"<<endl;
							points[1][0].x=points[0][0].x;
							points[1][0].y=points[0][0].y;
						}
					//}
				}catch(...){
					cerr<<"error"<<endl;
				}
	////////////////////////


				if (dx>50 || dy > 50){
					points[1][0].x=points[0][0].x;
					points[1][0].y=points[0][0].y;
				}

				//int screenX=(abs((scrx*1280/256)));
				int screenX=(abs((scrx*1280/256))) + dx*10;
					screenX=screenX<0?100:screenX;
				//int screenY=0;//abs(scry*4);
				int screenY=(800-abs(scry*4))+dy*15;//abs(scry*4);
					screenY=screenY<0?100:screenY;

				//cout<<"X:"<<screenX<<"Y:"<<screenY<<endl;
				////
				int resX=1280;
				int resY=800;
				CvPoint blocks[5] ={cvPoint(0,0), cvPoint(resX/4,resY/4),cvPoint(resX*3/4,resY/4),
									  cvPoint(resX/4,resY*3/4),cvPoint(resX*3/4,resY*3/4)};
				if(screenX<=resX/2){
					if(screenY<=resY/2){
						screenX=blocks[1].x;
						screenY=blocks[1].y;
					}else{
						screenX=blocks[2].x;
						screenY=blocks[2].y;
					}
				}else{
					if(screenY<=resY/2){
						screenX=blocks[3].x;
						screenY=blocks[3].y;
					}else{
						screenX=blocks[4].x;
						screenY=blocks[4].y;
					}
				}

				m.moveMouse(screenX,screenY);
				//m.moveMouse(screenX,screenY);

			}
			counter = k;
		}

		if( add_remove_pt && counter < MAX_COUNT ){
			points[1][counter++] = cvPointTo32f(pt);

	//		points[1][counter++] = cvPointTo32f(ptref1);
	//		points[1][counter++] = cvPointTo32f(ptref2);
	//
	//		points[1][counter++] = cvPointTo32f(ptx1);
	//		points[1][counter++] = cvPointTo32f(ptx2);
	//		points[1][counter++] = cvPointTo32f(pty1);
	//		points[1][counter++] = cvPointTo32f(pty2);

			cvFindCornerSubPix( grey, points[1] + counter - 1, 1,
			cvSize(win_size,win_size), cvSize(-1,-1),
			cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
			add_remove_pt = 0;
		}

		CV_SWAP( prev_grey, grey, swap_temp );
		CV_SWAP( prev_pyramid, pyramid, swap_temp );
		CV_SWAP( points[0], points[1], swap_points );
		need_to_init = 0;
		cvShowImage( "result", image );

		c = cvWaitKey(10);

		switch( (char) c ){
			case 'r':
				need_to_init = 1;
				break;
			case 'c':
				counter  = 0;
				isIrisSet=false;
				isFirstRun=true;
				break;
			case 'n':
				night_mode ^= 1;
				break;
			default:
				;
			}
		///////////////////////////
		*center=pt;
		cvReleaseImage(&frame);
		return isIrisSet;
}
LucasKanadeTracking::~LucasKanadeTracking() {
	// TODO Auto-generated destructor stub
}
