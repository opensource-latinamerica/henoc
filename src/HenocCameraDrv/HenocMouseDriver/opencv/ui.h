#ifndef UI_H_
#define UI_H_

#include <gtk/gtk.h>
#include <v4l2.h>

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */

/// Launch a dialog to choose capture source. The source could be a video device or a local multimedia file.  
 
char *capture_source_chooser_run(int *isV4L2Device);

///  Launch a dialog to tune video device parameters.

V4L2Capture *video_tuning_dialog_run(const char *location);

///  Launch a dialog to tune video device parameters and return the assigned setting through arguments 

V4L2Capture *video_tuning_dialog_run_with_props(const char *location,
		int *format,int *width,int *height,int *channel,int *norm,int *fps);

#ifdef __cplusplus
} /* extern "C" */
#endif /* defined(__cplusplus) */




#endif /*UI_H_*/
