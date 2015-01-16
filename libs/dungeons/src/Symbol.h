#pragma once

namespace mz
{
	struct Symbol
	{
		enum class RoomItem
		{
			Start = -1,
			Goal = -2,
			Boss = -3,
			Switch_On = -4,
			Switch_Off = -5,
			Switch = -6,
		};

		Symbol(RoomItem value)
			: m_value(value)
		{
		}

		inline bool operator==(const Symbol& symbol)
		{
			return symbol.m_value == m_value;
		}

	public:
		RoomItem GetValue() const
		{
			return m_value;
		}
		
	protected:
		RoomItem m_value;
	};
}