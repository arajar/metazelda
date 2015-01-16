#pragma once

namespace mz
{
	enum class DirectionType
	{
		North = 0,
		East = 1,
		South = 2,
		West = 3,
	};

	struct Direction
	{
		DirectionType m_dir;
		int m_x;
		int m_y;

		static Direction North()
		{
			Direction dir;
			dir.m_dir = DirectionType::North;
			dir.m_x = 0;
			dir.m_y = -1;

			return dir;
		}

		static Direction East()
		{
			Direction dir;
			dir.m_dir = DirectionType::East;
			dir.m_x = 1;
			dir.m_y = 0;

			return dir;
		}

		static Direction South()
		{
			Direction dir;
			dir.m_dir = DirectionType::South;
			dir.m_x = 0;
			dir.m_y = 1;

			return dir;
		}

		static Direction West()
		{
			Direction dir;
			dir.m_dir = DirectionType::West;
			dir.m_x = -1;
			dir.m_y = 0;

			return dir;
		}

		static DirectionType OppositeDirection(const DirectionType& dir)
		{
			switch (dir)
			{
			case DirectionType::North: return DirectionType::South;
			case DirectionType::South: return DirectionType::North;
			case DirectionType::East: return DirectionType::West;
			case DirectionType::West:
			default:
				return DirectionType::East;
			}
		}

		DirectionType Clockwise() const
		{
			switch (m_dir)
			{
			case DirectionType::North: return DirectionType::East;
			case DirectionType::East: return DirectionType::South;
			case DirectionType::South: return DirectionType::West;
			case DirectionType::West:
			default:
				return DirectionType::North;
			}
		}

		DirectionType Anticlockwise() const
		{
			switch (m_dir)
			{
			case DirectionType::North: return DirectionType::West;
			case DirectionType::East: return DirectionType::North;
			case DirectionType::South: return DirectionType::East;
			case DirectionType::West:
			default:
				return DirectionType::South;
			}
		}
	};
}