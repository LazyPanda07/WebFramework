#pragma once

#include "dll_handler.h"

/**
 * @brief Get error message from exception or NULL if exception is NULL
 * @param exception web_framework_exception_t
 * @return Error message or NULL if exception is NULL
 */
const char* wf_get_error_message(web_framework_exception_t exception);
