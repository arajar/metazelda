#pragma once

#include <set>
#include <vector>

#include "Symbol.h"
#include "Edge.h"
#include "Condition.h"
#include "util/Coords.h"

namespace mz
{
	class Room
	{
	public:
		int m_id;

	public:
		Room(int id, const std::set<Coords>& coords, Room* parent, Symbol* item, const Condition& condition)
			: m_id(id)
			, m_coords(coords)
			, m_parent(parent)
			, m_item(item)
			, m_precondition(condition)
			, m_intensity(0.0)
		{
			int x = 0;
			int y = 0;
			for (auto& xy : m_coords)
			{
				x += xy.m_x;
				y += xy.m_y;
			}

			m_center = Coords(x / m_coords.size(), y / m_coords.size());
		}

		Room(int id, Coords coords, Room* parent, Symbol* item, const Condition& condition)
			: Room(id, { coords }, parent, item, condition)
		{
		}

	public:
		double GetIntensity() const
		{
			return m_intensity;
		}

		void SetIntensity(double intensity)
		{
			m_intensity = intensity;
		}

		Symbol* GetItem() const
		{
			return m_item;
		}

		void SetItem(Symbol* item)
		{
			m_item = item;
		}

		std::vector<Edge*> GetEdges() const
		{
			return m_edges;
		}

		Edge* GetEdge(int targetRoomId)
		{
			auto edge = std::find(m_edges.begin(), m_edges.end(), targetRoomId);
			return *edge;
		}

		Edge* SetEdge(int targetRoomId, Symbol* symbol)
		{
			auto e = GetEdge(targetRoomId);
			if (e != nullptr)
			{
				e->m_symbol = symbol;
			}
			else
			{
				e = new Edge(targetRoomId, symbol);
				m_edges.push_back(e);
			}

			return e;
		}

		int LinkCount() const
		{
			return m_edges.size();
		}

		bool IsStart() const
		{
			return m_item != nullptr && m_item->GetValue() == Symbol::RoomItem::Start;
		}

		bool IsGoal() const
		{
			return m_item != nullptr && m_item->GetValue() == Symbol::RoomItem::Goal;
		}

		bool IsBoss() const
		{
			return m_item != nullptr && m_item->GetValue() == Symbol::RoomItem::Boss;
		}

		bool IsSwitch() const
		{
			return m_item != nullptr && m_item->GetValue() == Symbol::RoomItem::Switch;
		}

		Condition GetPrecondition() const
		{
			return m_precondition;
		}

		void SetPrecondition(const Condition& cond)
		{
			m_precondition = cond;
		}

		Room* GetParent() const
		{
			return m_parent;
		}

		void SetParent(Room* parent)
		{
			m_parent = parent;
		}

		std::vector<Room*> GetChildren() const
		{
			return m_children;
		}

		void AddChild(Room* child)
		{
			m_children.push_back(child);
		}

		std::set<Coords> GetCoords() const
		{
			return m_coords;
		}

		Coords GetCenter() const
		{
			return m_center;
		}

	private:
		Condition m_precondition;
		std::set<Coords> m_coords;
		Coords m_center;
		Symbol* m_item;
		std::vector<Edge*> m_edges;
		double m_intensity;
		Room* m_parent;
		std::vector<Room*> m_children;
	};
}