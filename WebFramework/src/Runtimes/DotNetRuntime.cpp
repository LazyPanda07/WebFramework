#include "DotNetRuntime.h"

#include <format>

#include <Log.h>
#include <JsonBuilder.h>

#include "Utility/DynamicLibraries.h"

#ifdef __WITH_DOT_NET_EXECUTORS__

namespace framework::runtime
{
	std::filesystem::path DotNetRuntime::getPathToRuntimeConfig()
	{
		return std::filesystem::temp_directory_path() / "web_framework_runtimeconfig.json";
	}

	void DotNetRuntime::createRuntimeConfig()
	{
		std::ofstream stream(DotNetRuntime::getPathToRuntimeConfig());
		json::JsonBuilder builder(CP_UTF8);
		json::JsonObject runtimeOptions;
		json::JsonObject framework;

		framework["name"] = "Microsoft.NETCore.App";
		framework["version"] = "8.0.0";

		runtimeOptions["tfm"] = "net8.0";
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
		this->loadMethod(typeName, "CreateExecutor", createExecutor);
		this->loadMethod(typeName, "CreateHttpRequest", createHttpRequest);
		this->loadMethod(typeName, "CreateHttpResponse", createHttpResponse);
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
		createExecutor(nullptr),
		createHttpRequest(nullptr),
		createHttpResponse(nullptr),
		doPost(nullptr),
		doGet(nullptr),
		doHead(nullptr),
		doPut(nullptr),
		doDelete(nullptr),
		doPatch(nullptr),
		doOptions(nullptr),
		doTrace(nullptr),
		doConnect(nullptr)
	{
		static const std::filesystem::path hostfxrPath = R"(C:\Program Files\dotnet\host\fxr\8.0.8\hostfxr.dll)";

		std::filesystem::path apiPath = std::filesystem::current_path() / "WebFrameworkCSharpAPI.dll";

		DotNetRuntime::createRuntimeConfig();

		runtimeLibrary = utility::loadLibrary(hostfxrPath);

		initialization = utility::load<hostfxr_initialize_for_runtime_config_fn>(runtimeLibrary, "hostfxr_initialize_for_runtime_config");
		getRuntimeDelegate = utility::load<hostfxr_get_runtime_delegate_fn>(runtimeLibrary, "hostfxr_get_runtime_delegate");
		close = utility::load<hostfxr_close_fn>(runtimeLibrary, "hostfxr_close");

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
		return nullptr;
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
		int code = loadAssembly(nativePathToSource.native().data(), nullptr, nullptr);
		std::optional<std::string> errorMessage;

#ifdef __LINUX__
		switch (code)
		{
		case 0:
			errorMessage = std::nullopt;

			break;

		case -1:
			errorMessage = "HostApiInitFailed: Failed to initialize hostfxr";

			break;

		case -10:
			errorMessage = "HostFxrResolveRuntimeFailure: Could not resolve runtime";

			break;

		case -11:
			errorMessage = "HostFxrDelegateFailure: Failed to get delegate";

			break;

		case -12:
			errorMessage = "HostFxrIncompatibleVersion: hostfxr incompatible with runtimeconfig.json";

			break;

		default:
			errorMessage = std::format("Unknown error code: {}", code);

			break;
		}
#else
		switch (code)
		{
		case 0:
			errorMessage = std::nullopt;

			break;

		case 0x80070057:
			errorMessage = "E_INVALIDARG: One or more arguments are invalid";

			break;

		case 0x80070002:
			errorMessage = "ERROR_FILE_NOT_FOUND: Assembly not found";

			break;

		case 0x80131522:
			errorMessage = "MISSING_METHOD_EXCEPTION: Method not found";

			break;

		case 0x80131534:
			errorMessage = "TYPE_LOAD_EXCEPTION: Type not found";

			break;

		case 0x80131513:
			errorMessage = "INVALID_PROGRAM_EXCEPTION: Invalid method signature or missing [UnmanagedCallersOnly]";

			break;

		case 0x80131040:
			errorMessage = "FILE_LOAD_EXCEPTION: Assembly failed to load";

			break;

		default:
		{
			char* msg = nullptr;
			std::ostringstream errorMessageStream;

			FormatMessageA
			(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPSTR>(&msg), 0, nullptr
			);

			if (msg)
			{
				errorMessageStream << "HRESULT 0x" << std::hex << code << ": " << msg;

				LocalFree(msg);
			}
			else
			{
				errorMessageStream << "Unknown HRESULT 0x" << std::hex << code;
			}

			errorMessage = errorMessageStream.str();
		}
		}
#endif

		if (!errorMessage)
		{
			source = nativePathToSource;
		}

		return errorMessage;
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
