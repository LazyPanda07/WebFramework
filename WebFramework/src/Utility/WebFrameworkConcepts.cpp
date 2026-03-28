#include "Utility/WebFrameworkConcepts.h"

#include "Utility/Utils.h"

namespace framework::utility::structs
{
	void SendOperation::process(streams::IOSocketStream& stream, std::string_view response)
	{
		stream << response;
	}

	void SendOperation::process(streams::IOSocketStream& stream, HttpResponseImplementation& response)
	{
		stream << response;
	}

	void ReceiveOperation::process(streams::IOSocketStream& stream, std::string& request)
	{
		stream >> request;
	}

	void ReceiveOperation::process(streams::IOSocketStream& stream, HttpRequestImplementation& request)
	{
		stream >> request;
	}
}
