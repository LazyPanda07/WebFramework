#pragma once

#include <unordered_map>
#include <filesystem>
#include <mutex>

#include "JSONParser.h"
#include "TextLocalization.h"
#include "WTextLocalization.h"

#pragma comment(lib, "JSON.lib")

namespace localization
{
	/// @brief Manage multi localization modules and multi localization itself
	class LOCALIZATION_API MultiLocalizationManager
	{
	public:
		struct LOCALIZATION_API LocalizationHolder
		{
		public:
			TextLocalization localization;
#ifndef __LINUX__
			WTextLocalization wlocalization;
#endif

		public:
#ifdef __LINUX__
			LocalizationHolder(TextLocalization&& localization) noexcept;
#else
			LocalizationHolder(TextLocalization&& localization, WTextLocalization&& wlocalization) noexcept;
#endif

			LocalizationHolder(const LocalizationHolder&) = delete;

			LocalizationHolder& operator = (const LocalizationHolder&) = delete;

			LocalizationHolder(LocalizationHolder&& other) noexcept = default;

			LocalizationHolder& operator = (LocalizationHolder&& other) noexcept = default;
		};

	private:
		json::JSONParser settings;
		std::string defaultModuleName;
		mutable std::mutex mapMutex;
		std::unordered_map<std::string, LocalizationHolder*> localizations;

	private:
		MultiLocalizationManager();

		MultiLocalizationManager(const MultiLocalizationManager&) = delete;

		MultiLocalizationManager(MultiLocalizationManager&&) noexcept = delete;

		MultiLocalizationManager& operator = (const MultiLocalizationManager&) = delete;

		MultiLocalizationManager& operator = (MultiLocalizationManager&&) noexcept = delete;

		~MultiLocalizationManager();

	public:
		/**
		 * @brief Get version of Localization library
		 * @return 
		 */
		static std::string getVersion();

	public:
		/// @brief Singleton instance
		/// @return MultiLocalizationManager
		/// @exception std::runtime_error
		/// @exception json::exceptions::CantFindValueException 
		/// @exception std::bad_variant_access Other type found
		static MultiLocalizationManager& getManager();

		/// @brief Add additional localization module. Thread safe
		/// @param localizationModuleName Name of module
		/// @param pathToLocalizationModule Path to localization module
		/// @return Pointer to MultiLocalizationManager::LocalizationHolder 
		/// @exception std::runtime_error
		LocalizationHolder* addModule(const std::string& localizationModuleName, const std::filesystem::path& pathToLocalizationModule = "");

		/// @brief Remove localization module. Thread safe
		/// @param localizationModuleName Name of module
		/// @return Module was successfully removed
		bool removeModule(const std::string& localizationModuleName);

		/// @brief Get pointer to MultiLocalizationManager::LocalizationHolder. Thread safe
		/// @param localizationModuleName Name of module
		/// @return Pointer to MultiLocalizationManager::LocalizationHolder 
		/// @exception std::runtime_error
		LocalizationHolder* getModule(const std::string& localizationModuleName) const;

		/// @brief Get localized text. Thread safe
		/// @param localizationModuleName Name of module
		/// @param key Localization key
		/// @param language Localized value from specific language
		/// @return Localized value
		/// @exception std::runtime_error Wrong key 
		const std::string& getLocalizedString(const std::string& localizationModuleName, const std::string& key, const std::string& language = "") const;

#ifndef __LINUX__
		/// @brief Get localized text. Thread safe
		/// @param localizationModuleName Name of module
		/// @param key Localization key
		/// @param language Localized value from specific language
		/// @return Localized value
		/// @exception std::runtime_error Wrong key 
		const std::wstring& getLocalizedWideString(const std::string& localizationModuleName, const std::string& key, const std::string& language = "") const;
#endif
	};

	using Holder = MultiLocalizationManager::LocalizationHolder;
}
