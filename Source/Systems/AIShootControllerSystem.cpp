#include "AIShootControllerSystem.hpp"
#include "Prefabs/Prefabs.hpp"

namespace tron
{
	void AIShootControllerSystem::onUpdate(double const deltaSeconds)
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

			fro::Vector2<double> const translation{ transform->world().getTranslation() };
			double const rotation{ transform->world().getRotation() };
			fro::Vector2<double> const lookDirection{ std::cos(rotation), std::sin(rotation) };

			auto&& [rigidbody, collider] { fro::PhysicsSystem::raycast(translation, translation + lookDirection * 1024.0) };

			if (not collider.valid())
				continue;

			if (not (collider->getCategoryBits() & fro::createBitfield(3)))
				continue;

			entity->getParentingScene()->addEntity(prefabs::blueTankBullet(transform));
			elapsedSeconds -= shootCooldown;
			fro::Audio::playSoundEffect("Data/Sound/shoot.wav");
		}
	}

	fro::Group<fro::Transform, AIShootController> AIShootControllerSystem::sGroup{};

}