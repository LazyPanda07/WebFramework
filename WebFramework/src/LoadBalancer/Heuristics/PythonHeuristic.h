#pragma once

#include "BaseLoadBalancerHeuristic.h"

#ifdef __WITH_PYTHON_EXECUTORS__

#include <pybind11/embed.h>

#include "Utility/Sources.h"

namespace framework::load_balancer
{
	class PythonHeuristic : public BaseLoadBalancerHeuristic
	{
	private:
		std::string ip;
		std::string port;
		bool useHTTPS;
		pybind11::object* implementation;

	public:
		PythonHeuristic(std::string_view ip, std::string_view port, bool useHTTPS, std::string_view heuristicName, const utility::LoadSource& source);

		uint64_t operator ()() const override;

		void onStart() override;

		void onEnd() override;

		const std::string& getIp() const override;

		const std::string& getPort() const override;

		bool getUseHTTPS() const override;

		~PythonHeuristic();
	};
}

#endif
