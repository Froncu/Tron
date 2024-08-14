#ifndef NAVIGATOR_SYSTEM_HPP
#define NAVIGATOR_SYSTEM_HPP

#include "Fronge.hpp"

#include "Components/Navigator.hpp"

namespace tron
{
	class NavigatorSystem final
	{
	public:
		static void onFixedUpdate(double const fixedDeltaSeconds);

	private:
		static fro::Group<fro::Transform, fro::Rigidbody, Navigator> sGroup;

		NavigatorSystem() = delete;
		NavigatorSystem(NavigatorSystem const&) = delete;
		NavigatorSystem(NavigatorSystem&&) noexcept = delete;

		~NavigatorSystem() = delete;

		NavigatorSystem& operator=(NavigatorSystem const&) = delete;
		NavigatorSystem& operator=(NavigatorSystem&&) noexcept = delete;
	};
}

#endif