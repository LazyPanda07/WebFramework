#pragma once

#include <stdbool.h>

void initialize();

void configConstructors(bool* configuration, bool* basePath, bool* rawConfiguration);

void configOverrideString(bool* assertTrue);

void configOverrideInteger(bool* assertTrue);

void configOverrideBool(bool* assertTrue);

void configOverrideStringArray(bool* assertFalse);

void configOverrideIntegerArray(bool* assertFalse);
|