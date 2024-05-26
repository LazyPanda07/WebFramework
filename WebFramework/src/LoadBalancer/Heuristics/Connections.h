#pragma once

#include "BaseLoadBalancerHeuristic.h"

namespace framework
{
	namespace load_balancer
	{
		class Connections : public BaseLoadBalancerHeuristic
		{
		private:
			std::atomic_uint64_t connectons;

		public:
			Connections(std::string_view ip, std::string_view port, bool useHTTPS);

			uint64_t operator ()() const override;

			void onStart() override;

			void onEnd() override;

			~Connections() = default;
		};
	}
}
