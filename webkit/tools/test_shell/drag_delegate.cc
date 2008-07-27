// Copyright 2008, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "webkit/tools/test_shell/drag_delegate.h"

#include <atltypes.h>

#include "webkit/glue/webview.h"

namespace {

void GetCursorPositions(HWND hwnd, CPoint* client, CPoint* screen) {
  // GetCursorPos will fail if the input desktop isn't the current desktop.
  // See http://b/1173534. (0,0) is wrong, but better than uninitialized.
  if (!GetCursorPos(screen))
    screen->SetPoint(0, 0);

  *client = *screen;
  ScreenToClient(hwnd, client);
}

}  // anonymous namespace

void TestDragDelegate::OnDragSourceCancel() {
  OnDragSourceDrop();
}

void TestDragDelegate::OnDragSourceDrop() {
  CPoint client;
  CPoint screen;
  GetCursorPositions(source_hwnd_, &client, &screen);
  webview_->DragSourceEndedAt(client.x, client.y, screen.x, screen.y);
}
void TestDragDelegate::OnDragSourceMove() {
  CPoint client;
  CPoint screen;
  GetCursorPositions(source_hwnd_, &client, &screen);
  webview_->DragSourceMovedTo(client.x, client.y, screen.x, screen.y);
}
