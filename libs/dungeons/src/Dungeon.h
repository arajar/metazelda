#pragma once

#include <map>

#include "IDungeon.h"
#include "Room.h"
#include "Bounds.h"

namespace mz
{
	class Dungeon : public IDungeon
	{
	public:
		Dungeon()
			: m_itemCount(0)
			, m_bounds(std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::min(), std::numeric_limits<int>::min())
		{
		}

	public:
		virtual Bounds GetExtentBounds() const
		{
			return m_bounds;
		}

		virtual std::map<int, Room*> GetRooms() override
		{
			return m_rooms;
		}

		virtual int GetRoomCount() const override
		{
			m_rooms.size();
		}

		virtual Room* Get(int id) override
		{
			return m_rooms[id];
		}

		virtual void Add(Room* room) override
		{
			m_rooms[room->m_id] = room;

			for (auto& xy : room->GetCoords())
			{
				if (xy.m_x < m_bounds.m_left)
				{
					m_bounds = Bounds(xy.m_x, m_bounds.m_top, m_bounds.m_right, m_bounds.m_bottom);
				}

				if (xy.m_x > m_bounds.m_right)
				{
					m_bounds = Bounds(m_bounds.m_left, m_bounds.m_top, xy.m_x, m_bounds.m_bottom);
				}

				if (xy.m_y < m_bounds.m_top)
				{
					m_bounds = Bounds(m_bounds.m_left, xy.m_y, m_bounds.m_right, m_bounds.m_bottom);
				}

				if (xy.m_y > m_bounds.m_bottom)
				{
					m_bounds = Bounds(m_bounds.m_left, m_bounds.m_top, m_bounds.m_right, xy.m_y);
				}
			}
		}




	protected:
		int m_itemCount;
		std::map<int, Room*> m_rooms;
		Bounds m_bounds;
	};
}