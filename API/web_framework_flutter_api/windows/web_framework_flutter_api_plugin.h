#pragma once

#include <memory>
#include <functional>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

namespace web_framework_flutter_api
{
	class WebFrameworkFlutterApiPlugin : public flutter::Plugin
	{
	public:
		static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

	public:
		WebFrameworkFlutterApiPlugin() = default;

		WebFrameworkFlutterApiPlugin(const WebFrameworkFlutterApiPlugin&) = delete;

		WebFrameworkFlutterApiPlugin& operator=(const WebFrameworkFlutterApiPlugin&) = delete;

		// Called when a method is called on this plugin's channel from Dart.
		void onMethodCall(const flutter::MethodCall<flutter::EncodableValue>& methodCall, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

		virtual ~WebFrameworkFlutterApiPlugin() = default;
	};
}
