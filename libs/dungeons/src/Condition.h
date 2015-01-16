#pragma once

#include "Symbol.h"
#include <algorithm>

namespace mz
{
	enum class SwitchState
	{
		Either,
		Off,
		On,
	};

	static Symbol ToSymbol(const SwitchState& state)
	{
		switch (state)
		{
		case SwitchState::Off:
			return Symbol(Symbol::RoomItem::Switch_Off);
			break;
		case SwitchState::On:
			return Symbol(Symbol::RoomItem::Switch_On);
			break;
		default:
			break;
		}
	}

	static SwitchState Invert(const SwitchState& state)
	{
		switch (state)
		{
		case SwitchState::Off: return SwitchState::On;
		case SwitchState::On: return SwitchState::Off;
		default:
			return state;
		}
	}

	struct Condition
	{
		int m_keyLevel;
		SwitchState m_switchState;

		Condition()
			: m_keyLevel(0)
			, m_switchState(SwitchState::Either)
		{
		}

		Condition(const SwitchState& state)
			: m_keyLevel(0)
			, m_switchState(state)
		{
		}

		Condition(const Symbol& symbol)
			: m_keyLevel(0)
		{
			if (symbol.GetValue() == Symbol::RoomItem::Switch_Off)
			{
				m_keyLevel = 0;
				m_switchState = SwitchState::Off;
			}
			else if (symbol.GetValue() == Symbol::RoomItem::Switch_On)
			{
				m_keyLevel = 0;
				m_switchState = SwitchState::On;
			}
			else
			{
				m_keyLevel = static_cast<int>(symbol.GetValue()) + 1;
				m_switchState = SwitchState::Either;
			}
		}

		inline bool operator==(const Condition& condition)
		{
			return m_keyLevel == condition.m_keyLevel && m_switchState == condition.m_switchState;
		}

		void Add(const Symbol& symbol)
		{
			if (symbol.GetValue() == Symbol::RoomItem::Switch_Off)
			{
				m_switchState = SwitchState::Off;
			}
			else if (symbol.GetValue() == Symbol::RoomItem::Switch_On)
			{
				m_switchState = SwitchState::On;
			}
			else
			{
				m_keyLevel = std::max(m_keyLevel, static_cast<int>(symbol.GetValue()) + 1);
			}
		}

		void Add(const Condition* condition)
		{
			if (m_switchState == SwitchState::Either)
			{
				m_switchState = condition->m_switchState;
			}

			m_keyLevel = std::max(m_keyLevel, condition->m_keyLevel + 1);
		}

		Condition* And(const Symbol& symbol)
		{
			auto result = new Condition(*this);
			result->Add(symbol);
			return result;
		}

		Condition* And(Condition* other)
		{
			if (other == nullptr)
			{
				return this;
			}

			auto result = new Condition(*this);
			result->Add(other);
			return result;
		}

		bool Implies(Condition* condition)
		{
			return m_keyLevel >= condition->m_keyLevel &&
				(m_switchState == condition->m_switchState || condition->m_switchState == SwitchState::Either);
		}

		bool Implies(const Symbol& symbol)
		{
			// TODO: Refactor this
			auto condition = new Condition(symbol);
			bool implied = Implies(condition);
			delete condition;
			return implied;
		}

		Symbol* SingleSymbolDifference(Condition* condition)
		{
			if (this == condition)
			{
				return nullptr;
			}

			if (m_switchState == condition->m_switchState)
			{
				return new Symbol(static_cast<Symbol::RoomItem>(std::max(m_keyLevel, condition->m_keyLevel) - 1));
			}
			else
			{
				if (m_keyLevel != condition->m_keyLevel)
				{
					return nullptr;
				}

				if (m_switchState != SwitchState::Either && condition->m_switchState != SwitchState::Either)
				{
					return nullptr;
				}

				SwitchState nonEither = m_switchState != SwitchState::Either
					? m_switchState
					: condition->m_switchState;

				return new Symbol(nonEither == SwitchState::On
					? Symbol::RoomItem::Switch_On
					: Symbol::RoomItem::Switch_Off);
			}
		}
	};

}