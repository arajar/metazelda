#pragma once

#include <vector>

namespace mz
{
	struct Room;
	struct Symbol;
	struct Bounds;

	class IDungeon
	{
	public:
		virtual std::vector<Room*> GetRooms() = 0;
		virtual int GetRoomCount() const = 0;
		virtual Room* Get(int id) = 0;

		virtual void Add(Room* room) = 0;

		virtual void LinkOneWay(Room* room1, Room* room2) = 0;
		virtual void Link(Room* room1, Room* room2) = 0;

		virtual void LinkOneWay(Room* room1, Room* room2, const Symbol& condition) = 0;
		virtual void Link(Room* room1, Room* room2, const Symbol& condition) = 0;

		virtual bool RoomsAreLinked(const Room& room1, const Room& room2) const = 0;

		virtual Room& FindStart() const = 0;
		virtual Room& FindBoss() const = 0;
		virtual Room& FindGoal() const = 0;
		virtual Room& FindSwitch() const = 0;

		virtual Bounds GetExtentBounds() const = 0;
	};
}