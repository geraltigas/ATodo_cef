//
// Created by geraltigas on 24-1-15.
//

#include "atodo_app.h"

#include <include/views/cef_window.h>

#include "atodo_client.h"
#include "window_delegate.h"

class CefBrowserView;

CefRefPtr<CefBrowserProcessHandler> BrowserApp::GetBrowserProcessHandler() {
    return this;
}

void BrowserApp::OnContextInitialized() {
    // Create the browser window.

    // Create the browser using the Views framework if "--use-views" is specified
    // via the command-line. Otherwise, create the browser using the native
    // platform framework. The Views framework is currently only supported on
    // Windows and Linux.

    // if (const CefRefPtr<CefCommandLine> command_line =
    //         CefCommandLine::GetGlobalCommandLine(); command_line->HasSwitch("use-views")) {
    //     // Create the BrowserView.
    //     const CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
    //         new ATodoClient, kStartupURL, CefBrowserSettings(), nullptr, nullptr, nullptr);
    //
    //     // Create the Window. It will show itself after creation.
    //     CefWindow::CreateTopLevelWindow(new WindowDelegate(browser_view));
    // } else {
    //     // Information used when creating the native window.
    //     const CefWindowInfo window_info;
    //
    //     // Create the browser window.
    //     CefBrowserHost::CreateBrowser(window_info, new ATodoClient, kStartupURL, CefBrowserSettings(),
    //                                   nullptr, nullptr);
    //
    // }
}
