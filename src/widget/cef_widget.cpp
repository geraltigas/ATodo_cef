//
// Created by geraltigas on 24-1-17.
//

#include "cef_widget.h"

#include "cef/atodo_client.h"

CefWidget::CefWidget(QWidget* parent) : QWidget(parent) {
    initializeCefBrowser();
}

CefWidget::~CefWidget() {
    // Properly close and clean up CEF browser instance
}

void CefWidget::setCefBrowser(CefRefPtr<CefBrowser> browser) {
    cefBrowser = browser;
}

void CefWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    if (cefBrowser.get()) {
        cefBrowser->GetHost()->WasResized();
    }
}

void CefWidget::initializeCefBrowser() {
    CefWindowInfo window_info;
    window_info.SetAsChild(this->winId(),
                           CefRect(0, 0, width(), height()));

    CefBrowserSettings browser_settings;
    CefRefPtr<CefClient> client = new MyClientHandler(this);

    // Note the addition of nullptr as the last argument
    CefBrowserHost::CreateBrowser(window_info, client, "http://example.com", browser_settings, nullptr, nullptr);
}

MyClientHandler::MyClientHandler(CefWidget* parent) : parentWidget(parent) {}

CefRefPtr<CefLifeSpanHandler> MyClientHandler::GetLifeSpanHandler() {
    return this;
}

void MyClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();
    if (!browser->IsPopup()) {
        parentWidget->setCefBrowser(browser);
    }
}