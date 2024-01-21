//
// Created by geraltigas on 24-1-17.
//

#include <QTimer>
#include <thread>
#include <include/internal/cef_linux.h>
#include <include/internal/cef_ptr.h>

#include "atodo_app.h"
#include "error/x_error_handler.h"
#include <cef/setup_cef.h>

class QTimer;
class CefApp;

std::thread cefThread;

int init_cef(int argc, char ** argv) {
    // init cef
    const CefMainArgs main_args(argc, argv);
    const CefRefPtr<CefApp> app = new BrowserApp();

    // Install xlib error handlers so that the application won't be terminated
    // on non-fatal errors.
    XSetErrorHandler(XErrorHandlerImpl);
    XSetIOErrorHandler(XIOErrorHandlerImpl);

    CefSettings settings;
    settings.no_sandbox = true;
    settings.multi_threaded_message_loop = false;
    settings.external_message_pump = true;
    CefString(&settings.root_cache_path) = "/tmp/cache";

    CefInitialize(main_args, settings, app, nullptr);
    cefThread = std::thread([]() {
    CefRunMessageLoop();
    });
    return 0;
}

int shutdown_cef() {
    // Later, when shutting down
    CefQuitMessageLoop();
    cefThread.join();
    CefShutdown();
    return 0;
}
