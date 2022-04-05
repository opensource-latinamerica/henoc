#include "cv.h"
#include "highgui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

//#include <libv4l2/capture.h>
#include <opencv/cvutils.h>
#include <opencv/ui.h>
#include <X11/extensions/XTest.h>

#ifdef _EiC
#define WIN32
#endif

static CvMemStorage* storage = 0;
static CvHaarClassifierCascade* cascade = 0;

void detect_and_draw( IplImage* image );

int main( int argc, char** argv ){
	gtk_init(&argc,&argv);
	V4L2Capture *capture = 0;
    
	IplImage *frame, *frame_copy = 0;
	const char* input_name;

        storage = cvCreateMemStorage(0);
    
    	int isVideoDevice;
    	char* source = capture_source_chooser_run(&isVideoDevice);
    	if (source){
    		if (isVideoDevice){
    			capture = video_tuning_dialog_run(source);
    		}
    		free(source);
    	}
    
	cvNamedWindow( "result", 1 );

	if( capture ){
		v4l2CaptureStartStreaming(capture,0,4);
		for(;;){
			IplImage buffer;
			frame = v4l2CaptureQueryIplImage(capture,&buffer);
			if( !frame )
				break;
			if( !frame_copy )
				frame_copy = cvCreateImage( cvSize(frame->width,frame->height), IPL_DEPTH_8U, frame->nChannels );
			if( frame->origin == IPL_ORIGIN_TL )
				cvCopy( frame, frame_copy, 0 );
			else
				cvFlip( frame, frame_copy, 0 );
			detect_and_draw( frame_copy );
			if( cvWaitKey( 10 ) >= 0 )
				break;
		}
		cvReleaseImage( &frame_copy );
		v4l2CaptureRelease(&capture);
	}
	else{
		const char* filename = input_name ? input_name : (char*)"lena.jpg";
		IplImage* image = cvLoadImage( filename, 1 );

		if( image ){
			detect_and_draw( image );
			cvWaitKey(0);
			cvReleaseImage( &image );
        	}
		else{
			FILE* f = fopen( filename, "rt" );
			if( f ){
				char buf[1000+1];
				while( fgets( buf, 1000, f ) ){
					int len = (int)strlen(buf);
					while( len > 0 && isspace(buf[len-1]) )
						len--;
					buf[len] = '\0';
					image = cvLoadImage( buf, 1 );
					if( image ){
						detect_and_draw( image );
						cvWaitKey(0);
						cvReleaseImage( &image );
					}
				}
				fclose(f);
			}
		}
	}
    
	cvDestroyWindow("result");
	return 0;
}

void detect_and_draw( IplImage* img ){
	static CvScalar colors[] = 
	{
		{{0,0,255}},
		{{0,128,255}},
		{{0,255,255}},
		{{0,255,0}},
		{{255,128,0}},
		{{255,255,0}},
		{{255,0,0}},
		{{255,0,255}}
	};

	CvMat* HomographyMatrix = NULL;
	HomographyMatrix = cvCreateMat( 3, 3, CV_32FC1);
	HomographyMatrix->data.fl[0]=1.687285;
	HomographyMatrix->data.fl[1]=0.379639;
	HomographyMatrix->data.fl[2]=-269.754669;
	HomographyMatrix->data.fl[3]=-0.048233;
	HomographyMatrix->data.fl[4]=2.214323;
	HomographyMatrix->data.fl[5]=-248.183975;
	HomographyMatrix->data.fl[6]=-0.000076;
	HomographyMatrix->data.fl[7]=0.000945;
	HomographyMatrix->data.fl[8]=1.000000;

	IplImage* salida=cvCreateImage(cvGetSize(img),8,3);
        cvWarpPerspective(img, salida, HomographyMatrix, CV_WARP_FILL_OUTLIERS,  cvScalarAll(0));
	CvPoint center;
	int i;
	int j;
	int radius;
	int r=0;
	int g=0;
	int b=0;
	int fao=0;
	for(i=20; i < 640; i++)
		for(j=0; j<800; j++){
			r = ((uchar*)(salida->imageData + salida->widthStep*i))[j*3+2];
			g = ((uchar*)(salida->imageData + salida->widthStep*i))[j*3+1];
			b = ((uchar*)(salida->imageData + salida->widthStep*i))[j*3];
			if(  ( (r > 150) && (r < 230) ) &&  ( (b > 150) && (b < 230) )  && (g > 220) ){
			//if(  ( (r > 150) && (r < 200) ) &&  ( (b > 150) && (b < 200) )  && (g > 220) ){
				//fao = 1;
				//printf("lo encontre\n");
			//}
			//if( ((r+g+b) >  680)  && fao ){ 
				center.x = cvRound(j);
				center.y = cvRound(i);
				radius = cvRound( 20 );
				cvCircle( salida, center, radius, colors[1], 3, 8, 0 );
				
				Display *display = XOpenDisplay(NULL);
				int screen = DefaultScreen( display );
				XTestFakeMotionEvent( display, screen, center.x, center.y, 0 );
				XTestFakeButtonEvent( display, 1, True, 0 );
				XCloseDisplay( display );
				fao = 1;
				goto pinta;
			}
		}

	pinta:
		//cvShowImage( "result", img );
	if(!fao){
		Display *display = XOpenDisplay(NULL);
		XTestFakeButtonEvent( display, 1, False, 0 );
		XCloseDisplay( display );
	}
	
	cvReleaseImage( &salida );
	cvReleaseMat( &HomographyMatrix );
}
