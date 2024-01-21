//
// Created by geraltigas on 24-1-15.
//

#ifndef ATODO_CLIENT_H
#define ATODO_CLIENT_H
#include <include/cef_client.h>

#include "widget/cef_widget.h"

class ATodoClient final : public CefClient,
                          public CefLifeSpanHandler {
public:
    ATodoClient(CefWidget* parent) : parentWidget(parent){};

    // CefClient methods:
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

    // CefLifeSpanHandler methods:
    bool DoClose(CefRefPtr<CefBrowser> browser) override;
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
private:
    CefWidget* parentWidget;
    IMPLEMENT_REFCOUNTING(ATodoClient);
};


#endif //ATODO_CLIENT_H
