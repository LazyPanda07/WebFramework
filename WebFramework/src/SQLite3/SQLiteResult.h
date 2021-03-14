#pragma once

#if defined(DATABASES_DLL) || defined(ALL_DLL)
#define WEB_FRAMEWORK_API __declspec(dllexport)
#define SHA256_DLL
#else
#define WEB_FRAMEWORK_API
#endif // DATABASES_DLL

#include <vector>
#include <unordered_map>

namespace framework
{
	namespace sqlite
	{
		namespace utility
		{
			class WEB_FRAMEWORK_API SQLiteResult
			{
			public:
				using const_iterator = std::vector<std::unordered_map<std::string, std::string>>::const_iterator;

			private:
				std::vector<std::unordered_map<std::string, std::string>> rows;

			public:
				SQLiteResult(size_t rowCount);

				size_t size() const;

				std::unordered_map<std::string, std::string>& operator [] (size_t index);

				const std::unordered_map<std::string, std::string>& operator [] (size_t index) const;

				const_iterator begin() const noexcept;

				const_iterator end() const noexcept;

				~SQLiteResult() = default;
			};
		}
	}
}
