#pragma once

#include <algorithm>
#include <vector>
#include <map>
#include <random>

#include "bounds.h"
#include "dungeon.h"
#include "constraints.h"

namespace map
{
	using Random = std::mt19937;

	class IDungeonGenerator
	{
	public:
		virtual void Generate() = 0;
		virtual std::shared_ptr<Dungeon> GetDungeon() = 0;
	};

	class DungeonGenerator : public IDungeonGenerator
	{
	protected:
		class KeyLevelRoomMapping
		{
			std::vector < std::vector<std::shared_ptr<Room>> > map;

		public:
			KeyLevelRoomMapping(const std::shared_ptr<IConstraints>& constraints)
				: map(constraints->GetMaxKeys())
			{
			}

			std::vector<std::shared_ptr<Room>> GetRooms(int keyLevel)
			{
				map.reserve(keyLevel);
				return map[keyLevel];
			}

			void AddRoom(int keyLevel, std::shared_ptr<Room> room)
			{
				map.reserve(keyLevel);
				map[keyLevel].push_back(room);
			}

			size_t KeyCount() const
			{
				return map.size();
			}
		};

	public:
		DungeonGenerator(long seed, const std::shared_ptr<IConstraints>& constraints)
			: seed_(seed)
			, constraints_(constraints)
		{
			random_.seed(seed);
		}

	public:
		virtual void Generate() override
		{
			dungeon_ = std::make_shared<Dungeon>();
			KeyLevelRoomMapping levels(constraints_);

			InitEntranceRoom(levels);
			PlaceRooms(levels);
			PlaceBossGoalRooms(levels);

			PlaceSwitches();
			Graphify();

			ComputeIntensity(levels);
			PlaceKeys(levels);

			int a = 3;
		}

		virtual std::shared_ptr<Dungeon> GetDungeon() override
		{
			return dungeon_;
		}

	protected:
		std::shared_ptr<Room> ChooseRoomWithFreeEdge(const std::vector<std::shared_ptr<Room>>& roomCollection)
		{
			std::vector<std::shared_ptr<Room>> rooms(roomCollection);
			std::shuffle(rooms.begin(), rooms.end(), random_);

			for (const auto& room : rooms)
			{
				auto coords = room->coords_.NextInDirection(Direction::N);
				auto derp = dungeon_->Get(coords);
				if (dungeon_->Get(coords) == nullptr && constraints_->ValidRoomCoords(coords))
				{
					return room;
				}

				coords = room->coords_.NextInDirection(Direction::E);
				derp = dungeon_->Get(coords);
				if (dungeon_->Get(coords) == nullptr && constraints_->ValidRoomCoords(coords))
				{
					return room;
				}

				coords = room->coords_.NextInDirection(Direction::S);
				derp = dungeon_->Get(coords);
				if (dungeon_->Get(coords) == nullptr && constraints_->ValidRoomCoords(coords))
				{
					return room;
				}

				coords = room->coords_.NextInDirection(Direction::W);
				derp = dungeon_->Get(coords);
				if (dungeon_->Get(coords) == nullptr && constraints_->ValidRoomCoords(coords))
				{
					return room;
				}
			}

			return nullptr;
		}

		Direction ChooseFreeEdge(const std::shared_ptr<Room>& room)
		{
			const int maxDirections = static_cast<int>(Direction::Code::Max);

			std::uniform_int_distribution<> rnd(0, maxDirections);
			int d = rnd(random_);

			for (int i = 0; i < maxDirections; ++i)
			{
				auto dir = Direction::FromCode(static_cast<Direction::Code>((d + i) % maxDirections));
				auto coords = room->coords_.NextInDirection(dir);
				if (dungeon_->Get(coords) == nullptr && constraints_->ValidRoomCoords(coords))
				{
					return dir;
				}
			}

			return Direction(Direction::Code::None, 0, 0);
		}

		void InitEntranceRoom(KeyLevelRoomMapping& levels)
		{
			std::vector<Coords> possibleEntries(constraints_->InitialCoords());

			std::uniform_int_distribution<> rnd(0, static_cast<int>(possibleEntries.size() - 1));
			int d = rnd(random_);

			auto coords = possibleEntries[d];

			auto entry = std::make_shared<Room>(coords, nullptr, Symbol::Start, Condition(Symbol::None));
			dungeon_->Add(entry);
			levels.AddRoom(0, entry);
		}

		void PlaceRooms(KeyLevelRoomMapping& levels)
		{
			int roomsPerLock = 0;
			if (constraints_->GetMaxKeys() > 0)
			{
				roomsPerLock = constraints_->GetMaxSpaces() / constraints_->GetMaxKeys();
			}
			else
			{
				roomsPerLock = constraints_->GetMaxSpaces();
			}

			int keyLevel = 0;
			Symbol latestKey = Symbol::None;

			Condition cond(latestKey);

			std::uniform_int_distribution<> rnd(0, 10);

			while (dungeon_->GetRooms().size() < constraints_->GetMaxSpaces())
			{
				bool doLock = false;

				if (levels.GetRooms(keyLevel).size() >= roomsPerLock && keyLevel < constraints_->GetMaxKeys() - 1)
				{
					latestKey = Symbol(keyLevel++);
					cond = cond.And(latestKey);
					doLock = true;
				}

				std::shared_ptr<Room> parent;
				if (!doLock && rnd(random_) > 0)
				{
					parent = ChooseRoomWithFreeEdge(levels.GetRooms(keyLevel));
				}

				if (parent == nullptr)
				{
					parent = ChooseRoomWithFreeEdge(dungeon_->GetRooms());
					doLock = true;
				}

				if (parent == nullptr)
				{
					int a = 3;
				}

				Direction d = ChooseFreeEdge(parent);
				auto coords = parent->coords_.NextInDirection(d);
				auto room = std::make_shared<Room>(coords, parent, Symbol::None, cond);

				dungeon_->Add(room);
				parent->children_.push_back(room);
				dungeon_->Link(parent, room, doLock ? latestKey : Symbol::None);
			}
		}

		void PlaceBossGoalRooms(KeyLevelRoomMapping& levels)
		{
			std::vector<std::shared_ptr<Room>> possibleGoals;

			for (const auto& room : dungeon_->GetRooms())
			{
				if (room->children_.empty())
				{
					int a = 3;
				}
			}

			for (const auto& room : dungeon_->GetRooms())
			{
				if (room->children_.empty() && room->item_ == Symbol::None)
				{
					const auto& parent = room->parent_;
					if (parent == nullptr || parent->children_.size() != 1 || room->item_ != Symbol::None || !parent->precond_.Implies(room->precond_))
					{
						continue;
					}

					possibleGoals.push_back(room);
				}
			}

			std::uniform_int_distribution<> rnd(0, static_cast<int>(possibleGoals.size() - 1));
			int idx = rnd(random_);
			auto goalRoom = possibleGoals[idx];
			auto bossRoom = goalRoom->parent_;

			goalRoom->item_ = Symbol::Goal;
			bossRoom->item_ = Symbol::Boss;

			int oldKeyLevel = bossRoom->precond_.keyLevel_;
			int newKeyLevel = std::min(static_cast<int>(levels.KeyCount()), constraints_->GetMaxKeys()) - 1;
			auto oldRooms = levels.GetRooms(oldKeyLevel);

			auto deleteRoom = [goalRoom, bossRoom](const std::shared_ptr<Room>& room) { return goalRoom == room || bossRoom == room; };
			oldRooms.erase(std::remove_if(oldRooms.begin(), oldRooms.end(), deleteRoom), oldRooms.end());

			levels.AddRoom(newKeyLevel, goalRoom);
			levels.AddRoom(newKeyLevel, bossRoom);

			auto bossKey = Symbol(newKeyLevel - 1);
			auto precond = bossRoom->precond_.And(bossKey);
			bossRoom->precond_ = precond;
			goalRoom->precond_ = precond;

			if (newKeyLevel == 0)
			{
				dungeon_->Link(bossRoom->parent_, bossRoom);
			}
			else
			{
				dungeon_->Link(bossRoom->parent_, bossRoom, bossKey);
			}

			dungeon_->Link(bossRoom, goalRoom);
		}

		void RemoveDescendantsFromList(std::vector<std::shared_ptr<Room>> rooms, const std::shared_ptr<Room>& room)
		{
			auto deleteRoom = [room](const std::shared_ptr<Room>& roomToDelete) { return room == roomToDelete; };
			rooms.erase(std::remove_if(rooms.begin(), rooms.end(), deleteRoom), rooms.end());

			for (auto& child : room->children_)
			{
				RemoveDescendantsFromList(rooms, child);
			}
		}

		void AddPrecond(const std::shared_ptr<Room>& room, const Condition& cond)
		{
			room->precond_ = room->precond_.And(cond);

			for (auto& child : room->children_)
			{
				AddPrecond(child, cond);
			}
		}

		bool SwitchLockChildRooms(const std::shared_ptr<Room>& room, const State& givenState)
		{
			bool anyLocks = false;

			std::uniform_int_distribution<> rnd(0, 2);
			int idx = rnd(random_);

			auto state = givenState != State::Either ? givenState
				: idx == 0 ? State::On : State::Off;

			const int maxDirections = static_cast<int>(Direction::Code::Max);
			for (int i = 0; i < maxDirections; ++i)
			{
				if (room->edge_[i] != Symbol::None)
				{
					const auto& nextRoom = dungeon_->Get(room->coords_.NextInDirection(Direction::FromCode(Direction::Code(i))));
					if (std::find(room->children_.begin(), room->children_.end(), nextRoom) != room->children_.end())
					{
						std::uniform_int_distribution<> rnd(0, maxDirections);
						int idx = rnd(random_);
						if (idx != 0)
						{
							dungeon_->Link(room, nextRoom, ToSymbol(state));
							AddPrecond(nextRoom, Condition(ToSymbol(state)));
							anyLocks = true;
						}
						else
						{
							anyLocks |= SwitchLockChildRooms(nextRoom, state);
						}

						if (givenState == State::Either)
						{
							state = Invert(state);
						}
					}
				}
			}

			return anyLocks;
		}

		std::vector<std::shared_ptr<Room>> GetSolutionPath()
		{
			std::vector<std::shared_ptr<Room>> solution;
			auto room = dungeon_->FindItem(Symbol::Goal);
			while (room != nullptr)
			{
				solution.push_back(room);
				room = room->parent_;
			}

			return solution;
		}

		void PlaceSwitches()
		{
			if (constraints_->GetMaxSwitches() <= 0)
			{
				return;
			}

			auto solution = GetSolutionPath();

			std::vector<std::shared_ptr<Room>> rooms(dungeon_->GetRooms());
			std::shuffle(rooms.begin(), rooms.end(), random_);
			std::shuffle(solution.begin(), solution.end(), random_);

			std::shared_ptr<Room> baseRoom = nullptr;
			for (auto& room : solution)
			{
				if (!room->children_.empty() && room->parent_ != nullptr)
				{
					baseRoom = room;
					break;
				}
			}

			auto baseRoomCond = baseRoom->precond_;
			RemoveDescendantsFromList(rooms, baseRoom);

			std::shared_ptr<Room> switchRoom = nullptr;
			for (auto& room : rooms)
			{
				if (room->item_ == Symbol::None && baseRoomCond.Implies(room->precond_))
				{
					switchRoom = room;
					break;
				}
			}

			if (SwitchLockChildRooms(baseRoom, State::Either))
			{
				switchRoom->item_ = Symbol::Switch;
			}
		}

		void Graphify()
		{
			const int maxDirections = static_cast<int>(Direction::Code::Max);

			for (const auto& room : dungeon_->GetRooms())
			{
				if (room->IsGoal() || room->IsBoss())
				{
					continue;
				}

				for (size_t i = 0; i < maxDirections; i++)
				{
					if (room->edge_[i] != Symbol::None)
					{
						continue;
					}

					auto nextRoom = dungeon_->Get(room->coords_.NextInDirection(Direction::FromCode(Direction::Code(i))));
					if (nextRoom == nullptr || nextRoom->IsGoal() || nextRoom->IsBoss())
					{
						continue;
					}

					bool forwardImplies = room->precond_.Implies(nextRoom->precond_);
					bool backwardImplies = nextRoom->precond_.Implies(room->precond_);

					if (forwardImplies && backwardImplies)
					{
						std::uniform_int_distribution<> rnd(0, 5);
						int idx = rnd(random_);
						if (idx != 0)
						{
							continue;
						}

						dungeon_->Link(room, nextRoom);
					}
					else
					{
						std::uniform_int_distribution<> rnd(0, 5);
						int idx = rnd(random_);
						auto diff = room->precond_.SingleSymbolDifference(nextRoom->precond_);
						if (diff == Symbol::None || ((diff == Symbol::Switch_On || diff == Symbol::Switch_Off) && idx != 0))
						{
							continue;
						}

						dungeon_->Link(room, nextRoom, diff);
					}
				}
			}
		}

		void PlaceKeys(KeyLevelRoomMapping& levels)
		{
			for (int key = 0; key < levels.KeyCount() - 1; ++key)
			{
				auto rooms = levels.GetRooms(key);

				std::shuffle(rooms.begin(), rooms.end(), random_);
				std::sort(rooms.begin(), rooms.end(), [](const std::shared_ptr<Room>& room1, const std::shared_ptr<Room>& room2)
				{
					return room1->intensity_ > room2->intensity_;
				});

				bool placedKey = false;
				for (auto& room : rooms)
				{
					if (room->item_ == Symbol::None)
					{
						room->item_ = Symbol(key);
						placedKey = true;
						break;
					}
				}
			}
		}

		double ApplyIntensity(const std::shared_ptr<Room>& room, double intensity)
		{
			std::uniform_real_distribution<> rnd(0.0, 1.0);
			double r = rnd(random_);
			intensity *= 1.0 - intensity_growth_jitter / 2.0 + intensity_growth_jitter * r;

			room->intensity_ = intensity;
			double maxIntensity = intensity;

			for (const auto& child : room->children_)
			{
				if (room->precond_.Implies(child->precond_))
				{
					maxIntensity = std::max(maxIntensity, ApplyIntensity(child, intensity + 1.0));
				}
			}

			return maxIntensity;
		}

		void NormalizeIntensity()
		{
			double maxIntensity = 0.0;
			// Get the max intensity
			for (const auto& room : dungeon_->GetRooms())
			{
				maxIntensity = std::max(maxIntensity, room->intensity_);
			}

			// Normalize the intensity of the rooms
			for (const auto& room : dungeon_->GetRooms())
			{
				room->intensity_ = room->intensity_ * 0.99 / maxIntensity;
			}
		}

		void ComputeIntensity(KeyLevelRoomMapping& levels)
		{
			double nextLevelBaseIntensity = 0.0;
			for (size_t i = 0; i < levels.KeyCount(); i++)
			{
				double intensity = nextLevelBaseIntensity * (1.0 - intensity_ease_off);

				for (auto& room : levels.GetRooms(i))
				{
					if (room->parent_ == nullptr || !room->parent_->precond_.Implies(room->precond_))
					{
						nextLevelBaseIntensity = std::max(nextLevelBaseIntensity, ApplyIntensity(room, intensity));
					}
				}
			}

			NormalizeIntensity();

			dungeon_->FindItem(Symbol::Boss)->intensity_ = 1.0;
			dungeon_->FindItem(Symbol::Goal)->intensity_ = 0.0;
		}

	protected:
		const double intensity_growth_jitter = 0.1;
		const double intensity_ease_off = 0.2;

		long seed_;
		const std::shared_ptr<IConstraints> constraints_;
		std::shared_ptr<Dungeon> dungeon_;
		Random random_;
	};
}