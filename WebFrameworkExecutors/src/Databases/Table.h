#pragma once

#include "Databases/Interfaces/ITable.h"

namespace framework
{
	class Table
	{
	private:
		interfaces::ITable* implementation;

	public:
		Table(interfaces::ITable* implementation);

		~Table() = default;
	};
}
