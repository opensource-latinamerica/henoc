#ifndef FOURCC_H_
#define FOURCC_H_

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */

#define FOURCC(a,b,c,d) (((guint32)(a)<<0)|((guint32)(b)<<8)|((guint32)(c)<<16)|((guint32)(d)<<24))

typedef unsigned int fourcc_t;

/// Return the name of a fourcc.  
const char* vidFourccToString(fourcc_t code);

/// Calculate the size of buffer required to hold a frame with specificed width, height and format.  
int vidFourccCalcFrameSize(fourcc_t code,int width,int height);

#ifdef __cplusplus
} /* extern "C" */
#endif /* defined(__cplusplus) */


#endif /*FOURCC_H_*/
