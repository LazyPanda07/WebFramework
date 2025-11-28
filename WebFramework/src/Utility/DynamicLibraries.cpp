#include "DynamicLibraries.h"

#include <format>
#include <fstream>
#include <array>
#include <cstring>

#include <Exceptions/FileDoesNotExistException.h>
#include <Strings.h>

#ifdef __LINUX__
#include <dlfcn.h>
#include <limits.h>
#include <unistd.h>
#endif

static bool isDotNetAssembly(const std::filesystem::path& libraryPath);

namespace framework::utility
{
	std::string makePathToLoadSource(const std::filesystem::path& pathToSource, LoadSourceType& type)
	{
		std::filesystem::path absolutePath = std::filesystem::absolute(pathToSource);
		std::filesystem::path fileName = absolutePath.filename();
		bool hasExtension = absolutePath.has_extension();
		std::string extension;
		std::string prefix;
		auto initPy = [&]()
			{
				extension = "";
				prefix = "";

				fileName.replace_extension();

				type = LoadSourceType::python;
			};
		auto generatePath = [&](std::string_view extension) -> std::string
			{
				return std::filesystem::path(std::format("{}/{}{}{}", absolutePath.string(), prefix, fileName.string(), extension)).make_preferred().string();
			};

		if (hasExtension)
		{
			extension = absolutePath.extension().string();

			if (extension == ".py")
			{
				initPy();
			}
			else
			{
#ifdef __LINUX__
				if (fileName.string().find("lib") == std::string::npos)
				{
					prefix = "lib";
				}
#endif

				type = LoadSourceType::dynamicLibrary;
			}
		}

		absolutePath = absolutePath.parent_path();

		if (!hasExtension)
		{
			if (std::filesystem::exists(generatePath(".py")))
			{
				initPy();
			}
			else if (std::filesystem::exists(generatePath(".dll")))
			{
				extension = ".dll";

				type = isDotNetAssembly(generatePath(".dll")) ? LoadSourceType::dotNet : LoadSourceType::dynamicLibrary;
			}
			else
			{
#ifdef __LINUX__
				if (fileName.string().find("lib") == std::string::npos)
				{
					prefix = "lib";
				}

				extension = ".so";
#else
				extension = ".dll";
#endif
				type = LoadSourceType::dynamicLibrary;
			}
		}

		return generatePath(extension);
	}

	std::string getPathToWebFrameworkSharedLibrary()
	{
#ifdef __LINUX__
		Dl_info info;

		if (dladdr(reinterpret_cast<void*>(&getPathToWebFrameworkSharedLibrary), &info) && info.dli_fname)
		{
			char fullpath[PATH_MAX]{};

			return realpath(info.dli_fname, fullpath) ?
				fullpath :
				info.dli_fname;
		}
#else
		HMODULE module = nullptr;

		BOOL result = GetModuleHandleExA
		(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			reinterpret_cast<LPCSTR>(&getPathToWebFrameworkSharedLibrary),
			&module
		);

		if (result)
		{
			char path[MAX_PATH]{};

			if (GetModuleFileNameA(module, path, MAX_PATH))
			{
				return ::utility::strings::replaceAll(path, ".DLL", ".dll");
			}
		}
#endif

		return "";
	}

	HMODULE loadLibrary(const std::filesystem::path& pathToLibrary)
	{
#ifdef __LINUX__
		return dlopen(pathToLibrary.string().data(), RTLD_LAZY);
#else
		return LoadLibraryA(pathToLibrary.string().data());
#endif				
	}

	HMODULE getLoadedLibrary(std::string_view libraryName)
	{
#ifdef __LINUX__
		return dlopen(libraryName.data(), RTLD_NOLOAD);
#else
		return GetModuleHandleA(libraryName.data());
#endif	
	}
}

bool isDotNetAssembly(const std::filesystem::path& libraryPath)
{
	constexpr size_t chunkSize = 1 * 1024 * 1024;
	constexpr std::array<uint8_t, 4> signature = { 0x42, 0x53, 0x4A, 0x42 }; // BSJB

	std::ifstream stream(libraryPath, std::ios::binary);
	std::vector<uint8_t> chunk(chunkSize, 0);
	std::vector<uint8_t> last;

	while (true)
	{
		size_t extractedBytes = stream.read(reinterpret_cast<char*>(chunk.data() + last.size()), chunk.size() - last.size()).gcount();

		if (last.size())
		{
			std::copy(last.begin(), last.end(), chunk.data());

			last.clear();
		}

		if (!extractedBytes)
		{
			break;
		}

		if (size_t size = extractedBytes % signature.size())
		{
			for (size_t i = 0; i < size; i++)
			{
				last.push_back(chunk[extractedBytes - 1 - i]);
			}

			extractedBytes -= size;
		}

		for (size_t i = 0; i < extractedBytes; i++)
		{
			if (!std::memcmp(chunk.data() + i, signature.data(), signature.size()))
			{
				return true;
			}
		}
	}

	return false;
}
