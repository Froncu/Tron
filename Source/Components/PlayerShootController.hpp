#ifndef PLAYER_SHOOT_CONTROLLER_HPP
#define PLAYER_SHOOT_CONTROLLER_HPP

#include "Fronge.hpp"

namespace tron
{
	struct PlayerShootController final : public fro::Component
	{
	public:
		std::string shootAction{ "shoot2" };

		double shootCooldown{ 3.0 };
		double elapsedSeconds{ shootCooldown };
	};
}

#endif