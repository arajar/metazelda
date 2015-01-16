#pragma once

#include <algorithm>
#include "Direction.h"

namespace mz
{
	struct Coords
	{
		int m_x;
		int m_y;

		Coords()
			: Coords(0, 0)
		{}

		Coords(int x, int y)
			: m_x(x)
			, m_y(y)
		{}

		Coords NextInDirection(const Direction& d)
		{
			return Add(d.m_x, d.m_y);
		}

		Coords Add(int x, int y)
		{
			return Coords(m_x + x, m_y + y);
		}

		Coords Subtract(const Coords& other)
		{
			return Coords(m_x - other.m_x, m_y - other.m_y);
		}

		inline bool operator==(const Coords& other)
		{
			return m_x == other.m_x && m_y == other.m_y;
		}

		bool IsAdjacent(const Coords& other)
		{
			int dx = std::abs(m_x - other.m_x);
			int dy = std::abs(m_y - other.m_y);

			return (dx == 1 && dy == 0) || (dx == 0 && dy == 1);
		}

		Direction GetDirectionTo(const Coords& other)
		{
			int dx = m_x - other.m_x;
			int dy = m_y - other.m_y;

			if (dx < 0) return Direction::East();
			if (dx > 0) return Direction::West();
			if (dy < 0) return Direction::South();

			return Direction::North();
		}

		double Distance(const Coords& other)
		{
			int dx = m_x - other.m_x;
			int dy = m_x - other.m_y;

			return std::sqrt(dx*dx + dy*dy);
		}
	};
}