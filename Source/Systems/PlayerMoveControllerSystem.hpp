#ifndef PLAYER_MOVE_CONTROLLER_SYSTEM_HPP
#define PLAYER_MOVE_CONTROLLER_SYSTEM_HPP

#include "Fronge.hpp"

#include "Components/Navigator.hpp"
#include "Components/PlayerCanonController.hpp"
#include "Components/PlayerMoveController.hpp"

namespace tron
{
	class PlayerMoveControllerSystem final
	{
	public:
		static void onUpdate(double const deltaSeconds);

	private:
		static fro::Group<fro::Transform, Navigator, PlayerMoveController> sGroup;

		PlayerMoveControllerSystem() = delete;
		PlayerMoveControllerSystem(PlayerMoveControllerSystem const&) = delete;
		PlayerMoveControllerSystem(PlayerMoveControllerSystem&&) noexcept = delete;

		~PlayerMoveControllerSystem() = delete;

		PlayerMoveControllerSystem& operator=(PlayerMoveControllerSystem const&) = delete;
		PlayerMoveControllerSystem& operator=(PlayerMoveControllerSystem&&) noexcept = delete;
	};
}

#endif