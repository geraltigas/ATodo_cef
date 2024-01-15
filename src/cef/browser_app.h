//
// Created by geraltigas on 24-1-15.
//

#ifndef BROWSER_APP_H
#define BROWSER_APP_H
#include <include/cef_base.h>
#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>

#include "shared/client_util.h"

const char kStartupURL[] = "https://www.google.com";

class WindowDelegate : public CefWindowDelegate {
public:
    explicit WindowDelegate(CefRefPtr<CefBrowserView> browser_view)
        : browser_view_(browser_view) {}

    void OnWindowCreated(CefRefPtr<CefWindow> window) override {
        // Add the browser view and show the window.
        window->AddChildView(browser_view_);
        window->Show();

        // Give keyboard focus to the browser view.
        browser_view_->RequestFocus();
    }

    void OnWindowDestroyed(CefRefPtr<CefWindow> window) override {
        browser_view_ = nullptr;
    }

    bool CanClose(CefRefPtr<CefWindow> window) override {
        // Allow the window to close if the browser says it's OK.
        CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
        if (browser)
            return browser->GetHost()->TryCloseBrowser();
        return true;
    }

    CefSize GetPreferredSize(CefRefPtr<CefView> view) override {
        // Preferred window size.
        return CefSize(800, 600);
    }

    CefSize GetMinimumSize(CefRefPtr<CefView> view) override {
        // Minimum window size.
        return CefSize(200, 100);
    }

private:
    CefRefPtr<CefBrowserView> browser_view_;

    IMPLEMENT_REFCOUNTING(WindowDelegate);
    DISALLOW_COPY_AND_ASSIGN(WindowDelegate);
};

class Client : public CefClient,
               public CefDisplayHandler,
               public CefLifeSpanHandler {
public:
    Client();

    // CefClient methods:
    CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

    // CefDisplayHandler methods:
    void OnTitleChange(CefRefPtr<CefBrowser> browser,
                       const CefString& title) override;

    // CefLifeSpanHandler methods:
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    bool DoClose(CefRefPtr<CefBrowser> browser) override;
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

private:
    IMPLEMENT_REFCOUNTING(Client);
    DISALLOW_COPY_AND_ASSIGN(Client);
};


Client::Client() {}

void Client::OnTitleChange(CefRefPtr<CefBrowser> browser,
                           const CefString& title) {
    // Call the default shared implementation.
    shared::OnTitleChange(browser, title);
}

void Client::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    // Call the default shared implementation.
    shared::OnAfterCreated(browser);
}

bool Client::DoClose(CefRefPtr<CefBrowser> browser) {
    // Call the default shared implementation.
    return shared::DoClose(browser);
}

void Client::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
    // Call the default shared implementation.
    return shared::OnBeforeClose(browser);
}



// Minimal implementation of CefApp for the browser process.
class BrowserApp : public CefApp, public CefBrowserProcessHandler {
public:
    BrowserApp() {}

    // CefApp methods:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
        return this;
    }

    void OnBeforeCommandLineProcessing(
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line) override {
        // Command-line flags can be modified in this callback.
        // |process_type| is empty for the browser process.
        if (process_type.empty()) {
#if defined(OS_MACOSX)
            // Disable the macOS keychain prompt. Cookies will not be encrypted.
            command_line->AppendSwitch("use-mock-keychain");
#endif
        }
    }

    // CefBrowserProcessHandler methods:
    void OnContextInitialized() override {
        // Create the browser window.

        CefRefPtr<CefCommandLine> command_line =
            CefCommandLine::GetGlobalCommandLine();

        // Create the browser using the Views framework if "--use-views" is specified
        // via the command-line. Otherwise, create the browser using the native
        // platform framework. The Views framework is currently only supported on
        // Windows and Linux.
        const bool use_views = command_line->HasSwitch("use-views");

        if (use_views) {
            // Create the BrowserView.
            CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
                new Client, kStartupURL, CefBrowserSettings(), nullptr, nullptr, nullptr);

            // Create the Window. It will show itself after creation.
            CefWindow::CreateTopLevelWindow(new WindowDelegate(browser_view));
        } else {
            // Information used when creating the native window.
            CefWindowInfo window_info;

            // Create the browser window.
            CefBrowserHost::CreateBrowser(window_info, new Client, kStartupURL, CefBrowserSettings(),
                                          nullptr, nullptr);

        }
    }
private:
    IMPLEMENT_REFCOUNTING(BrowserApp);
    DISALLOW_COPY_AND_ASSIGN(BrowserApp);
};


#endif //BROWSER_APP_H
