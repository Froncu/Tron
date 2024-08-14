#ifndef AI_MOVE_CONTROLLER_HPP
#define AI_MOVE_CONTROLLER_HPP

#include "Fronge.hpp"

namespace tron
{
	struct AIMoveController final : public fro::Component
	{
	public:
		fro::Reference<fro::Transform const> target{};
		fro::Vector2<double> targetPosition{};
		double elapsedSeconds{};
		double maxElapsedSeconds{};
		bool isChasing{ true };
	};
}

#endif