#include <sys/time.h>
#include "cvutils.h"

void v4l2FrameToIplImage(VidFrame *frame,IplImage *image){
		memset(image,0,sizeof(IplImage));
	
	image->nSize = sizeof(IplImage);
	
	image->ID = 0;
	
	image->nChannels = 3; //FIXME. Should implement channel() determination func in GVideoPixel
	
	image->alphaChannel = 0 ; /* ignored by OpenCV */
	
	image->depth=IPL_DEPTH_8U;        //FIXME
						/* pixel depth in bits: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16S,
						IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F are supported */
	//image->colorModel[4]; /* ignored by OpenCV */
	//image->channelSeq[4]; /* ditto */

	image->dataOrder=0;     /* 0 - interleaved color channels, 1 - separate color channels.
						cvCreateImage can only create interleaved images */
	image->origin=0;        /* 0 - top-left origin,
				1 - bottom-left origin (Windows bitmaps style) */
	//image->align;         /* Alignment of image rows (4 or 8).  OpenCV ignores it and uses widthStep instead */
	
	
	image->roi =  0 ;/* image ROI. if NULL, the whole image is selected */
        
    image->maskROI=0; /* must be NULL */
	
	image->imageId = 0;     /* ditto */
    
	image->tileInfo=0; /* ditto */

	image->width = frame->size.width;         /* image width in pixels */
	
	image->height = frame->size.height;        /* image height in pixels */
	
	image->imageSize = frame->imagesize;     /* image data size in bytes
								(==image->height*image->widthStep
								in case of interleaved data)*/
	
	image->imageData =(char*) frame->data; /* pointer to a ligned image data */
	
	//image->widthStep = frame->bytesperline;   /* size of aligned image row in bytes */
	
	image->widthStep = frame->bytesperline;   /* size of aligned image row in bytes */	
	
		//image->BorderMode[4]; /* ignored by OpenCV */
		//image->BorderConst[4]; /* ditto */

	image->imageDataOrigin=0 ; //FIXME
}


/**
 *  @todo Deprecate
 **/ 

IplImage * v4l2CaptureQueryIplImage(V4L2Capture *capture,IplImage *data){
	static VidFrame *tmp=0;
	VidConv *conv;
	VidFrame *input;
	IplImage *output=0;
	
	if (!tmp	) {
		tmp = vidFrameCreate();
	}
	
	conv = vidConvFind(v4l2CaptureGetImageFormat(capture),V4L2_PIX_FMT_BGR24);
	
	if (conv){	
		input = v4l2CaptureQueryFrame(capture);
		vidConvProcess(conv,input,tmp);
		v4l2FrameToIplImage(tmp,data);

		output = data;
	}
	
	return output;
}

