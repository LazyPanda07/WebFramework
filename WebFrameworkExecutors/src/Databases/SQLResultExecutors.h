#pragma once

#include <unordered_map>

#include "DatabaseInterfaces/ISQLResult.h"

#include "SQLValueExecutors.h"
#include "ExecutorsConstants.h"

namespace framework
{
	class EXECUTORS_API SQLResultExecutors
	{
	public:
		using ValueType = std::vector<std::unordered_map<std::string, SQLValueExecutors>>;

	private:
		ValueType rows;

	private:
		static void reserveSize(size_t size, void* buffer);

		static void fill(const char* columnName, const void* value, size_t index, size_t size, void* buffer);

	public:
		SQLResultExecutors(interfaces::ISQLResult* implementation);

		std::unordered_map<std::string, SQLValueExecutors>& at(size_t index);

		const std::unordered_map<std::string, SQLValueExecutors>& at(size_t index) const;

		size_t size() const;

		ValueType::iterator begin();

		ValueType::iterator end();

		operator ValueType& ();

		operator const ValueType& () const;

		std::unordered_map<std::string, SQLValueExecutors>& operator[](size_t index);

		const std::unordered_map<std::string, SQLValueExecutors>& operator[](size_t index) const;

		~SQLResultExecutors() = default;
	};
}
