#include <QApplication>
#include <QCefView.h>
#include <QCefContext.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Create a main window to host the QCefView widget
  QWidget mainWindow;
  mainWindow.resize(800, 600);  // Set the size of the window

  QCefConfig config;

  QCefContext cef_context = QCefContext::instance();

  QCefSetting *setting = new QCefSetting;

  // Create QCefView instance
  QCefView* cefView = new QCefView("https://www.google.com",setting,&mainWindow);
  cefView->resize(800, 600);  // Resize QCefView to fill the main window

  // Load a web page
  cefView->navigateToUrl("https://www.google.com");

  // Show the main window with the QCefView widget
  mainWindow.show();

  // Execute the application
  return QApplication::exec();
}
