#pragma once

#include "Runtime.h"

#include <concepts>

#include <Strings.h>

#ifdef __WITH_DOT_NET_EXECUTORS__

#include <hostfxr.h>
#include <coreclr_delegates.h>

namespace framework::runtime
{
	template <typename T>
	concept FunctionPointer = 
		std::is_pointer_v<T> &&
		std::is_function_v<std::remove_pointer_t<T>>;

	class DotNetRuntime : public Runtime
	{
	public:
		using NativeString = std::filesystem::path;

	public:
		using HasExecutorSignature = bool(*)(const char* executorName);
		using DoMethodSignature = int(*)(void* executor, void* request, void* response);
		using CallDynamicFunctionSignature = char* (*)(void* dynamicFunction, const char** arguments, size_t size);
		using FreeSignature = void(*)(void* implementation);
		using InitSignature = void(*)(void* executor, void* implementation);
		using CreateExecutorSignature = void* (*)(const char* executorName);
		using CreateDynamicFunctionSignature = void* (*)(const char* dynamicFunctionName);
		using CreateHeuristicFunctionSignature = void* (*)(const char* heuristicName, const char* ip, const char* port, bool useHTTPS);
		using CreateHttpRequestSignature = void* (*)(void* implementation);
		using CreateHttpResponceSignature = void* (*)(void* implementation);
		using CreateExecutorSettingsSignature = void* (*)(const void* implementation);
		using GetExecutorTypeSignature = int(*)(void* implementation);
		using DestroySignature = void(*)(void* implementation);
		using EventSignature = void(*)(void* implementation);
		using CallHeuristicSignature = uint64_t(*)(void* implementation);

	private:
		static std::filesystem::path getPathToRuntimeConfig();
		
		static void createRuntimeConfig();

	public:
		static NativeString getModuleName(const std::filesystem::path& modulePath);

		static NativeString getModuleName(std::string_view modulePath);

	private:
		::utility::strings::string_based_unordered_map<std::string> fullQualifiedNames;
		hostfxr_initialize_for_runtime_config_fn initialization;
		hostfxr_get_runtime_delegate_fn getRuntimeDelegate;
		hostfxr_close_fn close;
		hostfxr_handle handle;
		load_assembly_fn loadAssembly;
		get_function_pointer_fn getFunctionPointer;
		HMODULE runtimeLibrary;
		HasExecutorSignature hasExecutor;
		FreeSignature dotNetFree;
		FreeSignature dotNetDealloc;
		CreateExecutorSignature createExecutorFunction;
		CreateHttpRequestSignature createHttpRequest;
		CreateHttpResponceSignature createHttpResponse;
		CreateExecutorSettingsSignature createExecutorSettingsFunction;

	public:
		InitSignature init;
		DoMethodSignature doPost;
		DoMethodSignature doGet;
		DoMethodSignature doHead;
		DoMethodSignature doPut;
		DoMethodSignature doDelete;
		DoMethodSignature doPatch;
		DoMethodSignature doOptions;
		DoMethodSignature doTrace;
		DoMethodSignature doConnect;
		GetExecutorTypeSignature getExecutorType;
		DestroySignature destroy;
		CreateDynamicFunctionSignature createDynamicFunction;
		CreateHeuristicFunctionSignature createHeuristic;
		CallDynamicFunctionSignature callDynamicFunction;
		EventSignature onStartHeuristic;
		EventSignature onEndHeuristic;
		CallHeuristicSignature callHeuristic;

	private:
		void loadFunctions(const std::filesystem::path& pathToSource);
		
		template<FunctionPointer T>
		void loadMethod(const NativeString& typeName, const NativeString& methodName, T& method);

	public:
		DotNetRuntime();

		void free(void* implementation);

		void dealloc(void* allocatedMemory);

		bool loadExecutor(std::string_view name, const utility::LoadSource& source) override;

		std::unique_ptr<BaseExecutor> createExecutor(std::string_view name) const override;

		void finishInitialization() override;

		void* createExecutorSettings(const void* implementation) const override;

		void* createHTTPRequest(framework::interfaces::IHTTPRequest* request) const override;

		void* createHTTPResponse(framework::interfaces::IHTTPResponse* response) const override;

		void initializeWebFramework(std::string_view libraryPath) override;

		std::optional<std::string> loadSource(std::string_view pathToSource, utility::LoadSource& source) override;

		~DotNetRuntime();
	};
}

#endif
