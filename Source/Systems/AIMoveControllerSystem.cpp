#include "AIMoveControllerSystem.hpp"

namespace tron
{
	void AIMoveControllerSystem::onUpdate(double const deltaSeconds)
	{
		for (auto&& [entity, transform, navigator, controller] : sGroup)
		{
			auto const navigationMesh{ navigator->getNavigationMesh() };
			if (not navigationMesh.valid())
				continue;

			auto const& nodes{ navigationMesh->getNodes() };
			if (nodes.empty())
				continue;

			double& elapsedSeconds{ controller->elapsedSeconds };
			elapsedSeconds += deltaSeconds;

			bool& isChasing{ controller->isChasing };
			fro::Vector2<double> const translation{ transform->world().getTranslation() };
			fro::Vector2<double>& targetPosition{ controller->targetPosition };

			if (double& maxElapsedSeconds{ controller->maxElapsedSeconds };
				elapsedSeconds >= maxElapsedSeconds)
			{
				elapsedSeconds -= maxElapsedSeconds;

				int constexpr max{ 15 };
				int constexpr min{ 5 };

				maxElapsedSeconds = static_cast<double>(std::rand() % (max - min + 1)) + min;
				isChasing = !isChasing;

				if (not isChasing)
					targetPosition = translation;
			}

			auto const& target{ controller->target };

			if (isChasing and target.valid())
				targetPosition = target->world().getTranslation();
			else if (double constexpr epsilon{ 64.0 };
				(targetPosition - translation).getMagnitude() < epsilon)
			{
				do
				{
					targetPosition = nodes[std::rand() % nodes.size()].first;
				} while ((targetPosition - translation).getMagnitude() < 128.0);
			}

			if (navigator->isOnNode())
			{
				auto const& currentNode{ nodes[navigator->getClosestNodeIndex()] };
				auto const& availableIndices{ currentNode.second };
				fro::Vector2<double> const direction{ (targetPosition - translation).normalize() };

				std::size_t const bestIndex
				{
					*std::max_element(availableIndices.begin(), availableIndices.end(),
						[&nodes, currentNode, direction](std::size_t const index1, std::size_t const index2)
						{
							return
								(nodes[index1].first - currentNode.first).normalize() * direction <
								(nodes[index2].first - currentNode.first).normalize() * direction;
						})
				};

				navigator->mDesiredDirection =
					(nodes[bestIndex].first - translation).normalize();
			}
		}
	}

	fro::Group<fro::Transform, Navigator, AIMoveController> AIMoveControllerSystem::sGroup{};

}