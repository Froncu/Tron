#ifndef NAVIGATOR_HPP
#define NAVIGATOR_HPP

#include "Fronge.hpp"

#include "NavigationMesh/NavigationMesh.hpp"

namespace tron
{
	class Navigator final : public fro::Component
	{
		friend class NavigatorSystem;

	public:
		static auto constexpr INVALID_NODE_INDEX{ std::numeric_limits<std::size_t>().max() };

		Navigator() = default;
		Navigator(Navigator const&) = default;
		Navigator(Navigator&&) noexcept = default;

		virtual ~Navigator() override = default;

		Navigator& operator=(Navigator const&) = default;
		Navigator& operator=(Navigator&&) noexcept = default;

		void setNavigationMesh(NavigationMesh const& navigationMesh);

		FRO_NODISCARD fro::Reference<NavigationMesh const> getNavigationMesh() const;
		FRO_NODISCARD std::size_t getClosestNodeIndex() const;
		FRO_NODISCARD bool isOnNode() const;

		FRO_NODISCARD void reset();

		fro::Vector2<double> mDesiredDirection{};
		double mVelocity{ 64.0 };

	private:
		fro::Reference<NavigationMesh const> mNavigationMesh{};
		std::size_t mNodeIndex1{ INVALID_NODE_INDEX };
		std::size_t mNodeIndex2{ INVALID_NODE_INDEX };
	};
}

#endif