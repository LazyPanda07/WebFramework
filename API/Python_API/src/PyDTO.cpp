#include "PyDTO.h"

namespace framework
{
	PyDTO PyDTO::create(const SqlResult::Row& row)
	{
		PyDTO result;

		for (const auto& [key, value] : row)
		{
			result.data[key.data()] = value;
		}

		return result;
	}

	const pybind11::dict& PyDTO::operator *() const
	{
		return data;
	}
}
