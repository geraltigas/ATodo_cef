#include <include/cef_app.h>
#include <X11/Xlib.h>
#include <cef/x_error_handler.h>

#include "cef/browser_app.h"
#include "shared/client_manager.h"

int main(int argc, char *argv[]) {
  // Provide CEF with command-line arguments.
  CefMainArgs main_args(argc, argv);

  // Create a CefApp of the correct process type.
  CefRefPtr<CefApp> app = new BrowserApp();

  // Install xlib error handlers so that the application won't be terminated
  // on non-fatal errors.
  XSetErrorHandler(XErrorHandlerImpl);
  XSetIOErrorHandler(XIOErrorHandlerImpl);

  // Create the singleton manager instance.
  shared::ClientManager manager;

  // Specify CEF global settings here.
  CefSettings settings;
  // set cache path
  CefString(&settings.root_cache_path) = "/tmp/cache";

  // Initialize CEF for the browser process. The first browser instance will be
  // created in CefBrowserProcessHandler::OnContextInitialized() after CEF has
  // been initialized.
  CefInitialize(main_args, settings, app, nullptr);

  // Run the CEF message loop. This will block until CefQuitMessageLoop() is
  // called.
  CefRunMessageLoop();

  // Shut down CEF.
  CefShutdown();

  return 0;
}

