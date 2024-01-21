#include <QApplication>
#include "widget/cef_widget.h"
#include "include/cef_app.h"

int main(int argc, char *argv[]) {
  // Correctly initialize CefMainArgs for Linux
  CefMainArgs main_args(argc, argv);

  CefSettings settings;
  settings.no_sandbox = true;
  settings.multi_threaded_message_loop = false;
  settings.external_message_pump = true;

  // Pass main_args instead of argc, argv directly
  CefInitialize(main_args, settings, nullptr, nullptr);

  QApplication app(argc, argv);

  CefWidget* cefWidget = new CefWidget();
  cefWidget->show();

  int result = app.exec();

  CefShutdown();
  return result;
}
