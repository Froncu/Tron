#ifndef AI_MOVE_CONTROLLER_SYSTEM_HPP
#define AI_MOVE_CONTROLLER_SYSTEM_HPP

#include "Fronge.hpp"

#include "Components/Navigator.hpp"
#include "Components/AIMoveController.hpp"

namespace tron
{
	class AIMoveControllerSystem final
	{
	public:
		static void onUpdate(double const deltaSeconds);

	private:
		static fro::Group<fro::Transform, Navigator, AIMoveController> sGroup;

		AIMoveControllerSystem() = delete;
		AIMoveControllerSystem(AIMoveControllerSystem const&) = delete;
		AIMoveControllerSystem(AIMoveControllerSystem&&) noexcept = delete;

		~AIMoveControllerSystem() = delete;

		AIMoveControllerSystem& operator=(AIMoveControllerSystem const&) = delete;
		AIMoveControllerSystem& operator=(AIMoveControllerSystem&&) noexcept = delete;
	};
}

#endif