/*
 * main.cpp
 *
 *  Created on: Nov 25, 2009
 *      Author: sushil
 */

#include "main.h"

CvHaarClassifierCascade* cascade;
const char* cascade_name =
    "haarcascade_profileface.xml";

//static CvMemStorage* storage = 0;

bool isIrisSet=false;
int add_remove_pt;

EyeExtract eyeExtract;
EyeDetect eyeDetect;
HoughCircle houghcircle;
EyeTracking eyetracking;
CvPoint houghCenter;
float houghRadius;
CannyEdge cannyEdge;

int main( int argc, char** argv ){
	try{
		initialize(argc,argv);
	}catch(BAD_ARGUMENT){
		return -1;
	}

    //cvReleaseImage( &image );
    cvDestroyWindow("result");
}


void initialize(int argc, char** argv ){
	IplImage* image;
    // Structure for getting video from camera or avi
    CvCapture* capture = 0;

    // Images to capture the frame from video or camera or from file
    IplImage *frame, *frame_copy = 0;

    // Used for calculations
    int optlen = strlen("--cascade=");

    // Input file name for avi or image file.
    const char* input_name;

    // Check for the correct usage of the command line
    if( argc > 1 && strncmp( argv[1], "--cascade=", optlen ) == 0 ){
        cascade_name = argv[1] + optlen;
        input_name = argc > 2 ? argv[2] : 0;
    }else{
    	cerr<< cascade_name << " Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n"<<endl;
    	throw BAD_ARGUMENT();;
    }

    // Load the HaarClassifierCascade
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
    // Check whether the cascade has loaded successfully. Else report and error and quit
    if( !cascade ){
        cerr<<"ERROR: Could not load classifier cascade\n"<<endl;
        throw BAD_ARGUMENT();
    }

    // Allocate the memory storage
   // storage = cvCreateMemStorage(0);

    // Find whether to detect the object from file or from camera.
    if( !input_name || (isdigit(input_name[0]) && input_name[1] == '\0') )
        capture = cvCaptureFromCAM( !input_name ? 0 : input_name[0] - '0' );
    else
        capture = cvCaptureFromAVI( input_name );
    cvWaitKey(2000);
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 256);
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 256);
    // Create a new named window with title: result
    cvNamedWindow( "result", 1 );

    // Find if the capture is loaded successfully or not.
    // If loaded succesfully, then:
    if( capture ){
        // Capture from the camera.
        for(;;){
            // Capture the frame and load it in IplImage
            if( !cvGrabFrame( capture ))
                break;

            frame = cvRetrieveFrame( capture );

            // If the frame does not exist, quit the loop
            if( !frame )
                break;

            // Allocate framecopy as the same size of the frame
            //if( !frame_copy )
                frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );

            // Check the origin of image. If top left, copy the image frame to frame_copy.
            if( frame->origin == IPL_ORIGIN_TL )
                cvCopy( frame, frame_copy, 0 );
            // Else flip and copy the image
            else
            	cvCopy( frame, frame_copy, 0 );

            // Call the function to detect and draw the face
            //detect_and_draw( frame_copy );
            extractFace(frame_copy,cascade);
            //cvShowImage("Result",frame_copy);
            // Wait for a while before proceeding to the next frame
            if( cvWaitKey( 50 ) >= 0 )
                break;
        }

        // Release the images, and capture memory
        cvReleaseImage( &frame_copy );
        cvReleaseCapture( &capture );
    }

    // If the capture is not loaded successfully, then:
    else{
        // Assume the image to be lena.jpg, or the input_name specified
        const char* filename = input_name ? input_name : (char*)"lena.jpg";

        // Load the image from that filename
        image = cvLoadImage( filename, 1 );

        // If Image is loaded successfully, then:
        if( image ){
            // Detect and draw the face
            //detect_and_draw( image );
        	extractFace(image,cascade);
        	//cvShowImage("result",image);
            // Wait for user input
            cvWaitKey(0);

            // Release the image memory
           // cvReleaseImage( &image );
        }
        else{
            /* assume it is a text file containing the
               list of the image filenames to be processed - one per line */
            FILE* f = fopen( filename, "rt" );
            if( f ){
                char buf[1000+1];

                // Get the line from the file
                while(fgets( buf, 1000, f )){

                    // Remove the spaces if any, and clean up the name
                    int len = (int)strlen(buf);
                    while( len > 0 && isspace(buf[len-1]) )
                        len--;
                    buf[len] = '\0';

                    // Load the image from the filename present in the buffer
                    image = cvLoadImage( buf, 1 );

                    // If the image was loaded successfully, then:
                    if( image ){
                        // Detect and draw the face from the image
                        //detect_and_draw( image );
						//return image;
                    	//cvShowImage("result",image);
                    	extractFace(image,cascade);
                        // Wait for the user input, and release the memory
                        //cvWaitKey(0);
                        //cvReleaseImage( &image );
                    }
                }
                // Close then file
                fclose(f);
            }
        }
    }
    // Destroy the window previously created with filename: "result"
    cvDestroyWindow("result");
}

void extractFace(IplImage* img, CvHaarClassifierCascade* cascade){
	FaceDetect facedetect;
	bool isEyeDetected=false;
	facedetect.setHaarClassifier(cascade);
	cout<<"Assertion test 0.1";
	facedetect.extractFace(img);
	if((img)){ // Face is extracted here
		cvShowImage("Face",img);
		//cvWaitKey(0);
		//extracting eye, this will return roughly estimated eye region
		img=eyeExtract.extract(img);
		if(!img){
			//cout<<"NO face No eye";
			cvReleaseImage(&img);
			return;
		}
		cout<<"\nAssertion test 0.2";
		cvShowImage("eyeExtract",img);
//		cvWaitKey(0);


		/*
		 * tODO:Eye corner detection here
		 */
	//	cvWaitKey(0);
		//eyetracking.trackLucasKanade(img, &houghCenter, &add_remove_pt, &isIrisSet);

//
		char* cascade_name_eye="/opt/opencv/share/opencv/haarcascades/haarcascade_mcs_righteye.xml";
//		//char* cascade_name_eye="/opt/opencv/share/opencv/haarcascades/haarcascade_eye.xml";
//		//char* cascade_name_eye="/opt/opencv/share/opencv/haarcascades/haarcascade_righteye_2splits.xml";
//
//		/**
//		 * This is wrong??.. there is sth in img right from the start
//		 */
//
//		eyeDetect.setHaarClassifier(cascade_name_eye);
//		if((img=eyeDetect.extractEye(img))){
//			cvShowImage("TheEye",img);
//			isEyeDetected=true;
//		}else{
//			isEyeDetected=false;
//		}
//
//		cout<<"Assertion test 1";
//		//apply hough here




		///TEMPORARY substitution for eyeDetect haarclassifier
		isEyeDetected=true;
		//// REMOVE THIS




		if(!isIrisSet && isEyeDetected){
			//houghcircle.drawHocvCopyughCircles(img, 100, 30, 100, 20,50);
			cout<<"Assertion test 2";
			houghcircle.drawHoughCircles(img, 100, 30, 100, 25, 50);
			if(houghcircle.isHoughDetected()){
				cout<<"Assertion test 3";
				isIrisSet=true;
				add_remove_pt=1; // add a point for tracking
			}else{
				cout<<"Assertion test 4";
				isIrisSet=false;
				//add_remove_pt=0;
			}

			houghCenter=houghcircle.getCenter();
			houghRadius=houghcircle.getRadius();
			cout<<"Assertion test 5";
			cvShowImage("Hough Eye",img);
			cout<<"Assertion test 5.1";
			//get center of hough circle
			//pt=cvPoint(p[0],p[1]);
		}
		if(isIrisSet && isEyeDetected){ //if Iris is set
			cout<<"Assertion test 6";
			//isIrisSet=false;
			try{
				//isIrisSet=eyetracking.trackLucasKanade(img, &houghCenter, &add_remove_pt, &isIrisSet);
				isIrisSet=eyetracking.trackCamShift(img,&houghCenter,houghRadius);
			}catch(...){
				cerr<<"Error something is wrong"<<endl;
			}
			//cout<<"Is iris set? = "<<isIrisSet<<endl;
			//Track the point here
		}

	}
	cout<<"Assertion test 7";
	cvReleaseImage(&img);
}
