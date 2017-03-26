#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libssp.h>
#include <wiringPi.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include<opencv/cv.h>
#include <opencv/highgui.h>
// sudo apt-get install libopencv-dev
// sudo apt-get install libgl1-mesa-dri

#include <sys/time.h>

#define FRAME_MODE_RECORDING		(0)
#define FRAME_MODE_PREVIEW		(1)
#define FRAME_MODE_REALTIMEPREVIEW	(2)

#define INFO_SHOW_ENABLE		(1)
#define INFO_SHOW_DISABLE		(0)

#define KEY_CMD_QUIT			'q'
#define KEY_CMD_RECORDING		'r'
#define KEY_CMD_REALTIMEPREVIEW		'v'
#define KEY_CMD_WRITEVIDEO		'w'
#define KEY_CMD_WRITEVIDEO_MJPG         'W'
#define KEY_CMD_WRITEVIDEO_MP4          'a'
#define KEY_CMD_PAUSE			'p'
#define KEY_CMD_TOSTARTFRAME		's'
#define KEY_CMD_TOENDFRAME		'e'
#define KEY_CMD_SAVEFRAME		'z'
#define KEY_CMD_SAVEALLFRAME		'x'
#define KEY_CMD_SHOWINFO		't'
#define KEY_CMD_SLOWRATE_0		'0'
#define KEY_CMD_SLOWRATE_5		'5'
#define KEY_CMD_FRAMESTEP_5		'9'
#define KEY_CMD_FRAMESTEP_1		'8'
#define KEY_CMD_NEXTFRAME		' '
#define KEY_CMD_PREVFRAME		'm'
#define KEY_CMD_NEXTFRAME_SPD		'f'
#define KEY_CMD_PREVFRAME_SPD		'b'
#define KEY_CMD_NEXTFRAME_HIGH		'F'
#define KEY_CMD_PREVFRAME_HIGH		'B'
#define KEY_CMD_GETBALANCE		'c'

#define PAUSE_MODE_PAUSED		(1)
#define PAUSE_MODE_RUNNING		(0)

#define FRAME_COUNT	(600)           // specify how many frames you want to save
#define SLOW_RATE	(33)            // wait time
#define FPS_UPDATE_FREQUENCY		(100)
#define STEPS_SPD			(100)
#define STEPS_HIGH			(200) 
//#define RAW10THRU

// for recording, etc.
int g_frame_count = 0;
int g_pause_mode = PAUSE_MODE_RUNNING;
int g_slow_rate = 0;
int g_info_show = INFO_SHOW_ENABLE;
float g_balanceR = 1.f;
float g_balanceG = 1.f;
float g_balanceB = 1.f;
static int g_frame_mode_flg = FRAME_MODE_REALTIMEPREVIEW;
static int g_fno = 0;
static int g_stepval = 1;
struct ssp_frame *g_frame_array[FRAME_COUNT];
int g_max_frame = FRAME_COUNT;
int g_save_frame = 0;

// for output text
CvFont g_font;
char g_text_buf[256];

// for writing video
CvVideoWriter *g_video_writer = NULL;
int g_video_write_flg = 0;

volatile int stop_process;  // volatile !!!

IplImage* BayerImg, *RGBImg;

// for fps calculation
struct timeval tv_start, tv_end;
struct timezone tz_local;
double fps = 0;

struct ssp_profile *profile;

int __CCIRegWrite(struct ssp_handle *handle, int address, int data);
int __CCIRegRead(struct ssp_handle *handle, int address, int *data);

static void signal_handler(int sig)
{
  printf("Terminated.\n");
  if(sig == SIGINT)
    stop_process = 1;
}

void frame_drop_func(struct ssp_handle *handle)
{
  printf("Frame dropped by camera.\n");
}

void frame_drop_preprocess_func(struct ssp_handle *handle)
{
  printf("Frame dropped at preprocess.\n");
}

void reset_fps_start_time()
{
  gettimeofday(&tv_start, &tz_local);
}

void get_time_passed()
{
  gettimeofday(&tv_end, &tz_local);
  double start_time = tv_start.tv_sec+(double)tv_start.tv_usec/1000000;
  double end_time = tv_end.tv_sec+(double)tv_end.tv_usec/1000000;
  fps = (double)g_frame_count/(end_time-start_time);
  printf("Completed.\n");
  printf("FrameRate = %f\n", fps);
  printf("Frame Count = %d\n", g_save_frame);
  gettimeofday(&tv_start, &tz_local);
}

void frame_valiables_init()
{
  g_pause_mode = PAUSE_MODE_RUNNING;
  g_fno = 0;
  g_slow_rate = 0;
  g_save_frame = 0; 
  g_frame_count = 0;
}

void frame_apply_easy_white_balance(unsigned char *frame_data)
{
  int i, j;
  float R, Gr, Gb, B;

  for (i=0;i<BayerImg->height;i+=2) {
    for (j=0;j<BayerImg->width;j+=2){
      R  = frame_data[(i  )*BayerImg->widthStep+j  ];
      R -= ((R >= 16)?16:R);
      R = R * g_balanceR;
      if (R > 255.f) R = 255;

      Gr = frame_data[(i  )*BayerImg->widthStep+j+1];
      Gr -= ((Gr >= 16)?16:Gr);

      Gb = frame_data[(i+1)*BayerImg->widthStep+j  ];
      Gb -= ((Gb >= 16)?16:Gb);

      B  = frame_data[(i+1)*BayerImg->widthStep+j+1];
      B -= ((B >= 16)?16:B);

      B = B * g_balanceB;
      if (B > 255.f) B = 255;

      ((unsigned char *)(BayerImg->imageData + BayerImg->widthStep * (i  )))[j  ] = R;
      ((unsigned char *)(BayerImg->imageData + BayerImg->widthStep * (i  )))[j+1] = Gr;
      ((unsigned char *)(BayerImg->imageData + BayerImg->widthStep * (i+1)))[j  ] = Gb;
      ((unsigned char *)(BayerImg->imageData + BayerImg->widthStep * (i+1)))[j+1] = B;
    }
  }
}

void frame_write_video()
{
  if (g_video_writer != NULL) {
    cvWriteFrame(g_video_writer, RGBImg);
    sprintf(g_text_buf, "[Writing...] frame=%05d", g_fno);
    cvPutText(RGBImg, g_text_buf, cvPoint(0,30), &g_font, CV_RGB(255, 0, 0));
    cvShowImage("SSP Viewer", RGBImg);
    cvWaitKey(1);
  }
  g_fno++;
  if (g_fno >= g_save_frame) {
    g_fno = 0;
    g_pause_mode = PAUSE_MODE_PAUSED;
    g_video_write_flg = 0;
    if (g_video_writer != NULL) {
      cvReleaseVideoWriter(&g_video_writer);
      g_video_writer = NULL;
      printf("VideoWriter is released.\n");
    }
  }
}

void frame_preview_info_show()
{
  if (g_info_show == INFO_SHOW_ENABLE) {
    sprintf(g_text_buf, "[Preview Mode] frame=%05d", g_fno);
    cvPutText(RGBImg, g_text_buf, cvPoint(0,30), &g_font, CV_RGB(255, 0, 0));
    if (g_stepval != 0) {
      sprintf(g_text_buf, "step = %d", g_stepval);
      cvPutText(RGBImg, g_text_buf, cvPoint(0,60), &g_font, CV_RGB(255, 0, 0));
    }
  }
}

void frame_preview_writevideo_allframe(int fourcc, char *filename)
{
  printf("Write frame rate = %d\n", (int)fps);
  g_video_writer = cvCreateVideoWriter(filename, fourcc,
                                    (int)fps,
                                    cvSize(profile->ImageProperty.Width,
                                           profile->ImageProperty.Height), 1);
  if (g_video_writer == NULL) {
    printf("OpenCV Error: g_video_writer\n");
  } 
  else {
    g_pause_mode = 0;
    g_fno = 0;
    g_slow_rate = 0;
    g_video_write_flg = 1;
  }
}

void frame_preview_save_all_frame()
{
  int i;
  char fname[256];
  unsigned char *frame_data;
  int Width = ssp_get_frame_width(g_frame_array[1]);
  int Height = ssp_get_frame_height(g_frame_array[1]);

  for (i = 0; i < g_save_frame; i++) {
    sprintf(fname, "frame%04d.raw", i);
    FILE *fp = fopen(fname, "wb");
    frame_data = ssp_get_frame_data(g_frame_array[i]);
    fwrite(frame_data, Width*Height, 1, fp);
    fclose(fp);
    printf("written. %s\n", fname);
  }
}

void frame_preview_saveframe()
{
  char fname[256];
  unsigned char *frame_data = ssp_get_frame_data(g_frame_array[g_fno]);
  int Width = ssp_get_frame_width(g_frame_array[g_fno]);
  int Height = ssp_get_frame_height(g_frame_array[g_fno]);

  sprintf(fname, "frame%04d.raw", g_fno);
  FILE *fp = fopen(fname, "wb");
  fwrite(frame_data, Width*Height, 1, fp);
  fclose(fp);
  printf("written. %s\n", fname);
}

void init_frame_array()
{
  int i;
  for (i = 0; i < FRAME_COUNT; i++) {
    g_frame_array[i] = NULL;
  }
}

void discard_frame_array()
{
  int i;

  for (i = 0; i < FRAME_COUNT; i++) {
    if (g_frame_array[i] != NULL) {
      ssp_release_frame(g_frame_array[i]);
      g_frame_array[i] = NULL;
    }
  }
}

void make_filename(char *filename, char *basename, char* extname)
{
  time_t timer;
  struct tm *local;

  timer = time(NULL);
  local = localtime(&timer);
  sprintf(filename, "%s%04d%02d%02d%02d%02d%02d.%s",
          basename,
          local->tm_year+1900,
          local->tm_mon +1,
          local->tm_mday,
          local->tm_hour,
          local->tm_min,
          local->tm_sec,
          extname);
}

void process_preview_mode_key(int key)
{
  char filename[PATH_MAX];

  if (key == KEY_CMD_PAUSE) {
    g_pause_mode ^= 1; // toggle pause mode
  }
  else if ((key >= KEY_CMD_SLOWRATE_0) && (key <= KEY_CMD_SLOWRATE_5)) {
    g_slow_rate = key - KEY_CMD_SLOWRATE_0;
  }
  else if (key == KEY_CMD_FRAMESTEP_1) {
    g_stepval = 1;
  }
  else if (key == KEY_CMD_FRAMESTEP_5) {
    g_stepval = 5;
  }
  else if (key == KEY_CMD_NEXTFRAME_SPD) {
    g_fno += STEPS_SPD;
    if (g_fno >= FRAME_COUNT) {
      g_fno = FRAME_COUNT - 1;
    }
  }
  else if (key == KEY_CMD_PREVFRAME_SPD) {
    if (g_fno >= STEPS_SPD) {
      g_fno -= STEPS_SPD;
    }
    else {
      g_fno = 0;
    }
  }
  else if (key == KEY_CMD_NEXTFRAME_HIGH) {
    g_fno += STEPS_HIGH;
    if (g_fno >= FRAME_COUNT) {
      g_fno = FRAME_COUNT - 1;
    }
  }
  else if (key == KEY_CMD_PREVFRAME_HIGH) {
    if (g_fno >= STEPS_HIGH) {
      g_fno -= STEPS_HIGH;
    }
    else {
      g_fno = 0;
    }
  }

  if (g_pause_mode == PAUSE_MODE_RUNNING) {
    g_fno += g_stepval;
    if (g_fno >= g_save_frame) {
      g_fno = 0;
    }
  }
  else {
    sprintf(g_text_buf,"[Pause]");
    if (g_info_show == INFO_SHOW_ENABLE) cvPutText(RGBImg, g_text_buf, cvPoint(0,90), &g_font, CV_RGB(255, 0, 0));

    if (key == KEY_CMD_NEXTFRAME) {
      g_fno += g_stepval;
      if (g_fno >= g_save_frame) {
        g_fno = 0;
      }
    }
    else if (key == KEY_CMD_PREVFRAME) {
      if (g_fno >= g_stepval) {
        g_fno -= g_stepval;
      }
    }
  }

  if (key == KEY_CMD_TOSTARTFRAME) {       // go to the first frame
    g_fno = 0;
    g_pause_mode = PAUSE_MODE_PAUSED;
  }
  else if (key == KEY_CMD_TOENDFRAME) {  // go to the end frame
    g_fno = g_save_frame - 1;
    g_pause_mode = PAUSE_MODE_PAUSED;
  }
  else if ((key == KEY_CMD_RECORDING) || (key == KEY_CMD_REALTIMEPREVIEW)) {
    if (key == KEY_CMD_REALTIMEPREVIEW) {
      g_frame_mode_flg = FRAME_MODE_REALTIMEPREVIEW;
    }
    else {
      printf("Recording...\n");
      sprintf(g_text_buf, "[Recording...]");
      if (g_info_show == INFO_SHOW_ENABLE) cvPutText(RGBImg, g_text_buf, cvPoint(0,150), &g_font, CV_RGB(255, 0, 0));
      cvShowImage("SSP Viewer", RGBImg);
      cvWaitKey(1);
      g_frame_mode_flg = FRAME_MODE_RECORDING;
    }

    frame_valiables_init();

    // discard old frames
    discard_frame_array();
    reset_fps_start_time();
  }
  else if (key == KEY_CMD_SHOWINFO) {
     g_info_show ^= 1;  
  }
  else if (key == KEY_CMD_WRITEVIDEO) {  // RAW
    make_filename(filename, "capture_raw_", "avi");
    frame_preview_writevideo_allframe(0, filename);
  }
  else if (key == KEY_CMD_WRITEVIDEO_MJPG) {
    make_filename(filename, "capture_", "mjpg");
    frame_preview_writevideo_allframe(CV_FOURCC('M','J','P','G'), filename);
  }
  else if (key == KEY_CMD_WRITEVIDEO_MP4) {
    make_filename(filename, "capture_mp4_", "avi");
    frame_preview_writevideo_allframe(CV_FOURCC('F', 'M','P','4'), filename);
  }

  else if (key == KEY_CMD_QUIT) {
    stop_process = 1;
  }
  else if (key == KEY_CMD_SAVEALLFRAME) {   // save frames
    frame_preview_save_all_frame();
  }
  else if (key == KEY_CMD_SAVEFRAME) {      // save the frame
    frame_preview_saveframe();
  }

  sprintf(g_text_buf,"Slow Rate = %d", g_slow_rate);
  if (g_info_show == INFO_SHOW_ENABLE) cvPutText(RGBImg, g_text_buf, cvPoint(0,120), &g_font, CV_RGB(255, 0, 0));
  if (g_slow_rate != 0) {
    usleep(g_slow_rate*SLOW_RATE*1000);
  }
}

double get_fps()
{
  gettimeofday(&tv_end, &tz_local);
  double start_time = tv_start.tv_sec+(double)tv_start.tv_usec/1000000;
  double end_time = tv_end.tv_sec+(double)tv_end.tv_usec/1000000;
  double fps = (double)g_frame_count/(end_time-start_time);

  return fps;
}

void update_fps_base()
{
  g_frame_count++;
  if (g_frame_count > FPS_UPDATE_FREQUENCY) { // reset offset time every FPS_UPDATE_FREQUENCY frames
    g_frame_count = 0;
    reset_fps_start_time();
  }
}

void frame_preview()
{
  struct ssp_frame *frame;
  unsigned char *frame_data;

  do {
    frame = g_frame_array[g_fno];
    frame_data = ssp_get_frame_data(frame);

    frame_apply_easy_white_balance(frame_data);

    cvCvtColor( BayerImg, RGBImg, CV_BayerRG2RGB);

    if (g_video_write_flg == 1) {
      frame_write_video();
    }
  }
  while (g_video_write_flg == 1);

  frame_preview_info_show();
  int key = cvWaitKey(10) & 0xff;
  process_preview_mode_key(key);

  // get fps
  double fps = get_fps();

  // show fps
  sprintf(g_text_buf,"FrameRate = %f", fps);
  if (g_info_show == INFO_SHOW_ENABLE) cvPutText(RGBImg, g_text_buf, cvPoint(0,180), &g_font, CV_RGB(255, 0, 0));

  update_fps_base();

  cvShowImage("SSP Viewer", RGBImg);

}

void get_easy_white_balance_profile(unsigned char *frame_data, struct ssp_frame *frame)
{
  int sumR=0, sumGr=0, sumGb=0, sumB=0, aveR=0, aveG=0, aveB=0;
  int i, j;
  int Width = ssp_get_frame_width(frame);
  int Height = ssp_get_frame_height(frame);
    
  for (i=0;i<BayerImg->height; i+=2) {
    for (j=0;j<BayerImg->width; j+=2){
      sumR  = sumR  + frame_data[(i  )*BayerImg->widthStep+j  ];
      sumGr = sumGr + frame_data[(i  )*BayerImg->widthStep+j+1];
      sumGb = sumGb + frame_data[(i+1)*BayerImg->widthStep+j  ];
      sumB  = sumB  + frame_data[(i+1)*BayerImg->widthStep+j+1];
    }
  }
  aveR = sumR / (Width*Height/4);
  aveB = sumB / (Width*Height/4);
  aveG = (sumGr + sumGb) / (Width*Height/2);
  g_balanceR = (float)aveG / (float)aveR;  
  g_balanceG = 1.f;
  g_balanceB = (float)aveG / (float)aveB;  
  printf("aveR = %d\n", aveR);
  printf("aveG = %d\n", aveG);
  printf("aveB = %d\n", aveB);
  printf("g_balanceR = %f\n", g_balanceR);
  printf("g_balanceG = %f\n", g_balanceG);
  printf("g_balanceB = %f\n", g_balanceB);
}

void frame_recording_info_show()
{
  printf("Recording...\n");
  sprintf(g_text_buf, "[Recording...]");
  cvPutText(RGBImg, g_text_buf, cvPoint(0,150), &g_font, CV_RGB(255, 0, 0));
  cvShowImage("SSP Viewer", RGBImg);
  reset_fps_start_time();
}

void frame_realtime_preview_info_show()
{
  cvCvtColor( BayerImg, RGBImg, CV_BayerRG2RGB);
  sprintf(g_text_buf, "[Realtime Monitoring Mode]");
  cvPutText(RGBImg, g_text_buf, cvPoint(0,30), &g_font, CV_RGB(255, 0, 0));
  sprintf(g_text_buf, "[R] to Recording");
  cvPutText(RGBImg, g_text_buf, cvPoint(0,60), &g_font, CV_RGB(255, 0, 0));

  // get fps
  double fps = get_fps();

  // show fps
  sprintf(g_text_buf,"FrameRate = %f", fps);
  cvPutText(RGBImg, g_text_buf, cvPoint(0,90), &g_font, CV_RGB(255, 0, 0));

  sprintf(g_text_buf,"WB R=%1.3f G=%1.3f B=%1.3f", g_balanceR, g_balanceG, g_balanceB);
  cvPutText(RGBImg, g_text_buf, cvPoint(0,120), &g_font, CV_RGB(255, 0, 0));
}

void frame_realtime_preview(struct ssp_frame *frame)
{
  unsigned char *frame_data = ssp_get_frame_data(frame);

  frame_apply_easy_white_balance(frame_data);
  frame_realtime_preview_info_show();

  int key = cvWaitKey(2) & 0xff;

  if (key == KEY_CMD_RECORDING) {
    g_frame_mode_flg = FRAME_MODE_RECORDING;
    frame_valiables_init();
    frame_recording_info_show();
    cvWaitKey(1);
  }
  else if (key == KEY_CMD_QUIT) {
    stop_process = 1;
  }
  else if (key == KEY_CMD_GETBALANCE) {
    get_easy_white_balance_profile(frame_data, frame);
  }

  cvShowImage("SSP Viewer", RGBImg);

  update_fps_base();
}

void frame_recording(struct ssp_frame *frame)
{
    g_frame_array[g_save_frame++] = frame; // just record frame (release it later) (do not use frame #0)
    if (g_save_frame < g_max_frame) {     // record frames until the buffer is full
      g_frame_count++;                     // next frame
    }
    else {
      get_time_passed();
      g_frame_mode_flg = FRAME_MODE_PREVIEW; // go to preview mode
      g_fno = 0;
      g_pause_mode = PAUSE_MODE_RUNNING;
      g_frame_count = 0;
    }
}

void fame_preprocess(struct ssp_handle *handle, struct ssp_frame *frame)
{
  if (g_frame_mode_flg == FRAME_MODE_RECORDING) {
    // recording mode
    frame_recording(frame);
  }
  else if (g_frame_mode_flg == FRAME_MODE_PREVIEW) {
    // preview mode
    frame_preview();
    ssp_release_frame(frame);            // discard current frame
  }
  else if (g_frame_mode_flg == FRAME_MODE_REALTIMEPREVIEW) {
    // realtime preview
    frame_realtime_preview(frame);
    ssp_release_frame(frame);            // discard current frame
  }
}

void process_parameters(int argc, char **argv, struct ssp_handle *handle)
{
  int analog_gain;
  int digital_gain;
  int coarse_integration_time;
  int RegVal; 

  // analog gain
  if (argc >= 3) {
    analog_gain = atoi(argv[2]);
    printf("param:analog_gain=%02x\n", analog_gain);
    RegVal = analog_gain; 
    __CCIRegWrite(handle, 0x157, RegVal);
  }
  else {
    RegVal = 0xe0; // e0=>01
    printf("default:analog_gain=%02x\n", RegVal);
    __CCIRegWrite(handle, 0x157, RegVal);
  }

  // digital gain
  if (argc >= 4) {
    digital_gain = atoi(argv[3]);
    printf("param:digital_gain=%04x\n", digital_gain);
    RegVal = (digital_gain & 0xff00) >> 8; 
    __CCIRegWrite(handle, 0x158, RegVal);
    RegVal = (digital_gain & 0x00ff); 
    __CCIRegWrite(handle, 0x159, RegVal);
  }
  else {
    RegVal = 0x03; // 03=>00
    __CCIRegWrite(handle, 0x158, RegVal);

    RegVal = 0x00; // 00
    __CCIRegWrite(handle, 0x159, RegVal);
    printf("default:digital_gain=%04x\n", 0x0300);
  }

  // coarse_integration_time
  if (argc >= 5) {
    coarse_integration_time = atoi(argv[4]);
    printf("param:coars_integration_time=%04x\n", coarse_integration_time);
    RegVal = (coarse_integration_time & 0xff00) >> 8; 
    __CCIRegWrite(handle, 0x15A, RegVal);
    RegVal = (coarse_integration_time & 0x00ff); 
    __CCIRegWrite(handle, 0x15B, RegVal);
  }


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
  int retval;

  profile_filename = argv[1];

  stop_process = 0;
  if (signal(SIGINT, signal_handler) == SIG_ERR)
    return 2;

  struct ssp_settings settings;

  g_frame_count = 0;
  
  settings.lib_settings.NumFrameFIFOSize = 3; // 2:mandatory
  settings.lib_settings.NumThreadForBuildInPreprocess = 4;
  settings.lib_settings.frame_drop_cam_user_func = frame_drop_func;
  settings.lib_settings.frame_drop_pre_user_func = frame_drop_preprocess_func;
  settings.lib_settings.fame_preprocess_user_func = fame_preprocess;
  settings.frame_preprocess_options.FormatConvertType = SSP_FRAME_BAYER8;
  settings.frame_preprocess_options.GammaCorrectionEnable = GAMMA_OPTION;
  settings.frame_preprocess_options.GammaVal = 2.2;
  settings.camera_settings.PowerOnResetUsec = 1000000;

  struct ssp_handle *handle;

  
  ssp_read_profile(profile_filename, &profile);

  printf("Setting summary: %d x %d @ %d fps %d bits\n",
	 profile->ImageProperty.Width,
	 profile->ImageProperty.Height,
	 profile->ImageProperty.Frequency,
	 profile->ImageProperty.BayerBits);

  // OpenCV Settings
  cvNamedWindow("SSP Viewer", CV_WINDOW_AUTOSIZE); 

  printf("ImageProperty.Width =%d\n", profile->ImageProperty.Width);
  printf("ImageProperty.Height=%d\n", profile->ImageProperty.Height);

  BayerImg = cvCreateImage(cvSize(profile->ImageProperty.Width,profile->ImageProperty.Height),IPL_DEPTH_8U,1);
  RGBImg = cvCreateImage(cvSize(profile->ImageProperty.Width,profile->ImageProperty.Height),IPL_DEPTH_8U,3);
  
  cvInitFont(&g_font, CV_FONT_HERSHEY_SIMPLEX, 1.0f, 1.0f, 0, 1, 8);

  int i;
  for(i=0;i<1;i++){ // for repeat initialization test.
    if((retval = ssp_initialize(&handle, profile, &settings))!=SSP_SUCCESS){
      printf("Can not initialize ssp library. %x\n",retval);
      return 0;
    }

    process_parameters(argc, argv, handle);

    printf("Initialize done.\n");
    printf("start streaming\n");
    if((retval = ssp_start_streaming(handle)) != SSP_SUCCESS){
	
      printf("Can not start streaming. %x\n",retval);
      return 0;
    }

    ssp_flush_event(handle);

    ////////////////////////////////////////
    init_frame_array();
    reset_fps_start_time();
    ////////////////////////////////////////

    cvWaitKey(2);

    while(1){ if(stop_process==1) break;}

    ////////////////////////////////////////
    discard_frame_array();
    ////////////////////////////////////////

    printf("stop streaming\n");
    if((retval = ssp_stop_streaming(handle)) != SSP_SUCCESS){
      printf("Can not start streaming. %x\n",retval);
      return 0;
    }

    printf("Flush Wait.\n");
    ssp_flush_event(handle);
    printf("Flush Done.\n");

//finalize:

    if(ssp_finalize(handle) != SSP_SUCCESS){
      printf("Error during finalization.\n");
      return 0;
    }
    
    if (g_video_writer != NULL) {
      cvReleaseVideoWriter(&g_video_writer);
    }
    cvReleaseImage(&RGBImg);
    cvReleaseImage(&BayerImg);
    cvDestroyWindow("SSP Viewer");
    
    printf("Finalize done.\n");fflush(stdout);
  }
  return 1;
}

