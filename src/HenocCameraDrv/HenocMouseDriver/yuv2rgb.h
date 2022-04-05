#ifndef __YUV2RGB_H_
#define __YUV2RGB_H_

#include <rvtk/frame.h>

int yuv420_to_rgb24(VidFrame *src,VidFrame *dest);
int yuv420_to_bgr24(VidFrame *src,VidFrame *dest);
int yuyv_to_rgb24(VidFrame *src,VidFrame *dest);
int yuyv_to_bgr24(VidFrame *src,VidFrame *dest);

#endif
