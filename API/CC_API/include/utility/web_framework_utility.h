#pragma once

#include "../DLLHandler.h"

#define DEFAULT_CHUNK_SIZE 14680064

/**
 * @brief Generate UUID in format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
 * @param result UUID. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
WebFrameworkException generateUUID(WebFrameworkString* result);
