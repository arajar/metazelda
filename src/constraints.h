#pragma once

#include <algorithm>
#include <vector>
#include <map>

#include "bounds.h"
#include "dungeon.h"

namespace map
{
	class IConstraints
	{
	public:
		virtual std::vector<Coords> InitialCoords() = 0;
		virtual bool ValidRoomCoords(const Coords& c) = 0;

		virtual int GetMaxSpaces() = 0;
		virtual int GetMaxKeys() = 0;
		virtual int GetMaxSwitches() = 0;

		virtual bool IsAcceptable(const std::shared_ptr<Dungeon>& dungeon) = 0;
	};

	class CountConstraints : public IConstraints
	{
	public:
		CountConstraints(int maxSpaces, int maxKeys, int maxSwitches)
			: maxSpaces_(maxSpaces)
			, maxKeys_(maxKeys)
			, maxSwitches_(maxSwitches)
		{
		}

	public:
		virtual std::vector<Coords> InitialCoords() override
		{
			return{ Coords(Point(0, 0)) };
		}

		virtual bool ValidRoomCoords(const Coords& c) override
		{
			return true;
		}

		virtual int GetMaxSpaces() override
		{
			return maxSpaces_;
		}

		virtual int GetMaxKeys() override
		{
			return maxKeys_;
		}

		virtual int GetMaxSwitches() override
		{
			return maxSwitches_;
		}

		virtual bool IsAcceptable(const std::shared_ptr<Dungeon>& dungeon) override
		{
			return true;
		}

	protected:
		int maxSpaces_;
		int maxKeys_;
		int maxSwitches_;
	};
}