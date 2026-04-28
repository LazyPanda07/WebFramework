#pragma once

#include "WebFrameworkUtilityCApi.h"

typedef void* FrameObject;

EXPORT char* getFramePayload(FrameObject frame, uint64_t* size, Exception* exception);

EXPORT int getFrameType(FrameObject frame, Exception* exception);
