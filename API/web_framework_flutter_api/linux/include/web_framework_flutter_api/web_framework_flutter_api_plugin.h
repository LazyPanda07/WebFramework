#pragma once

#include <flutter_linux/flutter_linux.h>

G_BEGIN_DECLS

#ifdef FLUTTER_PLUGIN_IMPL
#define FLUTTER_PLUGIN_EXPORT __attribute__((visibility("default")))
#else
#define FLUTTER_PLUGIN_EXPORT
#endif

typedef struct _WebFrameworkFlutterApiPlugin WebFrameworkFlutterApiPlugin;
typedef struct
{
	GObjectClass parent_class;
} WebFrameworkFlutterApiPluginClass;

FLUTTER_PLUGIN_EXPORT GType web_framework_flutter_api_plugin_get_type();

FLUTTER_PLUGIN_EXPORT void web_framework_flutter_api_plugin_register_with_registrar(FlPluginRegistrar* registrar);

G_END_DECLS
