#include "include/web_framework_flutter_api/web_framework_flutter_api_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "web_framework_flutter_api_plugin.h"

void WebFrameworkFlutterApiPluginCApiRegisterWithRegistrar(FlutterDesktopPluginRegistrarRef registrar)
{
	web_framework_flutter_api::WebFrameworkFlutterApiPlugin::RegisterWithRegistrar
	(
		flutter::PluginRegistrarManager::GetInstance()->GetRegistrar<flutter::PluginRegistrarWindows>(registrar)
	);
}
