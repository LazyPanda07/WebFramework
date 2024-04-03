#pragma once

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <format>
#include <algorithm>
#include <fstream>

#ifdef __LINUX__
#include <dlfcn.h>
#else
#include <Windows.h>
#endif

#include "JSONParser.h"
#include "LocalizationConstants.h"

#ifdef __LINUX__
using HMODULE = void*;
#endif

namespace localization
{
	using namespace std::string_view_literals;

	/// @brief Singleton for text localization
	template<typename T>
	class LOCALIZATION_API BaseTextLocalization final
	{
	private:
		const std::unordered_map<std::string, const std::unordered_map<std::string, std::string>*>* dictionaries;
		const std::string* originalLanguage;
		std::string language;
		std::string pathToModule;
		HMODULE handle;

	private:
		BaseTextLocalization(const std::string& localizationModule);

		BaseTextLocalization(const BaseTextLocalization<T>&) = delete;

		BaseTextLocalization(BaseTextLocalization<T>&& other) noexcept;

		BaseTextLocalization<T>& operator = (const BaseTextLocalization<T>&) = delete;

		BaseTextLocalization<T>& operator = (BaseTextLocalization<T>&& other) noexcept;

		~BaseTextLocalization();

	public:
		/// @brief Exception can be thrown on first call. Warnings outputs in std::cerr
		/// @return Singleton instance of default localization module(Localization.dll)
		/// @exception std::runtime_error Can't find localization module or something inside localization module
		static BaseTextLocalization& get();

		/// @brief Change localization
		/// @param language Language key
		/// @exception std::runtime_error Wrong language
		void changeLanguage(const std::string& language);

		/// @brief Get original language
		/// @return originalLanguage
		const std::string& getOriginalLanguage() const;

		/// @brief Get current language
		/// @return language
		const std::string& getCurrentLanguage() const;

		/// @brief Get path to used module
		const std::string& getPathToModule() const;

		/// @brief Get localized text
		/// @param key Localization key
		/// @param language Specific language
		/// @return Localized value
		/// @exception std::runtime_error Wrong key
		/// @exception std::out_of_range
		const std::basic_string<T>& getString(const std::string& key, const std::string& language) const;

		/// @brief Get localized text
		/// @param key Localization key
		/// @return Localized value
		/// @exception std::runtime_error Wrong key
		const std::basic_string<T>& operator [] (const std::string& key) const;

		friend class BaseTextLocalization<wchar_t>;
		friend class MultiLocalizationManager;
		friend struct LocalizationHolder;
		friend std::unique_ptr<BaseTextLocalization<T>>::deleter_type;
	};

	template<typename T>
	BaseTextLocalization<T>::BaseTextLocalization(const std::string& localizationModule)
	{
#ifdef __LINUX__
		handle = dlopen((std::string("lib") + localizationModule + ".so").data(), RTLD_LAZY);
#else
		handle = LoadLibraryA((localizationModule + ".dll").data());
#endif
		auto load = [this](const char* name)
			{
#ifdef __LINUX__
				return dlsym(handle, name);
#else
				return GetProcAddress(handle, name);
#endif
			};

		if (!handle)
		{
			throw std::runtime_error(std::format("Can't find {}"sv, localizationModule));
		}

		pathToModule = localizationModule;

		dictionaries = reinterpret_cast<const std::unordered_map<std::string, const std::unordered_map<std::string, std::string>*>*>(load("dictionaries"));

		if (!dictionaries)
		{
			throw std::runtime_error(std::format("Can't find dictionaries in {}, rebuild and try again"sv, localizationModule));
		}

		originalLanguage = reinterpret_cast<const std::string*>(load("originalLanguage"));

		if (!originalLanguage)
		{
			throw std::runtime_error(std::format("Can't find originalLanguage in {}, rebuild and try again"sv, localizationModule));
		}

		language = *originalLanguage;
	}

	template<typename T>
	BaseTextLocalization<T>::BaseTextLocalization(BaseTextLocalization<T>&& other) noexcept
	{
		(*this) = std::move(other);
	}

	template<typename T>
	BaseTextLocalization<T>& BaseTextLocalization<T>::operator = (BaseTextLocalization<T>&& other) noexcept
	{
		dictionaries = other.dictionaries;
		originalLanguage = other.originalLanguage;
		language = std::move(other.language);
		pathToModule = std::move(other.pathToModule);
		handle = other.handle;

		other.handle = nullptr;

		return *this;
	}

	template<typename T>
	BaseTextLocalization<T>::~BaseTextLocalization()
	{
#ifdef __LINUX__
		dlclose(handle);
#else
		FreeLibrary(handle);
#endif
	}

	template<typename T>
	inline BaseTextLocalization<T>& BaseTextLocalization<T>::get()
	{
		static std::unique_ptr<BaseTextLocalization<T>> instance;

		if (!instance)
		{
			json::JSONParser settings(std::ifstream(localizationModulesFile.data()));

			instance = std::unique_ptr<BaseTextLocalization<T>>(new BaseTextLocalization<T>(settings.getString(settings::defaultModuleSetting)));
		}

		return *instance;
	}

	template<typename T>
	void BaseTextLocalization<T>::changeLanguage(const std::string& language)
	{
		if (dictionaries->find(language) == dictionaries->end())
		{
			throw std::runtime_error(std::format(R"(Wrong language value "{}")"sv, language));
		}

		this->language = language;
	}

	template<typename T>
	const std::string& BaseTextLocalization<T>::getOriginalLanguage() const
	{
		return *originalLanguage;
	}

	template<typename T>
	const std::string& BaseTextLocalization<T>::getCurrentLanguage() const
	{
		return language;
	}

	template<typename T>
	const std::string& BaseTextLocalization<T>::getPathToModule() const
	{
		return pathToModule;
	}

	template<typename T>
	const std::basic_string<T>& BaseTextLocalization<T>::getString(const std::string& key, const std::string& language) const
	{
		try
		{
			const std::string& result = dictionaries->at(language)->at(key);

			return result.empty() ? dictionaries->at(*originalLanguage)->at(key) : result;
		}
		catch (const std::out_of_range&)
		{
			try
			{
				return dictionaries->at(*originalLanguage)->at(key);
			}
			catch (const std::out_of_range&)
			{
				throw std::runtime_error(format(R"(Can't find key "{}")"sv, key));
			}
		}
	}

	template<typename T>
	const std::basic_string<T>& BaseTextLocalization<T>::operator[] (const std::string& key) const
	{
		return this->getString(key, language);
	}
}
