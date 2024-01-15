//
// Created by geraltigas on 24-1-15.
//

#ifndef WINDOW_DELEGATE_H
#define WINDOW_DELEGATE_H
#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>
#include <include/views/cef_window_delegate.h>


class WindowDelegate final : public CefWindowDelegate {
public:
    explicit WindowDelegate(const CefRefPtr<CefBrowserView> &browser_view)
        : browser_view_(browser_view) {};

    void OnWindowCreated(CefRefPtr<CefWindow> window) override;

    void OnWindowDestroyed(CefRefPtr<CefWindow> window) override;

    bool CanClose(CefRefPtr<CefWindow> window) override;

    CefSize GetPreferredSize(CefRefPtr<CefView> view) override;

    CefSize GetMinimumSize(CefRefPtr<CefView> view) override;

private:
    CefRefPtr<CefBrowserView> browser_view_;
    IMPLEMENT_REFCOUNTING(WindowDelegate);
};


#endif //WINDOW_DELEGATE_H
