#include "PlayerShootControllerSystem.hpp"
#include "Prefabs/Prefabs.hpp"

namespace tron
{
	void PlayerShootControllerSystem::onUpdate(double const deltaSeconds)
	{
		for (auto&& [entity, transform, controller] : sGroup)
		{
			double& elapsedSeconds{ controller->elapsedSeconds };
			double const shootCooldown{ controller->shootCooldown };

			if (elapsedSeconds < shootCooldown)
			{
				elapsedSeconds += deltaSeconds;
				continue;
			}

			if (not fro::InputManager::isActionJustPressed(controller->shootAction))
				continue;

			entity->getParentingScene()->addEntity(prefabs::blueTankBullet(transform));
			fro::Audio::playSoundEffect("Data/Sound/shoot.wav");
			elapsedSeconds -= shootCooldown;
		}
	}

	fro::Group<fro::Transform, PlayerShootController> PlayerShootControllerSystem::sGroup{};

}