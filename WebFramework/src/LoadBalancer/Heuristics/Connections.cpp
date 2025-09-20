#include "Connections.h"

namespace framework::load_balancer
{
	Connections::Connections(std::string_view ip, std::string_view port, bool useHTTPS) :
		ip(ip),
		port(port),
		useHTTPS(useHTTPS),
		connections(0)
	{

	}

	void Connections::onStart()
	{
		connections++;
	}

	void Connections::onEnd()
	{
		connections--;
	}

	uint64_t Connections::operator ()() const
	{
		return connections;
	}

	const std::string& Connections::getIp() const
	{
		return ip;
	}

	const std::string& Connections::getPort() const
	{
		return port;
	}

	bool Connections::getUseHTTPS() const
	{
		return useHTTPS;
	}
}
