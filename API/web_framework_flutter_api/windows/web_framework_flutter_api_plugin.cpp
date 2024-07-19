#include "web_framework_flutter_api_plugin.h"

// This must be included before many other Windows headers.
#include <Windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

namespace web_framework_flutter_api
{
	void WebFrameworkFlutterApiPlugin::RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar)
	{
		std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>
			(
				registrar->messenger(), "web_framework_flutter_api",
				&flutter::StandardMethodCodec::GetInstance()
			);
		std::unique_ptr<WebFrameworkFlutterApiPlugin> plugin = std::make_unique<WebFrameworkFlutterApiPlugin>();

		channel->SetMethodCallHandler
		(
			[plugin = plugin.get()](const flutter::MethodCall<flutter::EncodableValue>& methodCall, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
			{
				plugin->onMethodCall(methodCall, std::move(result));
			}
		);

		registrar->AddPlugin(std::move(plugin));
	}

	void WebFrameworkFlutterApiPlugin::onMethodCall(const flutter::MethodCall<flutter::EncodableValue>& methodCall, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
	{
		if (methodCall.method_name().compare("getPlatformVersion") == 0)
		{
			std::ostringstream version_stream;
			version_stream << "Windows ";

			if (IsWindows10OrGreater())
			{
				version_stream << "10+";
			}
			else if (IsWindows8OrGreater())
			{
				version_stream << "8";
			}
			else if (IsWindows7OrGreater())
			{
				version_stream << "7";
			}

			result->Success(flutter::EncodableValue(version_stream.str()));
		}
		else
		{
			result->NotImplemented();
		}
	}
}
