#include <include/cef_app.h>
#include <X11/Xlib.h>
#include <error/x_error_handler.h>
#include <cef/atodo_app.h>

int main(int argc, char ** argv) {
  // init cef
  const CefMainArgs main_args(argc, argv);
  const CefRefPtr<CefApp> app = new BrowserApp();

  // Install xlib error handlers so that the application won't be terminated
  // on non-fatal errors.
  XSetErrorHandler(XErrorHandlerImpl);
  XSetIOErrorHandler(XIOErrorHandlerImpl);

  CefSettings settings;
  CefString(&settings.root_cache_path) = "/tmp/cache";

  CefInitialize(main_args, settings, app, nullptr);

  CefRunMessageLoop();

  CefShutdown();
  return 0;
}

