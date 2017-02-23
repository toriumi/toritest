/**
 * @file      main_app.h
 * @brief     Header for MainApp Class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */
#ifndef _MAIN_APP
#define _MAIN_APP

#include "./include.h"
#include "./main_wnd.h"

/**
 * @class MainApp
 * @brief Vision Processing Framework App class
 */
class MainApp : public wxApp {
 public:
  /**
   * @brief
   * Constructor.
   */
  MainApp(void);

  /**
   * @brief
   * Destructor.
   */
  virtual ~MainApp(void);

  /**
   * @brief
   * MainApp initialization processing.
   */
  virtual bool OnInit(void);

  /**
   * @brief
   * MainApp end processing.
   */
  virtual int OnExit(void);
};

IMPLEMENT_APP(MainApp)

#endif /* _MAIN_APP*/
