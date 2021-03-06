// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/synchronization/waitable_event_watcher.h"

#include "base/compiler_specific.h"
#include "base/synchronization/waitable_event.h"
#include "base/win/object_watcher.h"

namespace base {

WaitableEventWatcher::ObjectWatcherHelper::ObjectWatcherHelper(
    WaitableEventWatcher* watcher)
    : watcher_(watcher) {
};

void WaitableEventWatcher::ObjectWatcherHelper::OnObjectSignaled(HANDLE h) {
  watcher_->OnObjectSignaled();
}


WaitableEventWatcher::WaitableEventWatcher()
    : ALLOW_THIS_IN_INITIALIZER_LIST(helper_(this)),
      event_(NULL),
      delegate_(NULL) {
}

WaitableEventWatcher::~WaitableEventWatcher() {
}

bool WaitableEventWatcher::StartWatching(WaitableEvent* event,
                                         Delegate* delegate) {
  delegate_ = delegate;
  event_ = event;

  return watcher_.StartWatching(event->handle(), &helper_);
}

void WaitableEventWatcher::StopWatching() {
  delegate_ = NULL;
  event_ = NULL;
  watcher_.StopWatching();
}

WaitableEvent* WaitableEventWatcher::GetWatchedEvent() {
  return event_;
}

void WaitableEventWatcher::OnObjectSignaled() {
  WaitableEvent* event = event_;
  Delegate* delegate = delegate_;
  event_ = NULL;
  delegate_ = NULL;
  DCHECK(event);

  delegate->OnWaitableEventSignaled(event);
}

}  // namespace base
