#pragma once

#include "Runtime.h"

#include <concepts>

#include "Executors/BaseExecutor.h"

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

	private:
		using HasExecutorSignature = int(*)(const char* executorName);
		using CreateExecutorSignature = void* (*)(const char* executorName);
		using DoMethodSignature = int(*)(void* executor, void* request, void* response);

	public:
		struct Module
		{
		public:
			HasExecutorSignature hasExecutor;
			CreateExecutorSignature createExecutor;
			DoMethodSignature doPost;
			DoMethodSignature doGet;
			DoMethodSignature doHead;
			DoMethodSignature doPut;
			DoMethodSignature doDelete;
			DoMethodSignature doPatch;
			DoMethodSignature doOptions;
			DoMethodSignature doTrace;
			DoMethodSignature doConnect;

		public:
			Module();

			~Module() = default;
		};
		
	private:
		struct NativeStringHash
		{
			size_t operator ()(const NativeString& value) const noexcept;
		};

		struct NativeStringEqual
		{
			bool operator ()(const NativeString& left, const NativeString& right) const noexcept;
		};

	private:
		static std::filesystem::path getPathToRuntimeConfig();

		static void createRuntimeConfig();

	public:
		static NativeString getModuleName(const std::filesystem::path& modulePath);

		static NativeString getModuleName(std::string_view modulePath);

	private:
		hostfxr_initialize_for_runtime_config_fn initialization;
		hostfxr_get_runtime_delegate_fn getRuntimeDelegate;
		hostfxr_close_fn close;
		hostfxr_handle handle;
		load_assembly_fn loadAssembly;
		get_function_pointer_fn getFunctionPointer;
		HMODULE runtimeLibrary;
		std::unordered_map<NativeString, Module, NativeStringHash, NativeStringEqual> modules;

	private:
		void loadFunctions(std::string_view pathToSource);

		template<FunctionPointer T>
		void loadMethod(const NativeString& typeName, const NativeString& methodName, T& method);

	public:
		DotNetRuntime();

		bool hasExecutor(std::string_view executorName, const std::filesystem::path& modulePath) const;

		CreateExecutorFunction getExecutorFunction(std::string_view executorName, const std::filesystem::path& modulePath);

		const Module& operator [](const NativeString& moduleName) const;

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
