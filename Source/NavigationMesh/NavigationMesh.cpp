#include "NavigationMesh.hpp"

namespace tron
{
	std::vector<fro::Vector2<double>> NavigationMesh::getPoints(std::vector<fro::Polygon<double>> const& polygons)
	{
		using namespace fro;

		static auto constexpr vectorLess
		{
			[](Vector2<double> const v1, Vector2<double> const v2)
			{
				if (v1.x == v2.x)
					return v1.y < v2.y;

				return v1.x < v2.x;
			}
		};

		std::set<Vector2<double>, decltype(vectorLess)> points{};

		for (Polygon<double> const& polygon : polygons)
		{
			std::size_t const vertexCount{ polygon.vertices.size() };
			for (std::size_t index{}; index < vertexCount; ++index)
			{
				Vector2<double> const previousVertex{ polygon.vertices[(index == 0 ? vertexCount : index) - 1] };
				Vector2<double> const currentVertex{ polygon.vertices[index] };
				Vector2<double> const nextVertex{ polygon.vertices[(index + 1) % vertexCount] };

				Vector2<double> point;

				if (Vector2<double> previousDirection{ (currentVertex - previousVertex).getNormalized() };
					previousDirection.x and previousDirection.y)
				{
					previousDirection.x = previousDirection.x > 0 ? std::ceil(previousDirection.x) : std::floor(previousDirection.x);
					previousDirection.y = previousDirection.y > 0 ? std::ceil(previousDirection.y) : std::floor(previousDirection.y);

					point = currentVertex + previousDirection * 16;
				}
				else if (Vector2<double> currentDirection{ (nextVertex - currentVertex).getNormalized() };
					currentDirection.x and currentDirection.y)
				{
					currentDirection.x = currentDirection.x > 0 ? std::ceil(currentDirection.x) : std::floor(currentDirection.x);
					currentDirection.y = currentDirection.y > 0 ? std::ceil(currentDirection.y) : std::floor(currentDirection.y);

					point = currentVertex - currentDirection * 16;
				}
				else
				{
					Vector2<double> const perpendicular{ currentDirection.y, -currentDirection.x };
					point = currentVertex + (perpendicular - currentDirection) * 16;
				}

				point.x = std::round(point.x);
				point.y = std::round(point.y);

				if (point.x <= 16 or point.x >= 464 or
					point.y <= 16 or point.y >= 432)
					continue;

				points.insert(point);
			}
		}

		return { points.begin(), points.end() };
	}

	std::vector<NavigationMesh::NodeType> NavigationMesh::calculateNodes(std::vector<fro::Polygon<double>> const& polygons)
	{
		using namespace fro;

		std::vector<Vector2<double>> points{ getPoints(polygons) };

		if (points.empty())
			return{};

		std::vector<NodeType> nodes{};
		nodes.reserve(points.size());

		auto const getClosestInDirection
		{
			[&points, &polygons](Vector2<double> const point1, Vector2<double> const direction)
			{
				std::size_t closestPointIndex{ points.size() };
				double smallestMagnitude{ std::numeric_limits<double>().infinity() };

				for (std::size_t index{}; index < points.size(); ++index)
				{
					Vector2<double> const point2{ points[index] };

					if (point1 == point2)
						continue;

					Vector2<double> const point1Point2{ point2 - point1 };
					double const magnitude{ point1Point2.getMagnitude() };

					if (magnitude >= smallestMagnitude)
						continue;

					double dot{ direction * (point1Point2 / magnitude) };
					if (std::abs(dot - 1.0) > std::numeric_limits<double>().epsilon())
						continue;

					if (doesIntersectAnyPolygon(point1, point2, polygons))
						continue;

					closestPointIndex = index;
					smallestMagnitude = magnitude;
				}

				return closestPointIndex;
			}
		};

		for (Vector2<double> const point : points)
		{
			std::vector<std::size_t> directions{};

			std::size_t availableIndex{ getClosestInDirection(point, { 1.0, 0.0 }) };
			if (availableIndex not_eq points.size())
				directions.push_back(availableIndex);

			availableIndex = getClosestInDirection(point, { 0.0, 1.0 });
			if (availableIndex not_eq points.size())
				directions.push_back(availableIndex);

			availableIndex = getClosestInDirection(point, { -1.0, 0.0 });
			if (availableIndex not_eq points.size())
				directions.push_back(availableIndex);

			availableIndex = getClosestInDirection(point, { 0.0, -1.0 });
			if (availableIndex not_eq points.size())
				directions.push_back(availableIndex);


			nodes.emplace_back(point, std::move(directions));
		}

		return nodes;
	}

	NavigationMesh::NavigationMesh(std::vector<fro::Polygon<double>> polygons)
		: mPolygons{ std::move(polygons) }
		, mNodes{ calculateNodes(mPolygons) }
	{
	}

	std::vector<fro::Polygon<double>> const& NavigationMesh::getPolygons() const
	{
		return mPolygons;
	}

	std::vector<NavigationMesh::NodeType> const& NavigationMesh::getNodes() const
	{
		return mNodes;
	}

	void NavigationMesh::translate(fro::Vector2<double> const translation)
	{
		for (fro::Polygon<double>& polygon : mPolygons)
			for (fro::Vector2<double>& vertex : polygon.vertices)
				vertex += translation;

		for (auto&& [point, availableIndices] : mNodes)
			point += translation;
	}
}