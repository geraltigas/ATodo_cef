//
// Created by geraltigas on 24-1-15.
//

#ifndef ATODO_CLIENT_H
#define ATODO_CLIENT_H
#include <include/cef_client.h>


class ATodoClient final : public CefClient,
                          public CefLifeSpanHandler {
public:
    ATodoClient() = default;

    // CefClient methods:
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

    // CefLifeSpanHandler methods:
    bool DoClose(CefRefPtr<CefBrowser> browser) override;
private:
    IMPLEMENT_REFCOUNTING(ATodoClient);
};


#endif //ATODO_CLIENT_H
