#ifndef V4L_H_
#define V4L_H_

#include <linux/ioctl.h>
#include <linux/videodev.h>
#include <linux/videodev2.h>
#include <linux/types.h>

#include <fourcc.h>
#include <frame.h>

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */

/// Video capturing structure

typedef struct {
	int fd;
	
	/* General Information */
	
	/// Device file (e.g /dev/video0)
	char *location;
	
	/// Name of the device
	char *name;
	
	///  The driver of the V4L device
	char *driver;
	
	/// The data bus;
	char *bus;
	
	/// The capabilities
	unsigned int capabilities;
	
	/// no. of channels
	int nChannels;
	
	/// An NULL terminated array of channel name 
	char **channel_list;
	
	/// no. of norms
	int nNorms;
		
	/// An NULL terminated array of norm name
	char **norm_list;

	/// An array of Video Stand for TV Norm. The size of this array is same as norm_list     
	v4l2_std_id* std_list;	

	/// An NULL terminated array to hold supported fourcc code. 
	int *imageformat_list;

	/// no. of support buffer_types;
	int buffer_types;
	/// supported buffer types
	int buffer_type[V4L2_BUF_TYPE_PRIVATE];
	
	/* Run-time Information */

	/// The current I/O moade = {V4L2_CAP_READWRITE(default) | V4L2_CAP_STREAMING}
	int iomode; 

	/// Burst mode 
	int burst_mode;
	
	/// The current input frame's pixel format in fourcc code (little endian) 
	int format;
	
	int channel;
	
	int norm;
	
	double fps;
	
	VidSize resolution;
			
	/// The buffer size of input frame. Assigned by v4l_dev_open/v4l_dev_get_image_format.
	/** If it is set to 0 ,that means it is compressed format. Negative
	 * value indicate error (probably that the image format is not supported) 
	 */
	int bufsize;
	
	int bytesperline;
	
	/// Turn on logging
	int log;
	
	/* Buffers */
	
	/// Buffer to reading frame
	VidFrame *framesbuffer;
	
	/// no. of frames for buffer
	int frames;	
		
	/// Index to current frames buffer
	int curr_frame_idx;
	
} V4L2Capture;


/////////////////////////
/* Basic Operatiosn */
/////////////////////////

/// Initializes capturing video from V4L2 device 
V4L2Capture* v4l2CaptureOpen(const char *location);

/// Read a frame from device
VidFrame* v4l2CaptureQueryFrame(V4L2Capture*);

/// Start streaming mode
int v4l2CaptureStartStreaming(V4L2Capture *capture,int burst,int nBuffer);

/// Stop streaming mode
int v4l2CaputreStopStreaming(V4L2Capture *capture);

/// Close and release the data allocated
void v4l2CaptureRelease(V4L2Capture**);

void v4l2CaptureSetLog(V4L2Capture *capture,int state);

//////////////////////////////////////////////
/* Query and set properties functions */
//////////////////////////////////////////////

const char* v4l2CaputreGetName(V4L2Capture *capture);
const char* v4l2CaputreGetLocation(V4L2Capture *capture);
const char* v4l2CaptureGetDriver(V4L2Capture *capture);

/// Query the supported frame size for current image format
/**
 *   sizes: Output-variable to hold a newly created 
 * null-terminated array to hold the no. of support 
 * frame size. It must be released by user.
 * 
 *  Return: no. of available frame size. Negative value to indicate
 * error.
 */
int v4l2CaptureQueryResolutionsList(V4L2Capture *,VidSize **sizes);

/* Setters / Getters */

/// Return: Fourcc code of current image format
int v4l2CaptureGetImageFormat(V4L2Capture *capture);

/// Set the image format and size
int v4l2CaptureSetImageFormat(V4L2Capture *capture,fourcc_t fourcc,VidSize *size);

/// Auto-set the image format which could be converted to assigned output format. 
int v4l2CaptureAutoSetImageFormat(V4L2Capture *capture,fourcc_t output);

int v4l2CaptureGetResolution(V4L2Capture *capture,VidSize *size); 
int v4l2CaptureSetResolution(V4L2Capture *capture,VidSize *size);

double v4l2CaptureGetFPS(V4L2Capture *capture);
int v4l2CaptureSetFPS(V4L2Capture *capture,int fps);

char** v4l2CaptureGetChannelsList(V4L2Capture *capture);
char** v4l2CaptureGetNormsList(V4L2Capture *capture);
int* v4l2CaptureGetImageFormatsList(V4L2Capture *capture);

int v4l2CaptureSetChannel(V4L2Capture *,int index);
int v4l2CaptureGetChannel(V4L2Capture *);

int v4l2CaptureSetNorm(V4L2Capture *,int index);
int v4l2CaptureGetNorm(V4L2Capture *);

#ifdef __cplusplus
} /* extern "C" */
#endif /* defined(__cplusplus) */



#endif /*V4L_H_*/
