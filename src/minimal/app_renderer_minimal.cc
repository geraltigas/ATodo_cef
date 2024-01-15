// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "shared/app_factory.h"

namespace shared {

// No CefApp for the renderer subprocess.
CefRefPtr<CefApp> CreateRendererProcessApp() {
  return nullptr;
}

}  // namespace shared
