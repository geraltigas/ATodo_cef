#include <QApplication>
#include <include/cef_app.h>
#include <X11/Xlib.h>
#include "shared/app_factory.h"
#include "shared/client_manager.h"

int XErrorHandlerImpl(Display* display, XErrorEvent* event) {
  LOG(WARNING) << "X error received: "
               << "type " << event->type << ", "
               << "serial " << event->serial << ", "
               << "error_code " << static_cast<int>(event->error_code) << ", "
               << "request_code " << static_cast<int>(event->request_code)
               << ", "
               << "minor_code " << static_cast<int>(event->minor_code);
  return 0;
}

int XIOErrorHandlerImpl(Display* display) {
  return 0;
}

CefRefPtr<CefCommandLine> CreateCommandLine(const CefMainArgs& main_args) {
  CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
#if defined(OS_WIN)
  command_line->InitFromString(::GetCommandLineW());
#else
  command_line->InitFromArgv(main_args.argc, main_args.argv);
#endif
  return command_line;
}
enum ProcessType {
  PROCESS_TYPE_BROWSER,
  PROCESS_TYPE_RENDERER,
  PROCESS_TYPE_OTHER,
};

namespace {

  // These flags must match the Chromium values.
  const char kProcessType[] = "type";
  const char kRendererProcess[] = "renderer";
#if defined(OS_LINUX)
  const char kZygoteProcess[] = "zygote";
#endif

}  // namespace
ProcessType GetProcessType(const CefRefPtr<CefCommandLine>& command_line) {
  // The command-line flag won't be specified for the browser process.
  if (!command_line->HasSwitch(kProcessType))
    return PROCESS_TYPE_BROWSER;

  const std::string& process_type = command_line->GetSwitchValue(kProcessType);
  if (process_type == kRendererProcess)
    return PROCESS_TYPE_RENDERER;

#if defined(OS_LINUX)
  // On Linux the zygote process is used to spawn other process types. Since we
  // don't know what type of process it will be we give it the renderer app.
  if (process_type == kZygoteProcess)
    return PROCESS_TYPE_RENDERER;
#endif

  return PROCESS_TYPE_OTHER;
}

int main(int argc, char *argv[]) {
  // // Initialize CEF.
  // const CefMainArgs main_args(argc, argv);
  // const CefRefPtr<CefApp> app(new MyCefApp);
  // const CefSettings settings;
  // // Set a custom cache path
  // CefString(&settings.root_cache_path) = "/tmp/cache";
  //
  // CefInitialize(main_args, settings, app.get(), nullptr);
  //
  // // Initialize Qt Application.
  // QApplication qtApp(argc, argv);
  //
  // // Create the main window.
  // QMainWindow mainWindow;
  //
  // // Create and set the CefBrowserWidget as the central widget of the main window.
  // auto* cefBrowserWidget = new CefWidget(&mainWindow, 800, 600);
  // mainWindow.setCentralWidget(cefBrowserWidget);
  //
  // // set size window
  // mainWindow.resize(800, 600);
  // cefBrowserWidget->resize(800, 600);
  //
  // // Show the main window.
  // mainWindow.show();
  // cefBrowserWidget->Cef.join();
  // // Execute the Qt application loop.
  // int qtResult = QApplication::exec();
  //
  //
  // // Shut down CEF after the Qt application exits.
  // CefShutdown();
  //
  // return qtResult;

  // Provide CEF with command-line arguments.
  CefMainArgs main_args(argc, argv);

  // Create a temporary CommandLine object.
  CefRefPtr<CefCommandLine> command_line = CreateCommandLine(main_args);

  // Create a CefApp of the correct process type.
  CefRefPtr<CefApp> app = shared::CreateBrowserProcessApp();

  // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
  // that share the same executable. This function checks the command-line and,
  // if this is a sub-process, executes the appropriate logic.
  int exit_code = CefExecuteProcess(main_args, app, nullptr);
  if (exit_code >= 0) {
    // The sub-process has completed so return here.
    return exit_code;
  }

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

