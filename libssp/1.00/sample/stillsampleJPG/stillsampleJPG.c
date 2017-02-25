
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libssp.h>

#include <wiringPi.h>

#include <signal.h>

#include<unistd.h>

#include <jpeglib.h>
// sudo apt-get install libjpeg-dev

static int frame_count;

char *filename;
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

struct jpeg_compress_struct cinfo;
struct jpeg_error_mgr jerr;
JSAMPARRAY img;

int ImgWidth, ImgHeight;

#define WaitCount 10
void fame_preprocess(struct ssp_handle *handle, struct ssp_frame *frame){
  //  printf("Preprocess is active. frame_count = %d\n", frame_count);

  int i,j;
  
  frame_count ++;
  if(frame_count % 1 == 0) printf("frame_count = %d\n", frame_count);
  if(frame_count == WaitCount){
    unsigned char *frame_data = ssp_get_frame_data(frame);
    jpeg_start_compress(&cinfo, TRUE);
    ImgHeight = ssp_get_frame_height(frame);
    ImgWidth = ssp_get_frame_width(frame);
    img = (JSAMPARRAY) malloc(sizeof(JSAMPROW) * ImgHeight);
    for (i = 0; i < ImgHeight; i++) {
      img[i] = (JSAMPROW) malloc(sizeof(JSAMPLE) * 3 * ImgWidth);
      for (j = 0; j < ssp_get_frame_width(frame); j++) {
	img[i][j*3 + 0] = frame_data[i*ImgWidth*3+j*3];
	img[i][j*3 + 1] = frame_data[i*ImgWidth*3+j*3+1];
	img[i][j*3 + 2] = frame_data[i*ImgWidth*3+j*3+2];
      }
    }
    jpeg_write_scanlines(&cinfo, img, ImgHeight);
    jpeg_finish_compress(&cinfo);
    printf("Save JPEG image\n");
  }

  ssp_release_frame(frame);

}

#define GAMMA_OPTION SSP_DISABLE
//#define GAMMA_OPTION SSP_ENABLE
int main(int argc, char **argv){
  if(argc < 3){
    printf("SSP library still sample program in JPEG\n");
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

  struct ssp_profile *profile;
  
  ssp_read_profile(profile_filename, &profile);

  printf("Setting summary: %d x %d @ %d fps %d bits\n",
	 profile->ImageProperty.Width,
	 profile->ImageProperty.Height,
	 profile->ImageProperty.Frequency,
	 profile->ImageProperty.BayerBits);

  // jpeg settings
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  FILE *fp = fopen(filename, "wb");
  if (fp == NULL) {
    printf("JPEG file cannot be opened.\n");
    return -1;
  }
  jpeg_stdio_dest(&cinfo, fp);
  cinfo.image_width = profile->ImageProperty.Width;
  cinfo.image_height = profile->ImageProperty.Height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, 75, TRUE);

  // SSP settings
  settings.lib_settings.NumFrameFIFOSize = 16;
  settings.lib_settings.NumThreadForBuildInPreprocess = 1;
  settings.lib_settings.frame_drop_cam_user_func = frame_drop_func;
  settings.lib_settings.frame_drop_pre_user_func = frame_drop_preprocess_func;
  settings.lib_settings.fame_preprocess_user_func = fame_preprocess;
  settings.frame_preprocess_options.FormatConvertType = SSP_FRAME_RGB24;
  settings.frame_preprocess_options.GammaCorrectionEnable = GAMMA_OPTION;
  settings.frame_preprocess_options.GammaVal = 2.2;
  settings.camera_settings.PowerOnResetUsec = 1000000;

  struct ssp_handle *handle;


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
      
    while(frame_count < WaitCount){
      if(stop_process != 0) break;
    }
    printf("FrameCount = %d\n", frame_count);
    printf("stop streaming\n");
    if((retval = ssp_stop_streaming(handle)) != SSP_SUCCESS){
	
      printf("Can not start streaming. %x\n",retval);
      return 0;
    }
    
    printf("Flush Wait.\n");
    ssp_flush_event(handle);
    printf("Flush Done.\n");

    jpeg_destroy_compress(&cinfo);
    for (j = 0; j < ImgHeight; j++) {
      free(img[j]);
    }
    free(img);
    fclose(fp);
    
    if(ssp_finalize(handle) != SSP_SUCCESS){
      printf("Error during finalization.\n");
      return 0;
    }
    printf("Finalize done.\n");fflush(stdout);
  }
  return 1;
}
