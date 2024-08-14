#ifndef AI_SHOOT_CONTROLLER_SYSTEM_HPP
#define AI_SHOOT_CONTROLLER_SYSTEM_HPP

#include "Fronge.hpp"

#include "Components/Navigator.hpp"
#include "Components/AIShootController.hpp"

namespace tron
{
	class AIShootControllerSystem final
	{
	public:
		static void onUpdate(double const deltaSeconds);

	private:
		static fro::Group<fro::Transform, AIShootController> sGroup;

		AIShootControllerSystem() = delete;
		AIShootControllerSystem(AIShootControllerSystem const&) = delete;
		AIShootControllerSystem(AIShootControllerSystem&&) noexcept = delete;

		~AIShootControllerSystem() = delete;

		AIShootControllerSystem& operator=(AIShootControllerSystem const&) = delete;
		AIShootControllerSystem& operator=(AIShootControllerSystem&&) noexcept = delete;
	};
}

#endif