#include "Registrars/HttpResponseRegistrar.h"

#include <HttpResponse.hpp>

namespace py = pybind11;

namespace registrar
{
	void registerHttpResponse(pybind11::module_& m)
	{
		using namespace py::literals;

		py::class_<framework::HttpResponse>(m, "HttpResponse")
			.def
			(
				py::init
				(
					[](uint64_t pointer)
					{
						return framework::HttpResponse(reinterpret_cast<framework::interfaces::IHttpResponse*>(pointer));
					}
				),
				"pointer_a"
			)
			.def("set_body", py::overload_cast<std::string_view>(&framework::HttpResponse::setBody), "body"_a)
			.def
			(
				"set_body",
				[](framework::HttpResponse& self, const py::dict& json)
				{
					py::module_ jsonModule = py::module_::import("json");

					self.setBody(framework::JsonBuilder(jsonModule.attr("dumps")(json, "ensure_ascii"_a = false).cast<std::string>()));
				},
				"json"_a
			)
			.def("append_body", &framework::HttpResponse::appendBody, "body"_a)
			.def("add_cookie", &framework::HttpResponse::addCookie, "name"_a, "value"_a)
			.def("add_header", &framework::HttpResponse::addHeader, "name"_a, "value"_a)
			.def("set_default", &framework::HttpResponse::setDefault)
			.def("set_is_valid", &framework::HttpResponse::setIsValid)
			.def("set_response_code", &framework::HttpResponse::setResponseCode, "response_code"_a)
			.def("__bool__", [](const framework::HttpResponse& self) { return static_cast<bool>(self); });
	}
}
