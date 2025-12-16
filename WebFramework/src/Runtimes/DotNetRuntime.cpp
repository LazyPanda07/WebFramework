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

#include "Executors/CSharpExecutor.h"

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
		static int64_t processId =

#ifdef __LINUX__
			static_cast<int64_t>(getpid());
#else
			static_cast<int64_t>(GetCurrentProcessId());
#endif

		return std::filesystem::temp_directory_path() / std::format("web_framework_{}_runtimeconfig.json", processId);
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
		this->loadMethod(typeName, "CreateExecutor", createExecutorFunction);
		this->loadMethod(typeName, "CreateDynamicFunction", createDynamicFunction);
		this->loadMethod(typeName, "CreateHeuristic", createHeuristic);
		this->loadMethod(typeName, "CreateHttpRequest", createHttpRequest);
		this->loadMethod(typeName, "CreateHttpResponse", createHttpResponse);
		this->loadMethod(typeName, "CreateExecutorSettings", createExecutorSettingsFunction);
		this->loadMethod(typeName, "GetExecutorType", getExecutorTypeFunction);
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
		this->loadMethod(typeName, "CallHeuristicOnStart", onStartHeuristic);
		this->loadMethod(typeName, "CallHeuristicOnEnd", onEndHeuristic);
		this->loadMethod(typeName, "CallHeuristicInvoke", callHeuristic);
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
		createExecutorFunction(nullptr),
		createHttpRequest(nullptr),
		createHttpResponse(nullptr),
		createExecutorSettingsFunction(nullptr),
		getExecutorTypeFunction(nullptr),
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
		createHeuristic(nullptr),
		callDynamicFunction(nullptr),
		onStartHeuristic(nullptr),
		onEndHeuristic(nullptr),
		callHeuristic(nullptr)
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

		hostfxr_set_error_writer_fn errorHandlerSetter = utility::load<hostfxr_set_error_writer_fn>(runtimeLibrary, "hostfxr_set_error_writer", true);

		errorHandlerSetter(errorHandler);

		initialization = utility::load<hostfxr_initialize_for_runtime_config_fn>(runtimeLibrary, "hostfxr_initialize_for_runtime_config", true);
		getRuntimeDelegate = utility::load<hostfxr_get_runtime_delegate_fn>(runtimeLibrary, "hostfxr_get_runtime_delegate", true);
		close = utility::load<hostfxr_close_fn>(runtimeLibrary, "hostfxr_close", true);

		DotNetRuntime::createRuntimeConfig();

		initialization(getPathToRuntimeConfig().native().data(), nullptr, &handle);
		getRuntimeDelegate(handle, hdt_load_assembly, reinterpret_cast<void**>(&loadAssembly));
		getRuntimeDelegate(handle, hdt_get_function_pointer, reinterpret_cast<void**>(&getFunctionPointer));
		loadAssembly(apiPath.native().data(), nullptr, nullptr);

		this->loadFunctions(apiPath);
	}

	DotNetRuntime::InitSignature DotNetRuntime::getInit() const
	{
		return init;
	}

	DotNetRuntime::DoMethodSignature DotNetRuntime::getDoPost() const
	{
		return doPost;
	}

	DotNetRuntime::DoMethodSignature DotNetRuntime::getDoGet() const
	{
		return doGet;
	}

	DotNetRuntime::DoMethodSignature DotNetRuntime::getDoHead() const
	{
		return doHead;
	}

	DotNetRuntime::DoMethodSignature DotNetRuntime::getDoPut() const
	{
		return doPut;
	}

	DotNetRuntime::DoMethodSignature DotNetRuntime::getDoDelete() const
	{
		return doDelete;
	}

	DotNetRuntime::DoMethodSignature DotNetRuntime::getDoPatch() const
	{
		return doPatch;
	}

	DotNetRuntime::DoMethodSignature DotNetRuntime::getDoOptions() const
	{
		return doOptions;
	}

	DotNetRuntime::DoMethodSignature DotNetRuntime::getDoTrace() const
	{
		return doTrace;
	}

	DotNetRuntime::DoMethodSignature DotNetRuntime::getDoConnect() const
	{
		return doConnect;
	}

	DotNetRuntime::GetExecutorTypeSignature DotNetRuntime::getGetExecutorType() const
	{
		return getExecutorTypeFunction;
	}

	DotNetRuntime::DestroySignature DotNetRuntime::getDestroy() const
	{
		return destroy;
	}

	DotNetRuntime::CreateDynamicFunctionSignature DotNetRuntime::getCreateDynamicFunction() const
	{
		return createDynamicFunction;
	}

	DotNetRuntime::CreateHeuristicFunctionSignature DotNetRuntime::getCreateHeuristic() const
	{
		return createHeuristic;
	}

	DotNetRuntime::CallDynamicFunctionSignature DotNetRuntime::getCallDynamicFunction() const
	{
		return callDynamicFunction;
	}

	DotNetRuntime::EventSignature DotNetRuntime::getOnStartHeuristic() const
	{
		return onStartHeuristic;
	}

	DotNetRuntime::EventSignature DotNetRuntime::getOnEndHeuristic() const
	{
		return onEndHeuristic;
	}

	DotNetRuntime::CallHeuristicSignature DotNetRuntime::getCallHeuristic() const
	{
		return callHeuristic;
	}

	void DotNetRuntime::free(void* implementation)
	{
		dotNetFree(implementation);
	}

	void DotNetRuntime::dealloc(void* allocatedMemory)
	{
		dotNetDealloc(allocatedMemory);
	}

	bool DotNetRuntime::loadExecutor(std::string_view name, const utility::LoadSource& source)
	{
		if (!std::holds_alternative<std::filesystem::path>(source))
		{
			return false;
		}

		const std::filesystem::path& modulePath = std::get<std::filesystem::path>(source);
		NativeString moduleName = DotNetRuntime::getModuleName(modulePath);
		std::string fullQualifiedName = std::format("{}, {}", name, moduleName.string());

		if (!hasExecutor(fullQualifiedName.data()))
		{
			return false;
		}

		if (Log::isValid())
		{
			Log::info("Found {} in {}", "LogWebFrameworkInitialization", name, modulePath.string());
		}

		fullQualifiedNames.emplace(name, std::move(fullQualifiedName));

		return true;
	}

	std::unique_ptr<BaseExecutor> DotNetRuntime::createExecutor(std::string_view name) const
	{
		auto it = fullQualifiedNames.find(name);

		if (it == fullQualifiedNames.end())
		{
			throw std::runtime_error(std::format("Can't find executor with name {}", name));
		}

		const auto& [_, fullQualifiedName] = *it;

		return std::make_unique<CSharpExecutor>(createExecutorFunction(fullQualifiedName.data()));
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

	void DotNetRuntime::initializeWebFramework(const utility::LoadSource& source, std::string_view libraryPath)
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
