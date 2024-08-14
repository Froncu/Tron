#ifndef PLAYER_SHOOT_CONTROLLER_SYSTEM_HPP
#define PLAYER_SHOOT_CONTROLLER_SYSTEM_HPP

#include "Fronge.hpp"

#include "Components/Navigator.hpp"
#include "Components/PlayerShootController.hpp"

namespace tron
{
	class PlayerShootControllerSystem final
	{
	public:
		static void onUpdate(double const deltaSeconds);

	private:
		static fro::Group<fro::Transform, PlayerShootController> sGroup;

		PlayerShootControllerSystem() = delete;
		PlayerShootControllerSystem(PlayerShootControllerSystem const&) = delete;
		PlayerShootControllerSystem(PlayerShootControllerSystem&&) noexcept = delete;

		~PlayerShootControllerSystem() = delete;

		PlayerShootControllerSystem& operator=(PlayerShootControllerSystem const&) = delete;
		PlayerShootControllerSystem& operator=(PlayerShootControllerSystem&&) noexcept = delete;
	};
}

#endif