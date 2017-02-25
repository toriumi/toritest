#ifndef __LIBSSP
#define __LIBSSP

typedef int SSP_RESULT;

#include<libsspprof.h>


#define SSP_SUCCESS 0
#define SSP_ERR_NOMEM 0x8000000 // Memory Exahousted
#define SSP_ERR_THREAD 0x8000001 // Thread can not be created
#define SSP_ERR_EBAYERBITS 0x8000002 // BayerBits are specified an invalid value
#define SSP_ERR_EINIT 0x8000003 // Library initialization failed.
#define SSP_ERR_ECAMSETUP 0x80000005 // Error during setting up camera
#define SSP_ERR_EMMAL 0x80000006 // Error in MMAL
#define SSP_ERR_EFRAMTYPE 0x80000007 // Unkown frame tyepe.
#define SSP_ERR_EFATAL 0x8FFFFFFF // Fatal error. Bug?
#define SSP_ERR_ENUMTHREADS 0x80000008 // (Width*Height)%(NumThreadForBuildInPreprocess*4) must be zero.
#define SSP_ERR_XMLURI 0x80000009
#define SSP_ERR_XMLELEMENT 0x8000000A

#define SSP_FRAME_BAYER10 0x1 // Bayer RAW10 format. Even line begins RGRGRG..., odd line begins GBGBGB ...
#define SSP_FRAME_BAYER8 0x2 // Bayer RAW8 format. Even line begins RGRGRG..., odd line begins GBGBGB ...
#define SSP_FRAME_RGB24 0x3 // RGB24 format. RGB is packed with 8bit R, G and B into RGBRGB.... 
#define SSP_FRAME_YUV444 0x4 // YUV444 format. YUV is packed with 8bit Y, Cb and Cr into YCbCrYCbCr.... 
#define SSP_FRAME_YUV422 0x5 // YUV422 format. YUV is packed with 8bit Y, Cb, Y and Cr into Y0 Cb00 Y1 Cr10 Y2 Cb21 Y3 Cr31....
#define SSP_FRAME_BAYER16 0x6 // Psuedo 16bit Bayer format. Even line begins RGRGRG..., odd line begins GBGBGB ...

struct ssp_frame {
  int FrameType;
  int Width;
  int Height;
  unsigned char *FrameData;
};

#define SSP_DISABLE 0
#define SSP_ENABLE 1

struct ssp_handle {
  void *user_data;
  struct ssp_settings *settings;
  void *internal_data;
};

struct ssp_camera_register {
  int address;
  int val;
};
  
//struct ssp_camera_register_setting {
//  struct ssp_camera_register *registers;
//  int NumRegisters;
//};

struct ssp_settings {
  struct ssplib_settings {
    int NumFrameFIFOSize;
    int NumThreadForBuildInPreprocess;
    void (*frame_drop_cam_user_func)(struct ssp_handle *handle);
    void (*frame_drop_pre_user_func)(struct ssp_handle *handle);
    void (*fame_preprocess_user_func)(struct ssp_handle *handle, struct ssp_frame *frame);
  }lib_settings;
  struct ssp_camera_settings {
    int PowerOnResetUsec;
    //    int Model;
    //    int OutputImageWidth;
    //    int OutputImageHeight;
    //    int PhysicalImageWidth;
    //    int PhysicalImageHeight;
    //    int BayerBits;
    //    int NumCSILanes;
    //    int FrameRate;
    //    struct ssp_camera_register_setting register_settings;
  }camera_settings;
  struct ssp_frame_preprocess_options {
    int FormatConvertType;
    int GammaCorrectionEnable;
    float GammaVal;
  }frame_preprocess_options;
};


#define CAMERA_MODEL_IMX219 0x00000219
#define CAMERA_MODEL_IU233 0x00000233
#define CAMERA_MODEL_IMX408 0x00000408
#define CAMERA_MODEL_IMX290 0x00000290

#define ssp_get_frame_data(frame) (frame->FrameData)
#define ssp_get_frame_type(frame) (frame->FrameType)
#define ssp_get_frame_height(frame) (frame->Height)
#define ssp_get_frame_width(frame) (frame->Width)
#define ssp_release_frame(frame) {free(frame->FrameData); free(frame);}

SSP_RESULT ssp_get_frame_size(struct ssp_frame *frame, int *size);
SSP_RESULT ssp_initialize(struct ssp_handle **handle, struct ssp_profile *profile, struct ssp_settings *settings);
SSP_RESULT ssp_start_streaming(struct ssp_handle *handle);
SSP_RESULT ssp_sync_event(struct ssp_handle *handle);
SSP_RESULT ssp_flush_event(struct ssp_handle *handle);
SSP_RESULT ssp_stop_streaming(struct ssp_handle *handle);
SSP_RESULT ssp_finalize(struct ssp_handle *handle);

#endif // __LIBSSP
