//
// Created by geraltigas on 24-1-15.
//

#ifndef CEFWIDGET_H
#define CEFWIDGET_H

#include <QWidget>
#include <thread>
#include <include/cef_app.h>

#include "SCefClient.h"
#include "include/cef_browser.h"

class MyCefApp : public CefApp {
    // Use the macro to handle reference counting
    IMPLEMENT_REFCOUNTING(MyCefApp);
};

class CefWidget : public QWidget {
    Q_OBJECT

public:
    std::thread Cef;
    explicit CefWidget(QWidget* parent = nullptr, int width = 800, int height = 600) : QWidget(parent) {
        setAttribute(Qt::WA_NativeWindow);
        setAttribute(Qt::WA_DontCreateNativeAncestors);

        // ... [Additional initialization code]

        CefWindowInfo window_info;
        const CefBrowserSettings browser_settings;
        window_info.bounds.width = width;
        window_info.bounds.height = height;

        // Set the browser as a child of the current widget
        window_info.SetAsChild(this->winId(), CefRect(0, 0, width, height));

        const CefRefPtr<SCefClient> client = SCefClient::GetInstance();

        browser = CefBrowserHost::CreateBrowserSync(window_info, client.get() , "https://www.google.com", browser_settings, nullptr, nullptr);
        // Define CefWindowInfo for the DevTools window
        const CefWindowInfo devToolsWindowInfo;
        // Open DevTools
        browser->GetHost()->ShowDevTools(devToolsWindowInfo, SCefClient::GetInstance(), CefBrowserSettings(), CefPoint());

        Cef = std::thread([&]() {
            CefRunMessageLoop();
        });
    }

    // ... [Additional methods, like resize event handler]
private:
    CefRefPtr<CefBrowser> browser;
};

#endif //CEFWIDGET_H
