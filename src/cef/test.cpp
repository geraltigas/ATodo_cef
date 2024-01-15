//
// Created by geraltigas on 24-1-14.
//
#include <cef/test.h>

#include "include/cef_app.h"
#include "include/cef_client.h"

class SimpleHandler : public CefClient, public CefLifeSpanHandler {
public:
    SimpleHandler() = default;
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
        return this;
    }
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
        DCHECK(CefCurrentlyOn(TID_UI));
        // Save the browser reference
        browser_ = browser;
    }
    bool DoClose(CefRefPtr<CefBrowser> browser) override {
        DCHECK(CefCurrentlyOn(TID_UI));
        // Closing the main window, so quit the application.
        CefQuitMessageLoop();
        return true;
    }
private:
    CefRefPtr<CefBrowser> browser_;
    IMPLEMENT_REFCOUNTING(SimpleHandler);
};

class MyCefApp : public CefApp {
    // Use the macro to handle reference counting
    IMPLEMENT_REFCOUNTING(MyCefApp);
};

void test_cef(int argc, char* argv[]) {
    CefMainArgs main_args(argc, argv);
    CefRefPtr<CefApp> app(new MyCefApp);

    // Initialize CEF
    CefSettings settings;
    CefInitialize(main_args, settings, app.get(), nullptr);

    CefWindowInfo window_info;
    // Configure window_info as required

    CefBrowserSettings browser_settings;
    // Configure browser_settings as required

    CefRefPtr<CefRequestContext> request_context = nullptr; // or a valid request context if you have one

    // The call to CreateBrowserSync
    CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(
        window_info,
        new SimpleHandler(),
        "http://www.google.com",
        browser_settings,
        nullptr, // nullptr if you don't have a custom context
        nullptr// Include this only if your version of CEF requires the 'no_javascript_access' parameter
    );

    // Define CefWindowInfo for the DevTools window
    CefWindowInfo devToolsWindowInfo;

    // Open DevTools
    browser->GetHost()->ShowDevTools(devToolsWindowInfo, new SimpleHandler(), CefBrowserSettings(), CefPoint());


    // Run the CEF message loop
    CefRunMessageLoop();

    // Shut down CEF
    CefShutdown();
}