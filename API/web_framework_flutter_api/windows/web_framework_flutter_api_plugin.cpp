#include "web_framework_flutter_api_plugin.h"

#include <filesystem>

// This must be included before many other Windows headers.
#include <Windows.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

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
		if (methodCall.method_name() == "getLibraryPath")
		{
			wchar_t currentPath[4096];

			GetModuleFileNameW(nullptr, currentPath, 4096);

			std::filesystem::path realPath(currentPath);

			realPath.remove_filename() /= "WebFramework.dll";

			result->Success(realPath.string());
		}
		else
		{
			result->NotImplemented();
		}
	}
}