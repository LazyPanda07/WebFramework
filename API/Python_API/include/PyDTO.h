#pragma once

#include <pybind11/pybind11.h>

#include <Databases/SqlResult.hpp>

namespace framework
{
	class PyDTO
	{
	private:
		pybind11::dict data;

	public:
		static PyDTO create(const SqlResult::Row& row);

	public:
		PyDTO() = default;

		const pybind11::dict& operator *() const;

		~PyDTO() = default;
	};
}
