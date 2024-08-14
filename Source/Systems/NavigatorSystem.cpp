#include "NavigatorSystem.hpp"

namespace tron
{
	void NavigatorSystem::onFixedUpdate(double const)
	{
		for (auto&& [entity, transform, rigidbody, navigator] : sGroup)
		{
			auto const& navigiationMesh{ navigator->mNavigationMesh };

			if (not navigiationMesh.valid())
				continue;

			auto const& nodes{ navigiationMesh->getNodes() };

			if (nodes.empty())
				continue;

			auto& nodeIndex1{ navigator->mNodeIndex1 };
			auto const translation{ transform->world().getTranslation() };
			auto& nodeIndex2{ navigator->mNodeIndex2 };

			if (nodeIndex1 == Navigator::INVALID_NODE_INDEX and
				nodeIndex2 == Navigator::INVALID_NODE_INDEX)
			{
				auto const closestPoint
				{
					std::min_element(nodes.begin(), nodes.end(),
						[translation](auto const& node1, auto const& node2)
						{
							fro::Vector2<double> const point1{ node1.first };
							fro::Vector2<double> const point2{ node2.first };

							return (point1 - translation).getMagnitude() < (point2 - translation).getMagnitude();
						})
				};

				transform->setWorldTranslation(closestPoint->first);
				nodeIndex1 = static_cast<std::size_t>(std::distance(nodes.begin(), closestPoint));
				nodeIndex2 = nodeIndex1;
			}

			fro::Vector2<double> const desiredDirection
			{
				std::abs(navigator->mDesiredDirection.x) > std::abs(navigator->mDesiredDirection.y) ?
				fro::Vector2<double>{ std::round(navigator->mDesiredDirection.x), 0.0 } :
				fro::Vector2<double>{ 0.0, std::round(navigator->mDesiredDirection.y) }
			};

			fro::Vector2<double> direction;

			if (not desiredDirection.x and not desiredDirection.y)
				direction = {};

			else if (nodeIndex1 == nodeIndex2)
			{
				fro::Vector2<double> const rootPoint{ nodes[nodeIndex1].first };
				auto const& availableIndices{ nodes[nodeIndex1].second };

				std::size_t bestIndex{};
				double largestDot{};
				for (std::size_t const index : availableIndices)
				{
					double const dot{ (nodes[index].first - rootPoint).getNormalized() * desiredDirection };
					if (dot > largestDot)
					{
						largestDot = dot;
						bestIndex = index;
					}
				}

				if (largestDot <= 0.0)
					direction = {};
				else
				{
					nodeIndex2 = bestIndex;
					direction = (nodes[nodeIndex2].first - nodes[nodeIndex1].first).normalize();
				}
			}
			else
			{
				fro::Vector2<double> const point1{ nodes[nodeIndex1].first };
				fro::Vector2<double> const point2{ nodes[nodeIndex2].first };

				double const distanceToNode1{ (point1 - translation).getMagnitude() };
				double const distanceToNode2{ (point2 - translation).getMagnitude() };

				std::size_t const closestNodeIndex{ distanceToNode1 < distanceToNode2 ? nodeIndex1 : nodeIndex2 };

				if (closestNodeIndex == nodeIndex2)
					std::swap(nodeIndex1, nodeIndex2);

				fro::Vector2<double> const point21{ nodes[nodeIndex1].first - nodes[nodeIndex2].first };
				fro::Vector2<double> const toClosestNode{ nodes[nodeIndex1].first - translation };

				if (point21 * toClosestNode < std::numeric_limits<double>().epsilon())
				{
					nodeIndex1 = closestNodeIndex;
					nodeIndex2 = closestNodeIndex;
					transform->setWorldTranslation(nodes[closestNodeIndex].first);
					direction = {};
				}
				else
				{
					fro::Vector2<double> const point12{ (point2 - point1).normalize() };
					double const dot{ point12 * desiredDirection };

					if (std::abs(dot) > 0.5)
						direction = point12 * std::round(dot);
					else
						direction = toClosestNode.getNormalized();
				}
			}

			direction.x = std::round(direction.x);
			direction.y = std::round(direction.y);

			rigidbody->setLinearVelocity(direction * navigator->mVelocity);
			if (direction.x or direction.y)
				transform->setWorldRotation(std::atan2(direction.y, direction.x));
		}
	}

	fro::Group<fro::Transform, fro::Rigidbody, Navigator> NavigatorSystem::sGroup{};
}