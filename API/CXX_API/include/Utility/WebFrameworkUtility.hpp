#pragma once

#include "../DLLHandler.hpp"
#include "../Exceptions/WebFrameworkException.hpp"
#include "../Databases/SqlValue.hpp"

namespace framework::utility
{
	namespace uuid
	{
		/**
		 * @brief Generate UUID in format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
		 * @return 
		 */
		std::string generateUUID();
	}

	namespace database
	{
		/**
		 * @brief Helper function for SqlValue
		 * @tparam ...Args 
		 * @param ...args 
		 * @return 
		 */
		template<typename... Args>
		std::array<SqlValue, sizeof...(Args)> makeSQLValues(Args&&... args);
	}
}

namespace framework::utility
{
	namespace uuid
	{
		inline std::string generateUUID()
		{
			using generateWebFrameworkUUID = void* (*)(void** exception);
			void* exception = nullptr;
			DllHandler& instance = DllHandler::getInstance();

			void* result = instance.CALL_WEB_FRAMEWORK_FUNCTION(generateWebFrameworkUUID, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return instance.getString(result);
		}
	}

	namespace database
	{
		template<typename... Args>
		inline std::array<SqlValue, sizeof...(Args)> makeSQLValues(Args&&... args)
		{
			std::array<SqlValue, sizeof...(Args)> result({ std::forward<Args>(args)... });

			return result;
		}
	}
}
