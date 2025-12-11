#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "PyChunkGenerator.h"

#include <format>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace framework::utility
{
	pybind11::object PyChunkGenerator::generate()
	{
		PYBIND11_OVERRIDE_PURE
		(
			pybind11::object,
			IPyChunkGenerator,
			generate
		);
	}

	ChunkGeneratorWrapper::ChunkGeneratorWrapper(IPyChunkGenerator& generator) :
		generator(generator)
	{

	}

	std::string_view ChunkGeneratorWrapper::generate(size_t& size)
	{
		pybind11::object temp = generator.generate();

		if (py::isinstance<py::str>(temp))
		{
			data = temp.cast<py::str>();
		}
		else if (py::isinstance<py::bytes>(temp))
		{
			data = temp.cast<py::bytes>();
		}
		else
		{
			throw std::runtime_error(std::format("Wrong type in ChunkGeneratorWrapper: {}", py::repr(py::type::of(temp)).cast<std::string>()));
		}

		size = data.size();

		return data;
	}
}
