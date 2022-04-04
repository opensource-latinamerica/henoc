//g++ GeneraHomografia.cpp -o generaHomografia -I/usr/include/opencv -L/usr/local/lib/ -lm -lcv -lhighgui -lcvaux  -lcxcore

#include <cxcore.h>
#include <highgui.h>
#include <math.h>
#include <cv.h>
#include <stdio.h>

int main( int argc, char** argv ) {
	CvPoint center;
	double scale=-3;
	IplImage* image = argc==2 ? cvLoadImage(argv[1]) : 0;
	if(!image) return -1;
	IplImage* med1=cvCreateImage(cvGetSize(image),8,3);
	CvMat* ImgPoints = NULL;
	CvMat* Points = NULL;
	CvMat* HomographyMatrix = NULL;

	ImgPoints = cvCreateMat( 4, 2, CV_32FC1);
	ImgPoints->data.fl[0]=0;
	ImgPoints->data.fl[1]=0;
	ImgPoints->data.fl[2]=800;
	ImgPoints->data.fl[3]=0;
	ImgPoints->data.fl[4]=800;
	ImgPoints->data.fl[5]=600;
	ImgPoints->data.fl[6]=0;
	ImgPoints->data.fl[7]=600;

	Points = cvCreateMat( 4, 2, CV_32FC1);
	Points->data.fl[0]=134;
	Points->data.fl[1]=115;
	Points->data.fl[2]=639;
	Points->data.fl[3]=126;
	Points->data.fl[4]=723;
	Points->data.fl[5]=516;
	Points->data.fl[6]=44;
	Points->data.fl[7]=515;

	HomographyMatrix = cvCreateMat( 3, 3, CV_32FC1);
	cvFindHomography(Points, ImgPoints, HomographyMatrix);

	for (int i=0;i<3;i++)
		printf("%f %f %f \n",HomographyMatrix->data.fl[i*3],HomographyMatrix->data.fl[i*3+1],HomographyMatrix->data.fl[i*3+2]);
  
	cvWarpPerspective(image, med1, HomographyMatrix, CV_WARP_FILL_OUTLIERS,  cvScalarAll(0));
	cvNamedWindow( "test", 1 );
	cvShowImage( "test", med1 );
	cvWaitKey();
	return 0; 
}

