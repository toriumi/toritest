
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libssp.h>
#include <libsspprof.h>

#include <wiringPi.h>

#include <signal.h>

#include<unistd.h>

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

char *filename;
#define WaitCount 1000
void fame_preprocess(struct ssp_handle *handle, struct ssp_frame *frame){
  //  printf("Preprocess is active. frame_count = %d\n", frame_count);
  frame_count ++;
  if(frame_count % 10 == 0) printf("frame_count = %d\n", frame_count);
  usleep(1000);
  if(frame_count == WaitCount){
    FILE *file;
    file = fopen(filename, "wb");
    if(file){
      size_t size = 0;
      if(ssp_get_frame_size(frame, &size) != SSP_SUCCESS){
	printf("Unkown frame type.\n");
	ssp_release_frame(frame);
	return;
      }
      fwrite(ssp_get_frame_data(frame), size, 1, file);
      fclose(file);
      printf("Saved RAW image to %s\n", filename);
    }
  }

  ssp_release_frame(frame);

}

int main(int argc, char **argv){

  if(argc < 3){
    printf("SSP library still sample program in RAW16\n");
    printf("Copyright (c) Streamtechnology Corporation.\n");
    printf("              http://www.streamtechnology.co.jp/\n");
    printf("Usage: %s profile_file_name output_file_name\n", argv[0]);

    return 0;
  }

  char *profile_filename;

  profile_filename = argv[1];
  filename = argv[2];

  
  stop_process = 0;
  if (signal(SIGINT, signal_handler) == SIG_ERR)
    return 2;
  
  struct ssp_settings settings;

  frame_count = 0;
  
  settings.lib_settings.NumFrameFIFOSize = 16;
  settings.lib_settings.NumThreadForBuildInPreprocess = 1;
  settings.lib_settings.frame_drop_cam_user_func = frame_drop_func;
  settings.lib_settings.frame_drop_pre_user_func = frame_drop_preprocess_func;
  settings.lib_settings.fame_preprocess_user_func = fame_preprocess;
  settings.frame_preprocess_options.FormatConvertType = SSP_FRAME_BAYER16;
  settings.frame_preprocess_options.GammaCorrectionEnable = SSP_DISABLE;
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

    while(frame_count < WaitCount){
      if(stop_process != 0) break;
    }
    
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
    
    printf("Sync Wait.\n");
    ssp_sync_event(handle);
    printf("Sync Done.\n");

    if(ssp_finalize(handle) != SSP_SUCCESS){
      printf("Error during finalization.\n");
      return 0;
    }
    printf("Finalize done.\n");fflush(stdout);
  }
  return 1;
}
