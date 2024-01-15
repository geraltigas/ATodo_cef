//
// Created by geraltigas on 24-1-15.
//

#ifndef BROWSER_APP_H
#define BROWSER_APP_H
#include <include/cef_app.h>
#include <include/cef_base.h>
#include <include/cef_browser_process_handler.h>
#include <include/wrapper/cef_helpers.h>

// Minimal implementation of CefApp for the browser process.
class BrowserApp final : public CefApp, public CefBrowserProcessHandler {
public:
    BrowserApp() = default;
    // CefApp methods:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

    // CefBrowserProcessHandler methods:
    void OnContextInitialized() override;
private:
    IMPLEMENT_REFCOUNTING(BrowserApp);
};


#endif //BROWSER_APP_H
