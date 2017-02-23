/**
 * @file      output_disp_opengl.cpp
 * @brief     Display plugin by using OpenGL.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */
#include "./output_disp_opengl.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>

static CUBE_STATE_T _state, *state = &_state;

/**
 * @brief
 * Constructor.
 */
OutputDispOpengl::OutputDispOpengl() : PluginBase() {
  DEBUG_PRINT("OutputDispOpengl::OutputDispOpengl()\n");

  set_plugin_name("OutputDispOpengl");

  AddInputPortCandidateSpec(kGRAY8);
  AddInputPortCandidateSpec(kGRAY16);
  AddInputPortCandidateSpec(kRGB888);
  AddInputPortCandidateSpec(kRGB48);

  set_is_use_dest_buffer(false);

  disp_info_.default_x = 0;
  disp_info_.default_y = 0;
  disp_info_.default_width = 0;
  disp_info_.default_height = 0;
  disp_info_.default_alpha = 255;
  disp_info_.current_x = 0;
  disp_info_.current_y = 0;
  disp_info_.current_width = 0;
  disp_info_.current_height = 0;
  disp_info_.current_alpha = disp_info_.default_alpha;
  disp_info_.is_disp = true;
  disp_info_.is_update = true;
  disp_info_.disp_state = kNormal;
}

/**
 * @brief
 * Destructor.
 */
OutputDispOpengl::~OutputDispOpengl() {}

/**
 * @brief
 * Initialize routine of the OutputDispOpengl plugin.
 * @param common [in] commom parameters.
 * @return If true, successful initialization.
 */
bool OutputDispOpengl::InitProcess(CommonParam* common) {
  DEBUG_PRINT("OutputDispOpengl::InitProcess \n");
  common_ = common;

  is_initialized_ = false;
  is_first_run_ = true;
  before_image_size_ = cvSize(0, 0);

  display_ = XOpenDisplay(NULL);
  if (display_ == NULL) {
    fprintf(stderr, "Cannot open display.\n");
    exit(1);
  }
  int state;
  XGetInputFocus(display_, &window_, &state);
  XSelectInput(display_, window_, ExposureMask | KeyPressMask);

  gc_ = XCreateGC(display_, window_, 0, 0);

  XFlush(display_);

  event_handling_thread_ =
      new EventHandlingThread(display_, window_, gc_, &disp_info_);
  if (event_handling_thread_ == NULL) {
    return false;
  }
  if (event_handling_thread_->Create() != wxTHREAD_NO_ERROR) {
    return false;
  }

  XGrabKeyboard(display_, window_, false, GrabModeAsync, GrabModeAsync,
                CurrentTime);

  event_handling_thread_->Run();

  return true;
}

/**
 * @brief
 * Finalize routine of the OutputDispOpengl plugin.
 */
void OutputDispOpengl::EndProcess() {
  DEBUG_PRINT("OutputDispOpengl::EndProcess \n");

  event_handling_thread_->Delete();
  if (event_handling_thread_->IsRunning()) {
    event_handling_thread_->Wait();
  }
  delete event_handling_thread_;
  event_handling_thread_ = NULL;

  before_image_size_ = cvSize(0, 0);

  ExitOgl();

  XUngrabKeyboard(display_, CurrentTime);
  XCloseDisplay(display_);
}

/**
 * @brief
 * Post-processing routine of the OutputDispOpengl plugin.
 * This function is empty implementation.
 */
void OutputDispOpengl::DoPostProcess(void) { ; }

/**
 * @brief
 * Main routine of the OutputDispOpengl plugin.
 * @param src_ipl [in] src image data.
 * @param dst_ipl [out] dst image data.
 * @return If true, success in the main processing.
 */
bool OutputDispOpengl::DoProcess(cv::Mat* src_image, cv::Mat* dst_image) {
  if (src_image == NULL) {
    DEBUG_PRINT("[OutputDispOpenGL]src_image == NULL\n");
    return false;
  }

  // If the size of the image has changed, executing the initialization
  CvSize current_image_size = src_image->size();
  if ((current_image_size.width != before_image_size_.width) ||
      (current_image_size.height != before_image_size_.height)) {
    DEBUG_PRINT(
        "[OutputDispOpengl] src size change before w = %d h = %d current w = "
        "%d h = %d \n",
        before_image_size_.width, before_image_size_.height,
        current_image_size.width, current_image_size.height);
    // Setting the drawing size to the size of the src_image.
    disp_info_.default_width = src_image->size().width;
    disp_info_.default_height = src_image->size().height;
    disp_info_.current_width = disp_info_.default_width;
    disp_info_.current_height = disp_info_.default_height;
    is_initialized_ = false;
  }
  before_image_size_ = src_image->size();

  // If detects a key operation, set up the initialization flag.
  if (disp_info_.is_update == true) {
    disp_info_.is_update = false;
    is_initialized_ = false;
  }

  if (is_initialized_ == false) {
    if (is_first_run_ == false) {
      ExitOgl();
    } else {
      is_first_run_ = false;
    }

    bcm_host_init();
    DEBUG_PRINT("end bcm_host_init()\n");

    // Clear application state
    memset(state, 0, sizeof(*state));
    DEBUG_PRINT("end memset()\n");

    // Start OGLES
    if (InitOgl(state, current_image_size) == false) {
      return false;
    }

    DEBUG_PRINT("end InitOgl()\n");
    InitTexture(state, src_image);

    is_initialized_ = true;
  } else {
    UpdateTexture(state, src_image);
  }
  DEBUG_PRINT("end InitTexture()\n");
  DrawImage(state);
  DEBUG_PRINT("end DrawImage()\n");

  return true;
}

/**
 * @brief
 * Initialize function of OpenGL.
 * @param state [in] drawing information
 * @param image_size [in] image size
 * @return If true, successful initialize.
 */
bool OutputDispOpengl::InitOgl(CUBE_STATE_T* state, CvSize image_size) {
  DEBUG_PRINT("OutputDispOpengl:InitOgl start\n");
  int32_t success = 0;
  EGLBoolean result;
  EGLint num_config;
  static EGL_DISPMANX_WINDOW_T nativewindow;
  DISPMANX_UPDATE_HANDLE_T dispman_update;
  VC_RECT_T dst_rect;
  VC_RECT_T src_rect;
  EGLConfig config;

  // get an EGL display connection
  state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (state->display == EGL_NO_DISPLAY) {
    PLUGIN_LOG_ERROR("Can't get display");
    DEBUG_PRINT("Can't get display\n");
    return false;
  }

  // initialize the EGL display connection
  result = eglInitialize(state->display, NULL, NULL);
  if (EGL_FALSE == result) {
    PLUGIN_LOG_ERROR("Failed EGL initialize");
    DEBUG_PRINT("Failed EGL initialize\n");
    return false;
  }

  // get an appropriate EGL frame buffer configuration
  result =
      eglChooseConfig(state->display, attribute_list, &config, 1, &num_config);
  if (EGL_FALSE == result) {
    PLUGIN_LOG_ERROR("Failed choose EGL Config");
    DEBUG_PRINT("Failed choose EGL Config\n");
    return false;
  }

  // create an EGL rendering context
  state->context =
      eglCreateContext(state->display, config, EGL_NO_CONTEXT, NULL);
  if (state->context == EGL_NO_CONTEXT) {
    PLUGIN_LOG_ERROR("Failed create EGL Context")
    DEBUG_PRINT("Failed create EGL Context\n");
    return false;
  }

  // create an EGL window surface
  success = graphics_get_display_size(0 /* LCD */, &state->screen_width,
                                      &state->screen_height);
  if (success < 0) {
    PLUGIN_LOG_ERROR("Can not get graphics display size");
    DEBUG_PRINT("Can not get graphics display size\n");
    return false;
  } else {
    disp_info_.disp_width = state->screen_width;
    disp_info_.disp_height = state->screen_height;
    printf("screen size w = %d h = %d\n", disp_info_.default_width,
           disp_info_.default_height);
  }

  // set display size
  dst_rect.x = disp_info_.current_x;
  dst_rect.y = disp_info_.current_y;
  dst_rect.width = disp_info_.current_width;
  dst_rect.height = disp_info_.current_height;

  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width = state->screen_width << 16;
  src_rect.height = state->screen_height << 16;

  state->dispman_display = vc_dispmanx_display_open(0 /* LCD */);
  dispman_update = vc_dispmanx_update_start(0);
  VC_DISPMANX_ALPHA_T alpha = {DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS,
                               disp_info_.current_alpha, 0};  // add
  state->dispman_element = vc_dispmanx_element_add(
      dispman_update, state->dispman_display, 0 /*layer*/, &dst_rect, 0 /*src*/,
      &src_rect, DISPMANX_PROTECTION_NONE, &alpha /*alpha*/, 0 /*clamp*/,
      DISPMANX_NO_ROTATE /*transform*/);

  nativewindow.element = state->dispman_element;
  nativewindow.width = state->screen_width;
  nativewindow.height = state->screen_height;
  vc_dispmanx_update_submit_sync(dispman_update);

  state->surface =
      eglCreateWindowSurface(state->display, config, &nativewindow, NULL);
  if (state->surface == EGL_NO_SURFACE) {
    PLUGIN_LOG_ERROR("Failed EGL Window surface");
    DEBUG_PRINT("Failed EGL Window surface\n");
    return false;
  }

  // connect the context to the surface
  result = eglMakeCurrent(state->display, state->surface, state->surface,
                          state->context);
  if (EGL_FALSE == result) {
    PLUGIN_LOG_ERROR("Can not make EGL current");
    DEBUG_PRINT("Can not make EGL current\n");
    return false;
  }

  // Set background color and clear buffers
  glClearColor(1.0f, 0.25f, 0.35f, 1.0f);

  // Enable back face culling.
  glEnable(GL_CULL_FACE);

  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glMatrixMode(GL_MODELVIEW);

  glGenBuffers(1, &state->vrt_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, state->vrt_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, vertex, GL_STATIC_DRAW);

  glGenBuffers(1, &state->tex_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, state->tex_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, texCoords, GL_STATIC_DRAW);

  glGenBuffers(1, &state->idx_vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state->idx_vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, indices,
               GL_STATIC_DRAW);

  DEBUG_PRINT("OutputDispOpengl:InitOgl end\n");

  return true;
}

/**
 * @brief
 * Initialize function of the texture.
 * @param state [in] drawing information
 * @param image [in] Pointer to the Mat class
 * @return If true, successful initialize.
 */
bool OutputDispOpengl::InitTexture(CUBE_STATE_T* state, cv::Mat* image) {
  DEBUG_PRINT("OutputDispOpengl:InitTexture start\n");
  cv::Mat* temp_image;

  temp_image = ConvertImage(image);
  if (temp_image == NULL) {
    return false;
  }

  if (image->channels() == 1) {
    state->internal_format = GL_LUMINANCE;
    state->image_format = GL_LUMINANCE;
  } else if (image->channels() == 3) {
    state->internal_format = GL_RGB;
    state->image_format = GL_RGB;
  }

  DEBUG_PRINT("temp_image size w = %d, h = %d\n", temp_image->size().width,
              temp_image->size().height);
  state->tex_buf = (unsigned char*)temp_image->data;

  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &state->tex);
  glBindTexture(GL_TEXTURE_2D, state->tex);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_HINT, GL_TRUE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, state->internal_format, image->size().width,
               image->size().height, 0, state->image_format, GL_UNSIGNED_BYTE,
               state->tex_buf);

  if (image->depth() == CV_16U) {
    delete temp_image;
    temp_image = NULL;
  }
  DEBUG_PRINT("OutputDispOpengl:InitTexture end\n");
  return true;
}

/**
 * @brief
 * Update function of the texture.
 * @param state [in] drawing information
 * @param image [in] Pointer to the Mat class
 * @return If true, successful update.
 */
bool OutputDispOpengl::UpdateTexture(CUBE_STATE_T* state, cv::Mat* image) {
  cv::Mat* temp_image;
  temp_image = ConvertImage(image);
  if (temp_image == NULL) {
    return false;
  }

  state->tex_buf = (unsigned char*)temp_image->data;
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, state->tex);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->size().width,
                  image->size().height, state->image_format, GL_UNSIGNED_BYTE,
                  state->tex_buf);

  if (image->depth() == CV_16U) {
    delete temp_image;
    temp_image = NULL;
  }
  return true;
}

/**
 * @brief
 * Flinalization function of OpenGL.
 */
void OutputDispOpengl::ExitOgl() {
  DEBUG_PRINT("OpenGLDisp ExitOgl start\n");
  DISPMANX_UPDATE_HANDLE_T dispman_update;
  int s;

  glClear(GL_COLOR_BUFFER_BIT);
  glDeleteTextures(1, &(state->tex));
  glDeleteBuffers(1, &state->vrt_vbo);
  glDeleteBuffers(1, &state->tex_vbo);
  glDeleteBuffers(1, &state->idx_vbo);
  bool success = eglDestroySurface(state->display, state->surface);
  if (success == false) {
    PLUGIN_LOG_ERROR("Failed destroy egl surface");
    DEBUG_PRINT("Failed destroy egl surface\n");
  }

  dispman_update = vc_dispmanx_update_start(0);
  s = vc_dispmanx_element_remove(dispman_update, state->dispman_element);
  if (s != 0) {
    PLUGIN_LOG_ERROR("Failed remove dispmanx element");
    DEBUG_PRINT("Failed remove dispmanx element\n");
  }

  vc_dispmanx_update_submit_sync(dispman_update);
  s = vc_dispmanx_display_close(state->dispman_display);
  if (s != 0) {
    PLUGIN_LOG_ERROR("Failed close dispmanx display");
    DEBUG_PRINT("Failed close dispmanx display\n");
  }

  // Release OpenGL resources
  eglMakeCurrent(state->display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                 EGL_NO_CONTEXT);
  eglDestroyContext(state->display, state->context);
  eglTerminate(state->display);
  DEBUG_PRINT("OpenGLDisp ExitOgl end\n");
}

/**
 * @brief
 * Drawing function.
 * @param state [in] drawing information
 */
void OutputDispOpengl::DrawImage(CUBE_STATE_T* state) {
  glClear(GL_COLOR_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D, state->tex);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  /* vertex */
  glBindBuffer(GL_ARRAY_BUFFER, state->vrt_vbo);
  glVertexPointer(2, GL_FLOAT, 0, 0);

  /* texture */
  glBindBuffer(GL_ARRAY_BUFFER, state->tex_vbo);
  glTexCoordPointer(2, GL_FLOAT, 0, 0);

  /* index */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state->idx_vbo);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  eglSwapBuffers(state->display, state->surface);
}

/**
 * @brief
 * Convert the image.
 * @param src_image [in] Pointer to the src image
 * @return Pointer to the converted image.
 */
cv::Mat* OutputDispOpengl::ConvertImage(cv::Mat* image) {
  cv::Mat* temp_image;

  if (image->depth() == CV_16U) {
    temp_image = UtilGetCvConvertScale(image, UTIL_CONVERT_10U_TO_8U, 0);
    if (temp_image == NULL) {
      return NULL;
    }
  } else {
    temp_image = image;
  }
  return temp_image;
}

/**
 * @brief
 * Convert the bit depth of image.
 * @param src_image [in] Pointer to the src image
 * @param cvt_mode [in] enum UtilConvertMode
 * @param shift [in] Value added to the scaled source array elements
 * @return Pointer to the converted image.
 */
cv::Mat* OutputDispOpengl::UtilGetCvConvertScale(cv::Mat* src_image,
                                                 int cvt_mode, double shift) {
  cv::Mat* dst_image;
  int src_channels = src_image->channels();
  int max_width = src_image->size().width * src_channels;
  double scale;

  if (cvt_mode == UTIL_CONVERT_10U_TO_8U) {
    switch (src_channels) {
      case 1:
        dst_image = new cv::Mat(
            cvSize(src_image->size().width, src_image->size().height), CV_8UC1);
        break;
      case 3:
        dst_image = new cv::Mat(
            cvSize(src_image->size().width, src_image->size().height), CV_8UC3);
        break;
      default:
        return NULL;
    }
    scale = 1.0 / 4.0;  // change scale 10bit -> 8bit
  } else if (cvt_mode == UTIL_CONVERT_10U_TO_16U) {
    switch (src_channels) {
      case 1:
        dst_image = new cv::Mat(
            cvSize(src_image->size().width, src_image->size().height),
            CV_16UC1);
        break;
      case 3:
        dst_image = new cv::Mat(
            cvSize(src_image->size().width, src_image->size().height),
            CV_16UC3);
        break;
      default:
        return NULL;
    }
    scale = 64.0;  // change scale 10bit -> 16bit
  } else {
    DEBUG_PRINT("Invalid convert mode =%d return NULL", cvt_mode);
    return NULL;
  }

  if (cvt_mode == UTIL_CONVERT_10U_TO_8U) {
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < dst_image->size().height; i++) {
      for (int j = 0; j < max_width; j += src_channels) {
        ((unsigned char*)(dst_image->data + dst_image->step * i))[j] =
            (reinterpret_cast<INT16*>(src_image->data +
                                      src_image->step * i))[j] *
                scale +
            shift;
        if (src_channels == 3) {
          ((unsigned char*)(dst_image->data + dst_image->step * i))[j + 1] =
              (reinterpret_cast<INT16*>(src_image->data +
                                        src_image->step * i))[j + 1] *
                  scale +
              shift;
          ((unsigned char*)(dst_image->data + dst_image->step * i))[j + 2] =
              (reinterpret_cast<INT16*>(src_image->data +
                                        src_image->step * i))[j + 2] *
                  scale +
              shift;
        }
      }
    }
  } else if (cvt_mode == UTIL_CONVERT_10U_TO_16U) {
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int i = 0; i < dst_image->size().height; i++) {
      for (int j = 0; j < max_width; j += src_channels) {
        // 16bit code
        (reinterpret_cast<INT16*>(dst_image->data + dst_image->step * i))[j] =
            (reinterpret_cast<INT16*>(src_image->data +
                                      src_image->step * i))[j] *
                scale +
            shift;
        if (src_channels == 3) {
          (reinterpret_cast<INT16*>(dst_image->data +
                                    dst_image->step * i))[j + 1] =
              (reinterpret_cast<INT16*>(src_image->data +
                                        src_image->step * i))[j + 1] *
                  scale +
              shift;
          (reinterpret_cast<INT16*>(dst_image->data +
                                    dst_image->step * i))[j + 2] =
              (reinterpret_cast<INT16*>(src_image->data +
                                        src_image->step * i))[j + 2] *
                  scale +
              shift;
        }
      }
    }
  } else {
    DEBUG_PRINT("Invalid convert mode =%d return NULL", cvt_mode);
    return NULL;
  }
  return dst_image;
}

extern "C" PluginBase* Create(void) {
  DEBUG_PRINT("Create Template plugins\n");
  OutputDispOpengl* plugin = new OutputDispOpengl();
  return plugin;
}

extern "C" void Destroy(PluginBase* plugin) { delete plugin; }
