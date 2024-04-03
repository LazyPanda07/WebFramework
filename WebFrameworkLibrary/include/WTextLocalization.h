#pragma once

/// @file WTextLocalization.h
/// @brief BaseTextLocalization specialization with std::wstring

#ifndef __LINUX__

#include "TextLocalization.h"

namespace localization
{
	/// @brief TextLocalization with std::wstring
	using WTextLocalization = localization::BaseTextLocalization<wchar_t>;

	/// @brief TextLocalization specialization with wchar_t
	template<>
	class LOCALIZATION_API BaseTextLocalization<wchar_t> final
	{
	private:
		std::unordered_map<std::string, std::unordered_map<std::string, std::wstring>> dictionaries;
		std::string originalLanguage;
		std::string language;
		std::string pathToModule;

	private:
		void convertLocalization(const TextLocalization& localizationModule);

	private:
		BaseTextLocalization(const std::string& localizationModule);

		BaseTextLocalization(const TextLocalization& localizationModule);

		BaseTextLocalization(const WTextLocalization&) = delete;

		BaseTextLocalization(WTextLocalization&& other) noexcept;

		WTextLocalization& operator = (const WTextLocalization&) = delete;

		WTextLocalization& operator = (WTextLocalization&& other) noexcept;

		~BaseTextLocalization() = default;

	public:
		/// @brief Exception can be thrown on first call
		/// @return Singleton instance
		/// @exception std::runtime_error Can't find Localization.dll or something inside Localization.dll
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
		const std::wstring& getString(const std::string& key, const std::string& language) const;

		/// @brief Get localized text
		/// @param key Localization key
		/// @return Localized value
		/// @exception std::runtime_error Wrong key
		const std::wstring& operator [] (const std::string& key) const;

		friend class MultiLocalizationManager;
		friend struct LocalizationHolder;
		friend std::unique_ptr<WTextLocalization>::deleter_type;
	};
}

#endif
