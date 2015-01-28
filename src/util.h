#pragma once

#include <algorithm>
#include <vector>
#include <map>

#include "bounds.h"

namespace map
{
	struct Point
	{
		Point() :x(0), y(0) {}
		Point(int x, int y) :x(x), y(y) {}

		int x;
		int y;
	};

	struct Direction
	{
		enum class Code : int
		{
			N = 0,
			E = 1,
			S = 2,
			W = 3,
			None,
			Max = 4
		};

		static const Direction N;
		static const Direction E;
		static const Direction S;
		static const Direction W;

		Direction()
		{
		}

		Direction(const Code& dir, int x, int y)
			: code_(dir)
			, coords_(x, y)
		{
		}

		Direction(const Code& dir, const Point& other)
			: code_(dir)
			, coords_(other)
		{
		}

		Direction& operator=(const Point& other)
		{
			coords_.x = other.x;
			coords_.y = other.y;

			return *this;
		}

		operator int() const
		{
			return static_cast<int>(code_);
		}

		static Direction OppositeDirection(const Direction& other)
		{
			switch (other.code_)
			{
			case Code::N: return Direction::S;
			case Code::S: return Direction::N;
			case Code::E: return Direction::W;
			case Code::W: return Direction::E;
			}
		}

		static Direction FromCode(const Code& dir)
		{
			switch (dir)
			{
			case Code::N: return Direction::N;
			case Code::S: return Direction::S;
			case Code::E: return Direction::E;
			case Code::W: return Direction::W;
			}
		}

		Point coords_;
		Code code_;
	};

	const Direction Direction::N = { Direction::Code::N, 0, -1 };
	const Direction Direction::E = { Direction::Code::E, 1, 0 };
	const Direction Direction::S = { Direction::Code::S, 0, 1 };
	const Direction Direction::W = { Direction::Code::W, -1, 0 };

	struct Coords
	{
		Point coords_;

		Coords(const Point& other) : coords_(other) {}
		Coords(const Coords& other) : coords_(other.coords_) {}

		Coords NextInDirection(const Direction& dir)
		{
			return Add(dir.coords_.x, dir.coords_.y);
		}

		Coords Add(int x, int y)
		{
			Point c(coords_);
			c.x += x;
			c.y += y;
			return c;
		}

		bool operator==(const Coords& other) const
		{
			return coords_.x == other.coords_.x && coords_.y == other.coords_.y;
		}

		bool IsAdjacent(const Coords& other)
		{
			int x = std::abs(coords_.x - other.coords_.x);
			int y = std::abs(coords_.y - other.coords_.y);

			return (x == 1 && y == 0) || (x == 0 && y == 1);
		}

		const Direction GetDirectionTo(const Coords& other)
		{
			int x = coords_.x - other.coords_.x;
			int y = coords_.y - other.coords_.y;

			if (x < 0) return Direction::E;
			if (x > 0) return Direction::W;
			if (y < 0) return Direction::S;
			if (y > 0) return Direction::N;

			return Direction(Direction::Code::None, 0, 0);
		}

		double Distance(const Coords& other)
		{
			int x = coords_.x - other.coords_.x;
			int y = coords_.y - other.coords_.y;

			return std::sqrt(x*x + y*y);
		}
	};
}

using MapCoords = map::Coords;

namespace std
{
	template <>
	struct less<MapCoords> : binary_function < MapCoords, MapCoords, bool >
	{
		bool operator() (const MapCoords& x, const MapCoords& y) const
		{
			return x.coords_.x == y.coords_.x && x.coords_.y == y.coords_.y;
		}
	};
};
