#include <QApplication>
#include <QCefView.h>
#include <QCefContext.h>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDir>

#define URL_ROOT "https://atodo"

int main(int argc, char *argv[]) {
#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
  // For off-screen rendering, Qt::AA_EnableHighDpiScaling must be enabled. If not,
  // then all devicePixelRatio methods will always return 1.0,
  // so CEF will not scale the web content
  // NOET: There is bugs in Qt 6.2.4, the HighDpi doesn't work
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif

  // create QApplication instance
  QApplication a(argc, argv);
  QDir dir = QCoreApplication::applicationDirPath();
  QString webResourceDir = /*QString("file://") +*/ QDir::toNativeSeparators(dir.filePath("webres"));
  // build QCefConfig
  QCefConfig config;
  // set user agent
  config.setUserAgent("QCefViewTest");
  // set log level
  config.setLogLevel(QCefConfig::LOGSEVERITY_DEFAULT);
  // set JSBridge object name (default value is QCefViewClient)
  config.setBridgeObjectName("CallBridge");
  // port for remote debugging (default is 0 and means to disable remote debugging)
  config.setRemoteDebuggingPort(9000);
  // set background color for all browsers
  // (QCefSetting.setBackgroundColor will overwrite this value for specified browser instance)
  config.setBackgroundColor(Qt::lightGray);

  // WindowlessRenderingEnabled is set to true by default,
  // set to false to disable the OSR mode
  config.setWindowlessRenderingEnabled(true);

  // add command line args, you can any cef supported switches or parameters
  config.addCommandLineSwitch("use-mock-keychain");
  // config.addCommandLineSwitch("disable-gpu");
  // config.addCommandLineSwitch("enable-media-stream");
  // config.addCommandLineSwitch("allow-file-access-from-files");
  // config.addCommandLineSwitch("disable-spell-checking");
  // config.addCommandLineSwitch("disable-site-isolation-trials");
  // config.addCommandLineSwitch("enable-aggressive-domstorage-flushing");
  config.addCommandLineSwitchWithValue("renderer-process-limit", "1");
  // allow remote debugging
  config.addCommandLineSwitchWithValue("remote-allow-origins", "*");
  // config.addCommandLineSwitchWithValue("disable-features", "BlinkGenPropertyTrees,TranslateUI,site-per-process");

#if defined(Q_OS_MACOS) && defined(QT_DEBUG)
  // cef bugs on macOS debug build
  config.setCachePath(QDir::tempPath());
#endif

  // create QCefContext instance with config,
  // the lifecycle of cefContext must be the same as QApplication instance
  QCefContext cefContext(&a, argc, argv, &config);

  QCefContext::instance()->addLocalFolderResource(webResourceDir, URL_ROOT);

  QCefSetting setting;

  setting.setWindowlessFrameRate(60);
  setting.setBackgroundColor(QColor::fromRgba(qRgba(255, 255, 220, 255)));
  QVBoxLayout* layout = new QVBoxLayout();
  QWidget* w = new QWidget();
  // ./webres/atodo.html
  auto* view = new QCefView("https://atodo/atodo/index.html", &setting, w);
  // add a button
  view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  auto* btn = new QPushButton("Hello");

  layout->addWidget(view);
  layout->addWidget(btn);

  // set button callback
    QObject::connect(btn, &QPushButton::clicked, [view]() {
        view->showDevTools();
    });


  w->setLayout(layout);
  w->show();

  return QApplication::exec();
}
