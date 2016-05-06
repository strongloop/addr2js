// Copyright (c) IBM Corp. 2014. All Rights Reserved.
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include "node.h"
#include "v8.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define UNREACHABLE abort
#define ASSERT assert
#define CHECK assert

namespace {

FILE* stream;

void JitCodeEventHandler(const v8::JitCodeEvent* event) {
  if (event->type == v8::JitCodeEvent::CODE_ADDED) {
    fprintf(stream,
            "A %lx %zu %.*s\n",
            reinterpret_cast<uintptr_t>(event->code_start),
            event->code_len,
            static_cast<int>(event->name.len),
            event->name.str);
  } else if (event->type == v8::JitCodeEvent::CODE_MOVED) {
    fprintf(stream,
            "M %lx %zu %lx\n",
            reinterpret_cast<uintptr_t>(event->code_start),
            event->code_len,
            reinterpret_cast<uintptr_t>(event->new_code_start));
  } else if (event->type == v8::JitCodeEvent::CODE_REMOVED) {
    fprintf(stream,
            "R %lx %zu\n",
            reinterpret_cast<uintptr_t>(event->code_start),
            event->code_len);
  } else {
    fprintf(stream,
            "? %lx %zu %d\n",
            reinterpret_cast<uintptr_t>(event->code_start),
            event->code_len,
            event->type);
  }
  fflush(stream);
}

void Initialize(v8::Handle<v8::Object>) {
  char filename[256];
  snprintf(filename, sizeof(filename), "/tmp/addr2js-%u.map", getpid());
  stream = fopen(filename, "w");
  if (stream == NULL) {
    fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    return;
  }
  v8::V8::SetJitCodeEventHandler(v8::kJitCodeEventEnumExisting,
                                 JitCodeEventHandler);
}

}

NODE_MODULE(addr2js, Initialize)
