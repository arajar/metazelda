#pragma once

#include <algorithm>
#include <vector>
#include <map>

#include <memory>

#include "bounds.h"
#include "util.h"

namespace map
{
	struct Room
	{
		Condition precond_;
		Coords coords_;
		double intensity_;
		Symbol item_;
		std::shared_ptr<Room> parent_;
		std::vector<std::shared_ptr<Room>> children_;
		Symbol edge_[static_cast<int>(Direction::Code::Max)]; // Index with Directions

		Room(const Coords& coords, const std::shared_ptr<Room>& parent, const Symbol& item, const Condition& precond)
			: coords_(coords)
			, parent_(parent)
			, item_(item)
			, precond_(precond)
			, intensity_(0.0)
		{
			for (int i = 0; i < static_cast<int>(Direction::Code::Max); ++i)
			{
				edge_[i] = Symbol::None;
			}
		}

		Symbol GetEdge(const Direction& dir)
		{
			return edge_[dir];
		}

		int LinkCount()
		{
			int result = 0;
			for (const auto& edge : edge_)
			{
				if (edge != Symbol::None)
				{
					++result;
				}
			}

			return result;
		}

		bool IsStart() const
		{
			return item_ == Symbol::Start;
		}

		bool IsGoal() const
		{
			return item_ == Symbol::Goal;
		}

		bool IsBoss() const
		{
			return item_ == Symbol::Boss;
		}

		bool IsSwitch() const
		{
			return item_ == Symbol::Switch;
		}
	};
}