/**
 * @file      output_disp_opengl.h
 * @brief     Display plugin by using OpenGL.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _OUTPUT_DISP_OPENGL_H_
#define _OUTPUT_DISP_OPENGL_H_

#include <wx/thread.h>
#include "./common_param.h"
#include "./event_handling_thread.h"
#include "./plugin_base.h"

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES/gl.h"
#include "./bcm_host.h"

#define PATH "./"

typedef struct {
  uint32_t screen_width;
  uint32_t screen_height;

  // OpenGL|ES objects
  DISPMANX_DISPLAY_HANDLE_T dispman_display;
  DISPMANX_ELEMENT_HANDLE_T dispman_element;
  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;
  GLuint tex;
  GLuint vrt_vbo;
  GLuint idx_vbo;
  GLuint tex_vbo;

  // pointers to texture buffers
  unsigned char* tex_buf;

  GLint internal_format;
  GLenum image_format;
} CUBE_STATE_T;

const GLfloat vertex[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};

const GLubyte indices[] = {0, 1, 2, 2, 3, 0};

const GLfloat texCoords[] = {0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};

const EGLint attribute_list[] = {EGL_RED_SIZE,     8,
                                 EGL_GREEN_SIZE,   8,
                                 EGL_BLUE_SIZE,    8,
                                 EGL_ALPHA_SIZE,   8,
                                 EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                                 EGL_NONE};

enum UtilConvertMode {
  UTIL_CONVERT_10U_TO_8U = 0,
  UTIL_CONVERT_10U_TO_16U,
};

/**
 * @class OutputDispOpengl
 * @brief Display plugin by using OpenGL.
 */
class OutputDispOpengl : public PluginBase {
 private:
  /*! Pointer to the CommonParameter class.*/
  CommonParam* common_;

 public:
  /**
   * @brief
   * Constructor.
   */
  OutputDispOpengl(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~OutputDispOpengl(void);

  /**
   * @brief
   * Initialize routine of the OutputDispOpengl plugin.
   * @param common [in] commom parameters.
   * @return If true, successful initialization.
   */
  virtual bool InitProcess(CommonParam* common);

  /**
   * @brief
   * Finalize routine of the OutputDispOpengl plugin.
   */
  virtual void EndProcess(void);

  /**
   * @brief
   * Post-processing routine of the OutputDispOpengl plugin.
   * This function is empty implementation.
   */
  virtual void DoPostProcess(void);

  /**
   * @brief
   * Main routine of the OutputDispOpengl plugin.
   * @param src_ipl [in] src image data.
   * @param dst_ipl [out] dst image data.
   * @return If true, success in the main processing.
   */
  virtual bool DoProcess(cv::Mat* src_image, cv::Mat* dst_image);

  void SetTitle(Display* display, Window window, char* title);

 private:
  /*! Pointer to the Display class */
  Display* display_;
  /*! Pointer to the Window class */
  Window window_;
  /*! Pointer to the GC class */
  GC gc_;
  /*! Pointer to the EventHandlingThread class */
  EventHandlingThread* event_handling_thread_;

  /*! Whether initialization is complete */
  bool is_initialized_;
  /*! Whether first run */
  bool is_first_run_;
  /*! The image size bofore 1 frame */
  CvSize before_image_size_;
  unsigned int* draw_image_buffer_;
  XImage* x_image_;
  /*! Information of the drawing */
  DisplayInfo disp_info_;
  int revert_to_;

  /**
   * @brief
   * Initialize function of OpenGL.
   * @param state [in] drawing information
   * @param image_size [in] image size
   * @return If true, successful initialize.
   */
  bool InitOgl(CUBE_STATE_T* state, CvSize image_size);

  /**
   * @brief
   * Initialize function of the texture.
   * @param state [in] drawing information
   * @param image [in] Pointer to the Mat class
   * @return If true, successful initialize.
   */
  bool InitTexture(CUBE_STATE_T* state, cv::Mat* image);

  /**
   * @brief
   * Update function of the texture.
   * @param state [in] drawing information
   * @param image [in] Pointer to the Mat class
   * @return If true, successful update.
   */
  bool UpdateTexture(CUBE_STATE_T* state, cv::Mat* image);

  /**
   * @brief
   * Flinalization function of OpenGL.
   */
  void ExitOgl(void);

  /**
   * @brief
   * Drawing function.
   * @param state [in] drawing information
   */
  void DrawImage(CUBE_STATE_T* state);

  /**
   * @brief
   * Convert the image.
   * @param src_image [in] Pointer to the src image
   * @return Pointer to the converted image.
   */
  cv::Mat* ConvertImage(cv::Mat* src_image);

  /**
   * @brief
   * Convert the bit depth of image.
   * @param src_image [in] Pointer to the src image
   * @param cvt_mode [in] enum UtilConvertMode
   * @param shift [in] Value added to the scaled source array elements
   * @return Pointer to the converted image.
   */
  cv::Mat* UtilGetCvConvertScale(cv::Mat* src_image, int cvt_mode,
                                 double shift);
};
#endif /* _OUTPUT_DISP_OPENGL_H_*/
