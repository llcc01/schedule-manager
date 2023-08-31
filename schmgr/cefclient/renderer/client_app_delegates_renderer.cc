// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "schmgr/cefclient/renderer/client_renderer.h"
#include "schmgr/shared/renderer/client_app_renderer.h"
#include "schmgr/cefclient/renderer/schmgr_delegate.h"

namespace client {

// static
void ClientAppRenderer::CreateDelegates(DelegateSet& delegates) {
  renderer::CreateDelegates(delegates);
  schmgr_delegate::CreateDelegates(delegates);
}

}  // namespace client
