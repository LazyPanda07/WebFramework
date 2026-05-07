#pragma once

#include "DLLHandler.hpp"
#include "JsonObject.hpp"
#include "Databases/Database.hpp"
#include "Databases/Implementations/DefaultDatabase.hpp"
#include "Databases/Implementations/SqliteDatabase.hpp"
#include "Databases/Implementations/RedisDatabase.hpp"

namespace framework::task_broker
{
	class TaskExecutor
	{
	public:
		class TaskExecutorContext
		{
		private:
			static void fillBuffer(const char* data, size_t size, void* buffer);

		private:
			void* implementation;

		public:
			TaskExecutorContext(void* implementation);

			std::string getFile(const std::filesystem::path& filePath) const;

			std::string processStaticFile(std::string_view fileData, std::string_view fileExtension) const;

			std::string processDynamicFile(std::string_view fileData, const JsonObject& arguments) const;

			template<DatabaseImplementation T = DefaultDatabase>
			Database getOrCreateDatabase(std::string_view databaseName) const;

			template<DatabaseImplementation T = DefaultDatabase>
			Database getDatabase(std::string_view databaseName) const;

			template<DatabaseImplementation T = DefaultDatabase>
			Table getOrCreateTable(std::string_view databaseName, std::string_view tableName, std::string_view createTableQuery) const;

			template<DatabaseImplementation T = DefaultDatabase>
			Table getTable(std::string_view databaseName, std::string_view tableName) const;

			~TaskExecutorContext() = default;
		};

	public:
		TaskExecutor() = default;

		virtual void execute(const JsonObject& data, const TaskExecutorContext& context) = 0;

		virtual ~TaskExecutor() = default;
	};
}

namespace framework::task_broker
{
	inline void TaskExecutor::TaskExecutorContext::fillBuffer(const char* data, size_t size, void* buffer)
	{
		static_cast<std::string*>(buffer)->append(data, size);
	}

	inline TaskExecutor::TaskExecutorContext::TaskExecutorContext(void* implementation) :
		implementation(implementation)
	{

	}

	inline std::string TaskExecutor::TaskExecutorContext::getFile(const std::filesystem::path& filePath) const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(getFileTaskExecutorContext, void, const char* filePath, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);
		void* exception = nullptr;
		std::string result;

		utility::DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getFileTaskExecutorContext, filePath.string().data(), &TaskExecutorContext::fillBuffer, &result, &exception);

		return result;
	}

	inline std::string TaskExecutor::TaskExecutorContext::processStaticFile(std::string_view fileData, std::string_view fileExtension) const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(processStaticFileTaskExecutorContext, void, const char* fileData, size_t size, const char* fileExtension, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);
		void* exception = nullptr;
		std::string result;

		utility::DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(processStaticFileTaskExecutorContext, fileData.data(), fileData.size(), fileExtension.data(), &TaskExecutorContext::fillBuffer, &result, &exception);

		return result;
	}

	inline std::string TaskExecutor::TaskExecutorContext::processDynamicFile(std::string_view fileData, const JsonObject& arguments) const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(processDynamicFileTaskExecutorContext, void, const char* fileData, size_t size, const void* variables, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);

		void* exception = nullptr;
		std::string result;

		utility::DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(processDynamicFileTaskExecutorContext, fileData.data(), fileData.size(), arguments.__getImplementation(), &TaskExecutorContext::fillBuffer, &result, &exception);

		return result;
	}

	template<DatabaseImplementation T>
	inline Database TaskExecutor::TaskExecutorContext::getOrCreateDatabase(std::string_view databaseName) const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(getOrCreateDatabaseTaskExecutorContext, void*, const char* databaseName, const char* implementationName, void** exception);
		void* exception = nullptr;

		Database result = Database::__createDatabase
		(
			static_cast<interfaces::IDatabase*>(utility::DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateDatabaseTaskExecutorContext, databaseName.data(), T::databaseImplementationName.data(), &exception))
		);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return result;
	}

	template<DatabaseImplementation T>
	inline Database TaskExecutor::TaskExecutorContext::getDatabase(std::string_view databaseName) const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(getDatabaseTaskExecutorContext, void*, const char* databaseName, const char* implementationName, void** exception);
		void* exception = nullptr;

		Database result = Database::__createDatabase
		(
			static_cast<interfaces::IDatabase*>(utility::DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getDatabaseTaskExecutorContext, databaseName.data(), T::databaseImplementationName.data(), &exception))
		);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return result;
	}

	template<DatabaseImplementation T>
	inline Table TaskExecutor::TaskExecutorContext::getOrCreateTable(std::string_view databaseName, std::string_view tableName, std::string_view createTableQuery) const
	{
		return this->getOrCreateDatabase<T>(databaseName).getOrCreateTable(tableName, createTableQuery);
	}

	template<DatabaseImplementation T>
	inline Table TaskExecutor::TaskExecutorContext::getTable(std::string_view databaseName, std::string_view tableName) const
	{
		return this->getDatabase<T>(databaseName).getTable(tableName);
	}
}

/**
* Macro for each TaskExecutor subclass
* Used for loading function that creates TaskExecutor subclass
*/
#define DEFINE_TASK_EXECUTOR(subclassName) WEB_FRAMEWORK_FUNCTIONS_API void* create##subclassName##TaskCXXInstance()	\
{	\
	return new subclassName();	\
}

#pragma region ExportFunctions
WEB_FRAMEWORK_FUNCTIONS_API inline void webFrameworkCXXTaskExecute(void* implementation, void* jsonData, void* context)
{
	framework::JsonObject data(jsonData);
	framework::task_broker::TaskExecutor::TaskExecutorContext contextWrapper(context);

	static_cast<framework::task_broker::TaskExecutor*>(implementation)->execute(data, context);
}

WEB_FRAMEWORK_FUNCTIONS_API inline void webFrameworkCXXDeleteTask(void* implementation)
{
	delete static_cast<framework::task_broker::TaskExecutor*>(implementation);
}
#pragma endregion
