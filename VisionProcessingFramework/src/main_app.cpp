/**
 * @file      main_app.cpp
 * @brief     Source for MainApp class
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./main_app.h"

/**
 * @brief
 * Constructor.
 */
MainApp::MainApp() { ; }

/**
 * @brief
 * Destructor.
 */
MainApp::~MainApp() { DEBUG_PRINT("MainApp::~MainApp()\n"); }

/**
 * @brief
 * MainApp initialization processing.
 */
bool MainApp::OnInit() {
  /* Main window create a class object*/
  DEBUG_PRINT("MainApp::OnInit()\n");
  MainWnd* main_wnd = new MainWnd();

  /* Open Main window.*/
  main_wnd->Show(true);

  /* Set the Main window to the top window*/
  SetTopWindow(main_wnd);

  return true;
}

/**
 * @brief
 * MainApp end processing.
 */
int MainApp::OnExit() { return 0; }
