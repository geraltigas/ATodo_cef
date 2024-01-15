//
// Created by geraltigas on 24-1-15.
//

#include "SCefClient.h"

SCefClient::SCefClient() = default;

SCefClient* SCefClient::GetInstance() {
    static auto* instance = new SCefClient();
    return instance;
}

void SCefClient::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    // This method is called after the browser is created.
}

bool SCefClient::DoClose(CefRefPtr<CefBrowser> browser) {
    // Return false if the browser can be closed.
    return false;
}

void SCefClient::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
    // This method is called before the browser window is closed.
}
