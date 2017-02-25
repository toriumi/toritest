
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

#include<pthread.h>

static int frame_count;

volatile int stop_process;
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

volatile int SettingMode = 0;
#define GAIN_MODE 0
#define SS_MODE 1 // Shutter Speed
volatile int GammaEnable = 0;

struct __paracopy_args {
  int width;
  int height;
  unsigned char *src;
  unsigned char *dest;
};

void *paracopy_thread_func(void *param){
  struct __paracopy_args *copyparams = (struct __paracopy_args *)param;
  int i;
  for(i=0;i<copyparams->height;i++)
    memcpy(copyparams->dest+copyparams->width*i,
	   copyparams->src+copyparams->width*i,
	   copyparams->width);

  return NULL;
}

char *model_name;

#define GAIN_CHANGE_VAL 25

#define NUM_COPY_THREAD 4
pthread_t thread_handle[NUM_COPY_THREAD];
struct __paracopy_args copy_params[NUM_COPY_THREAD];

void fame_preprocess(struct ssp_handle *handle, struct ssp_frame *frame){
  //  printf("Preprocess is active. frame_count = %d\n", frame_count);
  //if(frame_count % 10 == 0) printf("frame_count = %d\n", frame_count);
  
  int i,j;
  int Width = ssp_get_frame_width(frame);
  int Height = ssp_get_frame_height(frame);
  unsigned char *frame_data = ssp_get_frame_data(frame);
#if 0 // Simple copy
  for(i=0;i<BayerImg->height;i++)
    for(j=0;j<BayerImg->width;j++){
      ((unsigned char *)(BayerImg->imageData + BayerImg->widthStep * i))[j] = frame_data[i*BayerImg->widthStep+j];
    }
#endif

#if 0 // memcpy
  for(i=0;i<BayerImg->height;i++){
    memcpy((unsigned char *)(BayerImg->imageData + BayerImg->widthStep * i), &frame_data[i*BayerImg->widthStep], BayerImg->width);
  }
#endif
#if 0 // long long copy
  for(i=0;i<BayerImg->height;i++)
    for(j=0;j<BayerImg->width/sizeof(long long);j++){
      *((long long *)(BayerImg->imageData + BayerImg->widthStep * i) + j) = *((long long*)(frame_data+i*BayerImg->widthStep)+j);
    }
#endif

#if 1 // parallel copy
  for(i=0;i<NUM_COPY_THREAD;i++){
    copy_params[i].width = BayerImg->widthStep;
    copy_params[i].height = BayerImg->height/NUM_COPY_THREAD;
    copy_params[i].dest = (unsigned char *)(BayerImg->imageData + BayerImg->widthStep * (i*BayerImg->height/NUM_COPY_THREAD));
    copy_params[i].src = &frame_data[(i*BayerImg->height/NUM_COPY_THREAD)*BayerImg->widthStep];
    pthread_create(&(thread_handle[i]) , NULL , paracopy_thread_func ,&(copy_params[i]));
      
  }
  for(i=0;i<NUM_COPY_THREAD;i++)
    pthread_join(thread_handle[i] , NULL);
#endif
  
    ssp_release_frame(frame);

    cvCvtColor( BayerImg, RGBImg, CV_BayerRG2RGB);
    if(GammaEnable == 1)
      GammaCorrection(RGBImg, RGBImg, 2.2);

    cvShowImage("SSP Viewer", RGBImg);
    int KeyCode = cvWaitKey(2);
    KeyCode &= 0xFF;
    if(KeyCode == 's'){
      printf("Setting Mode is set to Shutter Speed Mode.\n");
      SettingMode = SS_MODE;
    }
    if(KeyCode == 'g'){
      printf("Setting Mode is set to Gain Mode.\n");
      SettingMode = GAIN_MODE;
    }
    if(KeyCode == '0'){
      if(GammaEnable == 0){
	GammaEnable = 1;
	printf("Gamma Correction is Enabled.\n");
      }
      else{
	GammaEnable = 0;
	printf("Gamma Correction is Disabled.\n");
      }
    }
    if(KeyCode == '+' || KeyCode == '-'){
      int RegVal;

      if(SettingMode == GAIN_MODE){
	if(strcmp(model_name, "IMX219") == 0){
	  if(__CCIRegRead(handle, 0x0157, &RegVal) == 0)
	    printf("Reading register error.\n");
	  if(KeyCode == '+'){
	    RegVal += GAIN_CHANGE_VAL;
	    if(RegVal >= 233) RegVal = 0;
	  }
	  if(KeyCode == '-'){
	    RegVal -= GAIN_CHANGE_VAL;
	    if(RegVal <= -1) RegVal = 232;
	  }
	  if(__CCIRegWrite(handle, 0x0157, RegVal) == 0)
	    printf("Writing register error.\n");
	  if(__CCIRegRead(handle, 0x0157, &RegVal) == 0)
	    printf("Reading register error.\n");
	  printf("Analog Gain Set to %d\n", RegVal);

	}
	if(strcmp(model_name, "IU233") == 0){
	  if(__CCIRegRead(handle, 0x0205, &RegVal) == 0)
	    printf("Reading register error.\n");
	  if(KeyCode == '+'){
	    RegVal += GAIN_CHANGE_VAL;
	    if(RegVal >= 225) RegVal = 0;
	  }
	  if(KeyCode == '-'){
	    RegVal -= GAIN_CHANGE_VAL;
	    if(RegVal <= -1) RegVal = 224;
	  }
	  if(__CCIRegWrite(handle, 0x0205, RegVal) == 0)
	    printf("Writing register error.\n");
	  if(__CCIRegRead(handle, 0x0205, &RegVal) == 0)
	    printf("Reading register error.\n");
	  printf("Analog Gain Set to %d\n", RegVal);

	}
	if(strcmp(model_name, "IMX408") == 0){
	  int GainVal;
	  if(__CCIRegRead(handle, 0x0205, &RegVal) == 0)
	    printf("Reading register error.\n");
	  GainVal = RegVal;
	  if(__CCIRegRead(handle, 0x0204, &RegVal) == 0)
	    printf("Reading register error.\n");
	  GainVal |= ((RegVal & 0x1) << 8);
	  RegVal = GainVal;
	  if(KeyCode == '+'){
	    RegVal += GAIN_CHANGE_VAL;
	    if(RegVal >= 481) RegVal = 0;
	  }
	  if(KeyCode == '-'){
	    RegVal -= GAIN_CHANGE_VAL;
	    if(RegVal <= -1) RegVal = 480;
	  }
	  if(__CCIRegWrite(handle, 0x0205, RegVal & 0xFF) == 0)
	    printf("Writing register error.\n");
	  if(__CCIRegWrite(handle, 0x0204, (RegVal>>8)&0x1) == 0)
	    printf("Writing register error.\n");	
	  if(__CCIRegRead(handle, 0x0205, &RegVal) == 0)
	    printf("Reading register error.\n");
	  GainVal = RegVal;
	  if(__CCIRegRead(handle, 0x0204, &RegVal) == 0)
	    printf("Reading register error.\n");
	  GainVal |= ((RegVal & 0x1) << 8);
	  RegVal = GainVal;
	  printf("Analog Gain Set to %d\n", RegVal);

	}
      }

      if(SettingMode == SS_MODE){
	if(strcmp(model_name, "IMX219") == 0){
	  int frame_length_lines;
	  int coarse_integration_time;
	  
	  if(__CCIRegRead(handle, 0x0160, &RegVal) == 0)
	    printf("Reading register error.\n");
	  frame_length_lines = RegVal<<8;
	  if(__CCIRegRead(handle, 0x0161, &RegVal) == 0)
	    printf("Reading register error.\n");
	  frame_length_lines |= RegVal;

	  if(__CCIRegRead(handle, 0x015A, &RegVal) == 0)
	    printf("Reading register error.\n");
	  coarse_integration_time = RegVal<<8;
	  if(__CCIRegRead(handle, 0x015B, &RegVal) == 0)
	    printf("Reading register error.\n");
	  coarse_integration_time |= RegVal;

	  if(KeyCode == '+'){
	    coarse_integration_time ++;
	    if(coarse_integration_time == (frame_length_lines-5)) RegVal = 1;
	  }
	  if(KeyCode == '-'){
	    coarse_integration_time --;
	    if(coarse_integration_time == -1) coarse_integration_time = frame_length_lines-4;
	  }
	  if(__CCIRegWrite(handle, 0x015A, (coarse_integration_time>>8)&0xFF) == 0)
	    printf("Writing register error.\n");
	  if(__CCIRegWrite(handle, 0x015B, coarse_integration_time&0xFF) == 0)
	    printf("Writing register error.\n");
	  if(__CCIRegRead(handle, 0x015A, &RegVal) == 0)
	    printf("Reading register error.\n");
	  coarse_integration_time = RegVal<<8;
	  if(__CCIRegRead(handle, 0x015B, &RegVal) == 0)
	    printf("Reading register error.\n");
	  coarse_integration_time |= RegVal;
	  printf("Shutter Speed Set (coarse_integration_time) to %d\n", coarse_integration_time);
	}
      }
    }
    
    printf("frame  count = %d\n", frame_count);
    frame_count ++;
}

int __CCIRegRead(struct ssp_handle *handle, int address, int *data);
int __CCIRegWrite(struct ssp_handle *handle, int address, int data);

#define GAMMA_OPTION SSP_DISABLE
//#define GAMMA_OPTION SSP_ENABLE
int main(int argc, char **argv){

  if(argc < 3){
    printf("SSP library viewer sample program\n");
    printf("Copyright (c) Streamtechnology Corporation.\n");
    printf("              http://www.streamtechnology.co.jp/\n");
    printf("Usage: %s profile_file model\n", argv[0]);
    printf("\tmodel: IMX219 or IMX408 or IU233\n");
    return 0;
  }

  model_name = argv[2];

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
    double fps;
    fps = (double)frame_count/(end_time-start_time);
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
