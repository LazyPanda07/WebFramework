#pragma once

#include <unordered_map>

#include "Databases/Interfaces/ISQLResult.h"

#include "SQLValue.h"
#include "ExecutorsConstants.h"

namespace framework
{
	class EXECUTORS_API SQLResult
	{
	public:
		using ValueType = std::vector<std::unordered_map<std::string, SQLValue>>;

	private:
		ValueType rows;

	private:
		static void fill(const char* columnName, const interfaces::ISQLValue* value, size_t index, size_t size, void* data);

	public:
		SQLResult(interfaces::ISQLResult* implementation);

		std::unordered_map<std::string, SQLValue>& at(size_t index);

		const std::unordered_map<std::string, SQLValue>& at(size_t index) const;

		size_t size() const;

		ValueType::iterator begin();

		ValueType::iterator end();

		operator ValueType& ();

		operator const ValueType& () const;

		std::unordered_map<std::string, SQLValue>& operator[](size_t index);

		const std::unordered_map<std::string, SQLValue>& operator[](size_t index) const;

		~SQLResult() = default;
	};
}
