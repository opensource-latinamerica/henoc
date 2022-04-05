#ifndef FRAME_H_
#define FRAME_H_

#include <sys/time.h>
#include <fourcc.h>

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */

/// Screen Size

typedef struct {
	/// The width 
	int width;
	/// The height
	int height;
} VidSize;

/// Video Frame

typedef struct {
	/// The size of frame
	VidSize size;	

	/// The image type(fourcc) 
	fourcc_t format;
	
	int bytesperline;
	
	/// Excepted image size. 
	int imagesize;
	
	/// The size of data/buffer allocated to hold image. It should be larger than or equal to imagesize
	int bufsize; 
	
	/// Pointer to image data.
	unsigned char *data;
	
	struct timeval	timestamp;
} VidFrame;

/// Allocate and initialize a V4L2Frame structure
VidFrame *vidFrameCreate();

/// Release a V4L2Frame.
void vidFrameRelease(VidFrame **frame);

/// Get the width of a V4L2Frame
int vidFrameGetWidth(VidFrame *frame);

/// Get the height of a V4L2Frame
int vidFrameGetHeight(VidFrame *frame) ;

/// Get the image size of a V4L2Frame
int vidFrameGetImageSize(VidFrame *frame);

/// Get the pointer to image data
unsigned char* vidFrameGetImageData(VidFrame *frame);

/// Resize the buffer length of image data. 
int vidFrameResizeBuffer(VidFrame *frame,int length);

/* Image Format Convertor */

typedef int (*v4l2ConvFunc) (VidFrame *src,VidFrame *dest);

/// Image format converter 

typedef struct {
	/// Name of the converter
	char *name;
	/// The input image format(fourcc)
	fourcc_t input;
	/// The output image format(fourcc))
	fourcc_t output;
	
	/// The callback function to handle the convertion
	v4l2ConvFunc convert;	
} VidConv;

/// Find a converter to convert image format from input to output

VidConv* vidConvFind(fourcc_t input,fourcc_t output);

/// Execute the image converter.

int vidConvProcess(VidConv *conv,VidFrame *src,VidFrame *dest);

#ifdef __cplusplus
} /* extern "C" */
#endif /* defined(__cplusplus) */


#endif /*FRAME_H_*/
