#ifndef PLAYER_CANON_SHOOTER_SYSTEM_HPP
#define PLAYER_CANON_SHOOTER_SYSTEM_HPP

#include "Fronge.hpp"
#include "Components/PlayerCanonController.hpp"

namespace tron
{
	class PlayerCanonControllerSystem final
	{
	public:
		static void onUpdate(double const deltaSeconds);

	private:
		static fro::Group<fro::Transform, PlayerCanonController> sGroup;

		PlayerCanonControllerSystem() = delete;
		PlayerCanonControllerSystem(PlayerCanonControllerSystem const&) = delete;
		PlayerCanonControllerSystem(PlayerCanonControllerSystem&&) noexcept = delete;

		~PlayerCanonControllerSystem() = delete;

		PlayerCanonControllerSystem& operator=(PlayerCanonControllerSystem const&) = delete;
		PlayerCanonControllerSystem& operator=(PlayerCanonControllerSystem&&) noexcept = delete;
	};
}

#endif