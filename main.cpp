#include <QApplication>
#include <QMainWindow>
#include <include/cef_app.h>

#include "cef/CefWidget.h"

int main(int argc, char *argv[]) {
  // Initialize CEF.
  const CefMainArgs main_args(argc, argv);
  const CefRefPtr<CefApp> app(new MyCefApp);
  const CefSettings settings;
  // Set a custom cache path
  CefString(&settings.root_cache_path) = "/tmp/cache";

  CefInitialize(main_args, settings, app.get(), nullptr);

  // Initialize Qt Application.
  QApplication qtApp(argc, argv);

  // Create the main window.
  QMainWindow mainWindow;

  // Create and set the CefBrowserWidget as the central widget of the main window.
  auto* cefBrowserWidget = new CefWidget(&mainWindow, 800, 600);
  mainWindow.setCentralWidget(cefBrowserWidget);

  // set size window
  mainWindow.resize(800, 600);
  cefBrowserWidget->resize(800, 600);

  // Show the main window.
  mainWindow.show();
  cefBrowserWidget->Cef.join();
  // Execute the Qt application loop.
  int qtResult = QApplication::exec();


  // Shut down CEF after the Qt application exits.
  CefShutdown();

  return qtResult;
}

