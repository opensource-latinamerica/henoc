#ifndef CVUTILS_H_
#define CVUTILS_H_

#include <v4l2.h>
#include <cv.h>
#include <highgui.h>

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */

typedef enum  {
	VID_CAPTURE_TYPE_V4L2,
	VID_CAPTURE_TYPE_CV,
} VIDCaptureType;

/// Prototype of VIDCapture structure for future use

typedef struct {
	/// Type of capture source. 
	VIDCaptureType type;

	union {
		V4L2Capture *v4l2;
		CvCapture *cv;
	} capture;
	
	VidConv *converter;
	
} VIDCapture;

/// Convert a V4L2Frame to IplImage
void v4l2FrameToIplImage(VidFrame *frame,IplImage *image);

/// Grab a frame and convert to IplImage *.
IplImage * v4l2CaptureQueryIplImage(V4L2Capture *capture,IplImage *buffer);

#ifdef __cplusplus
} /* extern "C" */
#endif /* defined(__cplusplus) */

#endif /*CVUTILS_H_*/
