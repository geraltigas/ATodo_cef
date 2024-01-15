//
// Created by geraltigas on 24-1-15.
//

#include "atodo_client.h"

#include <include/cef_app.h>
#include <include/wrapper/cef_helpers.h>

bool ATodoClient::DoClose(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();
    CefQuitMessageLoop();
    return false;
}
