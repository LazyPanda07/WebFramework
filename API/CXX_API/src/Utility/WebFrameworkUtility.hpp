#pragma once

#include "../DLLHandler.hpp"
#include "../Exceptions/WebFrameworkException.hpp"

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
}

namespace framework::utility
{
	namespace uuid
	{
		std::string generateUUID()
		{
			using generateWebFrameworkUUID = void* (*)(void** exception);
			void* exception = nullptr;
			DLLHandler& instance = DLLHandler::getInstance();

			void* result = instance.CALL_WEB_FRAMEWORK_FUNCTION(generateWebFrameworkUUID, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return instance.getString(result);
		}
	}
}
