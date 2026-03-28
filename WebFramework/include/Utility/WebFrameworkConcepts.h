#pragma once

#include <type_traits>
#include <string>

#include <IOSocketStream.h>

#include "Framework/WebFrameworkConstants.h"
#include "Web/HttpRequestImplementation.h"

namespace framework::utility
{
	namespace structs
	{
		struct SendOperation
		{
			static inline constexpr auto& errorName = logging::message::responseError;

			static void process(streams::IOSocketStream& stream, std::string_view response);

			static void process(streams::IOSocketStream& stream, HttpResponseImplementation& response);
		};

		struct ReceiveOperation
		{
			static inline constexpr auto& errorName = logging::message::requestError;

			static void process(streams::IOSocketStream& stream, std::string& request);

			static void process(streams::IOSocketStream& stream, HttpRequestImplementation& request);
		};
	}

	namespace concepts
	{
		template<typename T>
		concept RouteParameterType = std::same_as<T, std::string> || std::same_as<T, int64_t> || std::same_as<T, double>;

		template<typename T>
		concept StreamOperation = std::same_as<T, structs::SendOperation> || std::same_as<T, structs::ReceiveOperation>;
	}
}
