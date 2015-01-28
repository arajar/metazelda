#pragma once

#include <algorithm>
#include <vector>
#include <map>

#include "bounds.h"
#include "util.h"
#include "room.h"

namespace map
{
	struct Dungeon
	{
	protected:
		std::vector<std::shared_ptr<Room>> rooms_;
		Bounds bounds_;

	public:
		Dungeon()
			: bounds_(0, 0, 0, 0)
		{
		}

		std::vector<std::shared_ptr<Room>> GetRooms()
		{
			return rooms_;
		}

		std::shared_ptr<Room> Get(const Coords& coords)
		{
			if (!rooms_.empty())
			{
				for (const auto& room : rooms_)
				{
					if (room->coords_ == coords)
					{
						return room;
					}
				}
			}

			return nullptr;
		}

		std::shared_ptr<Room> Get(int x, int y)
		{
			return Get(Coords(Point(x, y)));
		}

		void Add(const std::shared_ptr<Room>& room)
		{
			if (Get(room->coords_) == nullptr)
			{
				rooms_.push_back(room);
			}

			if (room->coords_.coords_.x < bounds_.left_)
			{
				bounds_ = Bounds(room->coords_.coords_.x, bounds_.top_, bounds_.right_, bounds_.bottom_);
			}

			if (room->coords_.coords_.x > bounds_.right_)
			{
				bounds_ = Bounds(bounds_.left_, bounds_.top_, room->coords_.coords_.x, bounds_.bottom_);
			}

			if (room->coords_.coords_.y < bounds_.top_)
			{
				bounds_ = Bounds(bounds_.left_, room->coords_.coords_.y, bounds_.right_, bounds_.bottom_);
			}

			if (room->coords_.coords_.y > bounds_.bottom_)
			{
				bounds_ = Bounds(bounds_.left_, bounds_.top_, bounds_.right_, room->coords_.coords_.y);
			}
		}

		void LinkOneWay(const std::shared_ptr<Room>& room1, const std::shared_ptr<Room>& room2)
		{
			LinkOneWay(room1, room2, Symbol::None);
		}

		void Link(const std::shared_ptr<Room>& room1, const std::shared_ptr<Room>& room2)
		{
			Link(room1, room2, Symbol::None);
		}

		void LinkOneWay(const std::shared_ptr<Room>& room1, const std::shared_ptr<Room>& room2, const Symbol& symbol)
		{
			auto dir = room1->coords_.GetDirectionTo(room2->coords_);
			room1->edge_[dir] = symbol;
		}

		void Link(const std::shared_ptr<Room>& room1, const std::shared_ptr<Room>& room2, const Symbol& symbol)
		{
			auto dir = room1->coords_.GetDirectionTo(room2->coords_);
			room1->edge_[dir] = symbol;
			room2->edge_[Direction::OppositeDirection(dir)] = symbol;
		}

		bool RoomsAreLinked(const std::shared_ptr<Room>& room1, const std::shared_ptr<Room>& room2)
		{
			auto d = room1->coords_.GetDirectionTo(room2->coords_);
			return room1->GetEdge(d) != Symbol::None || room2->GetEdge(Direction::OppositeDirection(d)) != Symbol::None;
		}

		std::shared_ptr<Room> FindItem(const Symbol& item)
		{
			for (const auto& room : GetRooms())
			{
				if (room->item_ == item)
				{
					return room;
				}
			}

			return nullptr;
		}



	};
}