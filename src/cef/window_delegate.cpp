//
// Created by geraltigas on 24-1-15.
//

#include "window_delegate.h"

void WindowDelegate::OnWindowCreated(const CefRefPtr<CefWindow> window) {
        // Add the browser view and show the window.
        window->AddChildView(browser_view_);
        window->Show();

        // Give keyboard focus to the browser view.
        browser_view_->RequestFocus();
}

void WindowDelegate::OnWindowDestroyed(CefRefPtr<CefWindow> window) {
                browser_view_ = nullptr;
}

bool WindowDelegate::CanClose(CefRefPtr<CefWindow> window) {
    // Allow the window to close if the browser says it's OK.
    if (const CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser())
        return browser->GetHost()->TryCloseBrowser();
    return true;
}

CefSize WindowDelegate::GetPreferredSize(CefRefPtr<CefView> view) {
    // Preferred window size.
    return {800, 600};
}

CefSize WindowDelegate::GetMinimumSize(CefRefPtr<CefView> view) {
    // Minimum window size.
    return {200, 100};
}


