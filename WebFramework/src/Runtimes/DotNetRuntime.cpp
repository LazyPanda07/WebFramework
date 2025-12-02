#ifndef __LINUX__
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "DotNetRuntime.h"

#include <format>
#include <cstdlib>

#include <Log.h>
#include <JsonBuilder.h>

#include "Utility/DynamicLibraries.h"

#ifdef __WITH_DOT_NET_EXECUTORS__

static void errorHandler(const char_t* message)
{
	if (Log::isValid())
	{
		Log::error(".NET error: {}", "LogRuntime", framework::runtime::DotNetRuntime::NativeString(message).string());
	}

	throw std::runtime_error(std::format(".NET error: {}", framework::runtime::DotNetRuntime::NativeString(message).string()));
}

namespace framework::runtime
{
	std::filesystem::path DotNetRuntime::getPathToRuntimeConfig()
	{
		return std::filesystem::temp_directory_path() / "web_framework_runtimeconfig.json";
	}

	void DotNetRuntime::createRuntimeConfig()
	{
		constexpr int dotNetVersion = 8;

		std::ofstream stream(DotNetRuntime::getPathToRuntimeConfig());
		json::JsonBuilder builder(CP_UTF8);
		json::JsonObject runtimeOptions;
		json::JsonObject framework;

		framework["name"] = "Microsoft.NETCore.App";
		framework["version"] = std::format("{}.0.0", dotNetVersion);

		runtimeOptions["tfm"] = std::format("net{}.0", dotNetVersion);
		runtimeOptions["framework"] = std::move(framework);

		builder["runtimeOptions"] = std::move(runtimeOptions);

		stream << builder;
	}

	DotNetRuntime::NativeString DotNetRuntime::getModuleName(const std::filesystem::path& modulePath)
	{
		return std::filesystem::path(modulePath).filename().replace_extension();
	}

	DotNetRuntime::NativeString DotNetRuntime::getModuleName(std::string_view modulePath)
	{
		return std::filesystem::path(modulePath).filename().replace_extension();
	}

	void DotNetRuntime::loadFunctions(const std::filesystem::path& pathToSource)
	{
		NativeString moduleName = DotNetRuntime::getModuleName(pathToSource);
		NativeString typeName = std::format("Framework.Utility.Utils, {}", moduleName.string());

		this->loadMethod(typeName, "HasExecutor", hasExecutor);
		this->loadMethod(typeName, "Free", dotNetFree);
		this->loadMethod(typeName, "Dealloc", dotNetDealloc);
		this->loadMethod(typeName, "Init", init);
		this->loadMethod(typeName, "CreateExecutor", createExecutor);
		this->loadMethod(typeName, "CreateDynamicFunction", createDynamicFunction);
		this->loadMethod(typeName, "CreateHttpRequest", createHttpRequest);
		this->loadMethod(typeName, "CreateHttpResponse", createHttpResponse);
		this->loadMethod(typeName, "CreateExecutorSettings", createExecutorSettingsFunction);
		this->loadMethod(typeName, "GetExecutorType", getExecutorType);
		this->loadMethod(typeName, "Destroy", destroy);

		this->loadMethod(typeName, "CallDoPost", doPost);
		this->loadMethod(typeName, "CallDoGet", doGet);
		this->loadMethod(typeName, "CallDoHead", doHead);
		this->loadMethod(typeName, "CallDoPut", doPut);
		this->loadMethod(typeName, "CallDoDelete", doDelete);
		this->loadMethod(typeName, "CallDoPatch", doPatch);
		this->loadMethod(typeName, "CallDoOptions", doOptions);
		this->loadMethod(typeName, "CallDoTrace", doTrace);
		this->loadMethod(typeName, "CallDoConnect", doConnect);

		this->loadMethod(typeName, "CallInvoke", callDynamicFunction);
	}

	template<FunctionPointer T>
	void DotNetRuntime::loadMethod(const NativeString& typeName, const NativeString& methodName, T& method)
	{
		if (method)
		{
			throw std::runtime_error(std::format("Method already intialized, type name: {}, method name: {}", typeName.string(), methodName.string()));
		}

		getFunctionPointer(typeName.native().data(), methodName.native().data(), UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr, reinterpret_cast<void**>(&method));
	}

	DotNetRuntime::DotNetRuntime() :
		hasExecutor(nullptr),
		dotNetFree(nullptr),
		dotNetDealloc(nullptr),
		createExecutor(nullptr),
		createHttpRequest(nullptr),
		createHttpResponse(nullptr),
		createExecutorSettingsFunction(nullptr),
		getExecutorType(nullptr),
		init(nullptr),
		doPost(nullptr),
		doGet(nullptr),
		doHead(nullptr),
		doPut(nullptr),
		doDelete(nullptr),
		doPatch(nullptr),
		doOptions(nullptr),
		doTrace(nullptr),
		doConnect(nullptr),
		destroy(nullptr),
		createDynamicFunction(nullptr),
		callDynamicFunction(nullptr)
	{
		constexpr size_t envSize = 512;

		const std::filesystem::path directoryPath = std::filesystem::path(utility::getPathToWebFrameworkSharedLibrary()).parent_path();
		const std::filesystem::path apiPath = directoryPath / "WebFrameworkCSharpAPI.dll";

		if (!std::filesystem::exists(apiPath))
		{
			throw std::runtime_error(std::format("Can't find {}", apiPath.string()));
		}

#ifdef __LINUX__
		std::string runtimeLibraryName = "libhostfxr.so";
#else
		std::string runtimeLibraryName = "hostfxr.dll";
#endif

		runtimeLibrary = utility::getLoadedLibrary(runtimeLibraryName);
		size_t size = 0;
		std::string runtimePathFromEnv(envSize, '\0');

		if (!runtimeLibrary)
		{
			if (runtimeLibrary = utility::loadLibrary(runtimeLibraryName); !runtimeLibrary)
			{
				throw std::runtime_error(std::format("Can't find {}", runtimeLibraryName));
			}
		}

		hostfxr_set_error_writer_fn errorHandlerSetter = utility::load<hostfxr_set_error_writer_fn>(runtimeLibrary, "hostfxr_set_error_writer");

		errorHandlerSetter(errorHandler);

		initialization = utility::load<hostfxr_initialize_for_runtime_config_fn>(runtimeLibrary, "hostfxr_initialize_for_runtime_config");
		getRuntimeDelegate = utility::load<hostfxr_get_runtime_delegate_fn>(runtimeLibrary, "hostfxr_get_runtime_delegate");
		close = utility::load<hostfxr_close_fn>(runtimeLibrary, "hostfxr_close");

		DotNetRuntime::createRuntimeConfig();

		initialization(getPathToRuntimeConfig().native().data(), nullptr, &handle);
		getRuntimeDelegate(handle, hdt_load_assembly, reinterpret_cast<void**>(&loadAssembly));
		getRuntimeDelegate(handle, hdt_get_function_pointer, reinterpret_cast<void**>(&getFunctionPointer));
		loadAssembly(apiPath.native().data(), nullptr, nullptr);

		this->loadFunctions(apiPath);
	}

	void DotNetRuntime::free(void* implementation)
	{
		dotNetFree(implementation);
	}

	void DotNetRuntime::dealloc(void* allocatedMemory)
	{
		dotNetDealloc(allocatedMemory);
	}

	bool DotNetRuntime::getExecutorFunction(std::string_view executorName, const std::filesystem::path& modulePath, CreateExecutorFunction& creator)
	{
		NativeString moduleName = DotNetRuntime::getModuleName(modulePath);
		std::string fullQualifiedName = std::format("{}, {}", executorName, moduleName.string());

		if (hasExecutor(fullQualifiedName.data()))
		{
			creator = [this, fullQualifiedName]()
				{
					return createExecutor(fullQualifiedName.data());
				};

			return true;
		}

		return false;
	}

	void DotNetRuntime::finishInitialization()
	{

	}

	void* DotNetRuntime::createExecutorSettings(const void* implementation) const
	{
		return createExecutorSettingsFunction(implementation);
	}

	void* DotNetRuntime::createHTTPRequest(framework::interfaces::IHTTPRequest* request) const
	{
		return createHttpRequest(request);
	}

	void* DotNetRuntime::createHTTPResponse(framework::interfaces::IHTTPResponse* response) const
	{
		return createHttpResponse(response);
	}

	void DotNetRuntime::initializeWebFramework(std::string_view libraryPath)
	{

	}

	std::optional<std::string> DotNetRuntime::loadSource(std::string_view pathToSource, utility::LoadSource& source)
	{
		std::filesystem::path nativePathToSource(pathToSource);

		if (!loadAssembly(nativePathToSource.native().data(), nullptr, nullptr))
		{
			source = nativePathToSource;
		}

		return std::nullopt;
	}

	DotNetRuntime::~DotNetRuntime()
	{
		if (!close)
		{
			return;
		}

		close(handle);

		close = nullptr;

		std::filesystem::remove(DotNetRuntime::getPathToRuntimeConfig());
	}
}

#endif
