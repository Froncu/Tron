#ifndef AI_SHOOT_CONTROLLER_HPP
#define AI_SHOOT_CONTROLLER_HPP

#include "Fronge.hpp"

namespace tron
{
	struct AIShootController final : public fro::Component
	{
	public:
		double shootCooldown{ 3.0 };
		double elapsedSeconds{ shootCooldown };
	};
}

#endif