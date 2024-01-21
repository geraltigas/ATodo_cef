//
// Created by geraltigas on 24-1-17.
//

#ifndef CEF_WIDGET_H
#define CEF_WIDGET_H

#include <QWidget>
#include <QWindow>
#include <QVBoxLayout>

#include "cef/atodo_app.h"
#include "include/cef_browser.h"

#include <QWidget>
#include <QResizeEvent>

#include <QWidget>
#include <QTimer>
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_life_span_handler.h"

class CefWidget : public QWidget {
    Q_OBJECT
public:
    explicit CefWidget(QWidget* parent = nullptr);
    virtual ~CefWidget();

    void setCefBrowser(CefRefPtr<CefBrowser> browser);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void initializeCefBrowser();

    CefRefPtr<CefBrowser> cefBrowser;
};

class MyClientHandler : public CefClient, public CefLifeSpanHandler {
public:
    MyClientHandler(CefWidget* parent);

    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;

    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

private:
    CefWidget* parentWidget;

    IMPLEMENT_REFCOUNTING(MyClientHandler);
};

#endif //CEF_WIDGET_H
