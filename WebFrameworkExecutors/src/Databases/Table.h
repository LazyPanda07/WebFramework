#pragma once

#include "Databases/Interfaces/ITable.h"

#include "ExecutorsConstants.h"
#include "SQLResult.h"

namespace framework
{
	class EXECUTORS_API Table
	{
	private:
		interfaces::ITable* implementation;

	public:
		Table(interfaces::ITable* implementation);

		SQLResult execute(std::string_view query, const std::vector<SQLValue>& values);

		~Table() = default;
	};
}
