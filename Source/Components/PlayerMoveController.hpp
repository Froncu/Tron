#ifndef PLAYER_M0VE_CONTROLLER_HPP
#define PLAYER_M0VE_CONTROLLER_HPP

#include "Fronge.hpp"

namespace tron
{
	struct PlayerMoveController final : public fro::Component
	{
	public:
		std::string moveRightAction{ "moveRight1" };
		std::string moveLeftAction{ "moveLeft1" };
		std::string moveUpAction{ "moveUp1" };
		std::string moveDownAction{ "moveDown1" };
	};
}

#endif