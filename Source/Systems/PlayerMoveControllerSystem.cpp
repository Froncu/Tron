#include "PlayerMoveControllerSystem.hpp"

namespace tron
{
	void PlayerMoveControllerSystem::onUpdate(double const)
	{
		for (auto&& [entity, transform, navigator, controller] : sGroup)
		{
			fro::Vector2<double> const moveDirection
			{
				fro::InputManager::getActionStrengthAxis2D(
					controller->moveRightAction,
					controller->moveLeftAction,
					controller->moveDownAction,
					controller->moveUpAction)
			};

			navigator->mDesiredDirection = moveDirection;

			fro::Vector2<double> const translation{ transform->world().getTranslation() };
			fro::Vector2<double> constexpr diamondPosition{ 240.0, 288.0 };
			if ((translation - diamondPosition).getMagnitude() < 16.0)
			{
				auto const& nodes{ navigator->getNavigationMesh()->getNodes() };
				std::size_t nodeIndex;
				do
				{
					nodeIndex = std::rand() % nodes.size();
				} while ((nodes[nodeIndex].first - translation).getMagnitude() < 128.0);

				transform->setWorldTranslation(nodes[nodeIndex].first);
				navigator->reset();
				fro::Audio::playSoundEffect("Data/Sound/teleport.wav");
			};
		}
	}

	fro::Group<fro::Transform, Navigator, PlayerMoveController> PlayerMoveControllerSystem::sGroup{};
}