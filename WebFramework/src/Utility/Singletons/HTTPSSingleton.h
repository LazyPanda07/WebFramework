#pragma once

#include "headers.h"

namespace framework
{
	namespace utility
	{
		class HTTPSSingleton
		{
		private:
			bool useHTTPS;
			std::filesystem::path pathToCertificate;
			std::filesystem::path pathToKey;

		private:
			HTTPSSingleton();

			~HTTPSSingleton() = default;

		public:
			static HTTPSSingleton& get();

			void setUseHTTPS(bool useHTTPS);

			void setPathToCertificate(const std::filesystem::path& pathToCertificate);

			void setPathToKey(const std::filesystem::path& pathToKey);

			bool getUseHTTPS() const;

			const std::filesystem::path& getPathToCertificate() const;

			const std::filesystem::path& getPathToKey() const;
		};
	}
}
