//
// Created by geraltigas on 24-1-15.
//

#ifndef CEFCLIENT_H
#define CEFCLIENT_H


#include "include/cef_client.h"

class SCefClient : public CefClient,
                        public CefLifeSpanHandler {
public:
    SCefClient();

    // Provide access to the single global instance of this object.
    static SCefClient* GetInstance();

    // CefClient methods
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
        return this;
    }

    // CefLifeSpanHandler methods
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    bool DoClose(CefRefPtr<CefBrowser> browser) override;
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

private:
    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(SCefClient);
};



#endif //CEFCLIENT_H
