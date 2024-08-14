#include "Components/PlayerBulletCollider.hpp"
#include "PlayerCanonControllerSystem.hpp"
#include "Prefabs/Prefabs.hpp"

namespace tron
{
	void PlayerCanonControllerSystem::onUpdate(double const deltaSeconds)
	{
		for (auto&& [entity, transform, controller] : sGroup)
		{
			controller->elapsedSecondsSinceLastShot += deltaSeconds;

			fro::Vector2<double> const lookDirection
			{
				fro::InputManager::getActionStrengthAxis2D(
					controller->lookRightAction,
					controller->lookLeftAction,
					controller->lookDownAction,
					controller->lookUpAction)
			};

			if (lookDirection.x or lookDirection.y)
				transform->setWorldRotation(std::atan2(lookDirection.y, lookDirection.x));

			if (not fro::InputManager::isActionJustPressed(controller->shootAction))
				continue;

			if (controller->elapsedSecondsSinceLastShot < controller->cooldown)
			{
				if (controller->consecutiveShotsCount == 3)
					continue;

				++controller->consecutiveShotsCount;
			}
			else
				controller->consecutiveShotsCount = 1;

			controller->elapsedSecondsSinceLastShot = 0.0;
			entity->getParentingScene()->addEntity(prefabs::bullet(transform));
			fro::Audio::playSoundEffect("Data/Sound/canon.wav");
		}
	}


	fro::Group<fro::Transform, PlayerCanonController> PlayerCanonControllerSystem::sGroup{};
}