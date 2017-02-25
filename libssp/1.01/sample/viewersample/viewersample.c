
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libssp.h>

#include <wiringPi.h>

#include <signal.h>

#include<unistd.h>

#include<opencv/cv.h>
#include <opencv/highgui.h>
// sudo apt-get install libopencv-dev
// sudo apt-get install libgl1-mesa-dri

#include <sys/time.h>

static int frame_count;

int stop_process;
static void signal_handler(int sig)
{
  printf("Terminated.\n");
  if(sig == SIGINT)
    stop_process = 1;
}


void frame_drop_func(struct ssp_handle *handle){

    printf("Frame dropped by camera.\n");

}

void frame_drop_preprocess_func(struct ssp_handle *handle){

  printf("Frame dropped at preprocess.\n");
}

IplImage* BayerImg, *RGBImg;

#define WaitCount 200

void GammaCorrection(IplImage* src, IplImage* dst, double gamma){
  int i;
  uchar LUT[256];

  for (i = 0; i < 256; i++){
    LUT[i] = (int)(pow((double)i / 255.0, 1.0 / gamma) * 255.0);
  }
  CvMat lut_mat = cvMat(1, 256, CV_8UC1, LUT);
  cvLUT(src, dst, &lut_mat);
}

void fame_preprocess(struct ssp_handle *handle, struct ssp_frame *frame){
  //  printf("Preprocess is active. frame_count = %d\n", frame_count);
  //if(frame_count % 10 == 0) printf("frame_count = %d\n", frame_count);

  int i,j;
  int Width = ssp_get_frame_width(frame);
  int Height = ssp_get_frame_height(frame);
  unsigned char *frame_data = ssp_get_frame_data(frame);
  for(i=0;i<BayerImg->height;i++)
    for(j=0;j<BayerImg->width;j++){
      ((unsigned char *)(BayerImg->imageData + BayerImg->widthStep * i))[j] = frame_data[i*BayerImg->widthStep+j];
    }
  
    ssp_release_frame(frame);
    
    cvCvtColor( BayerImg, RGBImg, CV_BayerRG2RGB);
    GammaCorrection(RGBImg, RGBImg, 2.2);
    
    cvShowImage("SSP Viewer", RGBImg);
      cvWaitKey(2);

    printf("frame  count = %d\n", frame_count);
    frame_count ++;
}

#define GAMMA_OPTION SSP_DISABLE
//#define GAMMA_OPTION SSP_ENABLE
int main(int argc, char **argv){

  if(argc < 2){
    printf("SSP library viewer sample program\n");
    printf("Copyright (c) Streamtechnology Corporation.\n");
    printf("              http://www.streamtechnology.co.jp/\n");
    printf("Usage: %s profile_file\n", argv[0]);
    return 0;
  }

  char *profile_filename;
  char *filename;

  profile_filename = argv[1];
  filename = argv[2];

  stop_process = 0;
  if (signal(SIGINT, signal_handler) == SIG_ERR)
    return 2;

  struct ssp_settings settings;

  frame_count = 0;
  
  settings.lib_settings.NumFrameFIFOSize = 2;
  settings.lib_settings.NumThreadForBuildInPreprocess = 4;
  settings.lib_settings.frame_drop_cam_user_func = frame_drop_func;
  settings.lib_settings.frame_drop_pre_user_func = frame_drop_preprocess_func;
  settings.lib_settings.fame_preprocess_user_func = fame_preprocess;
  settings.frame_preprocess_options.FormatConvertType = SSP_FRAME_BAYER8;
  settings.frame_preprocess_options.GammaCorrectionEnable = GAMMA_OPTION;
  settings.frame_preprocess_options.GammaVal = 2.2;
  settings.camera_settings.PowerOnResetUsec = 1000000;

  struct ssp_handle *handle;

  struct ssp_profile *profile;
  
  ssp_read_profile(profile_filename, &profile);

  printf("Setting summary: %d x %d @ %d fps %d bits\n",
	 profile->ImageProperty.Width,
	 profile->ImageProperty.Height,
	 profile->ImageProperty.Frequency,
	 profile->ImageProperty.BayerBits);

  // OpenCV Settings
  cvNamedWindow("SSP Viewer", CV_WINDOW_AUTOSIZE); 

  BayerImg = cvCreateImage(cvSize(profile->ImageProperty.Width,profile->ImageProperty.Height),IPL_DEPTH_8U,1);
  RGBImg = cvCreateImage(cvSize(profile->ImageProperty.Width,profile->ImageProperty.Height),IPL_DEPTH_8U,3);

  
  int i,j;
  for(i=0;i<1;i++){ // for repeat initialization test.
    int retval;
    if((retval = ssp_initialize(&handle, profile, &settings))!=SSP_SUCCESS){
      
      printf("Can not initialize ssp library. %x\n",retval);
      return 0;
    }

    printf("Initialize done.\n");
    printf("start streaming\n");
    if((retval = ssp_start_streaming(handle)) != SSP_SUCCESS){
	
      printf("Can not start streaming. %x\n",retval);
      return 0;
    }

    struct timeval tv_start, tv_end;
    struct timezone tz_local;
    gettimeofday(&tv_start, &tz_local);
    
    cvWaitKey(2);

    while(1){ if(stop_process==1) break;}
      
    gettimeofday(&tv_end, &tz_local);

    double start_time = tv_start.tv_sec+(double)tv_start.tv_usec/1000000;
    double end_time = tv_end.tv_sec+(double)tv_end.tv_usec/1000000;
    double fps = (double)frame_count/(end_time-start_time);

    printf("FrameRate = %f\n", fps);
    
    printf("FrameCount = %d\n", frame_count);
    printf("stop streaming\n");
    if((retval = ssp_stop_streaming(handle)) != SSP_SUCCESS){
	
      printf("Can not start streaming. %x\n",retval);
      return 0;
    }
    
    printf("Flush Wait.\n");
    ssp_flush_event(handle);
    printf("Flush Done.\n");

    if(ssp_finalize(handle) != SSP_SUCCESS){
      printf("Error during finalization.\n");
      return 0;
    }
    
    cvReleaseImage(&RGBImg);
    cvReleaseImage(&BayerImg);
    cvDestroyWindow("SSP Viewer");
    
    printf("Finalize done.\n");fflush(stdout);
  }
  return 1;
}
