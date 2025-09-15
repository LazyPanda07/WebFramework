#pragma once

#include "DatabaseInterfaces/ITable.h"

#include "ExecutorsConstants.h"
#include "SQLResultExecutors.h"

namespace framework
{
	class EXECUTORS_API TableExecutors
	{
	private:
		interfaces::ITable* implementation;

	public:
		TableExecutors(interfaces::ITable* implementation);

		SQLResultExecutors execute(std::string_view query, const std::vector<SQLValueExecutors>& values);

		~TableExecutors() = default;
	};
}
