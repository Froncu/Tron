#ifndef PLAYER_CANON_CONTROLLER_HPP
#define PLAYER_CANON_CONTROLLER_HPP

#include "Fronge.hpp"

namespace tron
{
	struct PlayerCanonController final : public fro::Component
	{
	public:
		std::string lookRightAction{ "lookRight1" };
		std::string lookLeftAction{ "lookLeft1" };
		std::string lookUpAction{ "lookUp1" };
		std::string lookDownAction{ "lookDown1" };
		std::string shootAction{ "shoot1" };

		double cooldown{ 1.0 };
		double elapsedSecondsSinceLastShot{};
		int consecutiveShotsCount{};
	};
}

#endif