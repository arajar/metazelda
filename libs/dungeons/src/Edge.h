#pragma once

#include "Symbol.h"
namespace mz
{
	struct Edge
	{
	public:
		Symbol* m_symbol;

	public:
		Edge(int targetRoomId, Symbol* symbol)
			: m_targetRoomId(targetRoomId)
			, m_symbol(symbol)
		{
		}

		bool HasSymbol() const
		{
			m_symbol != nullptr;
		}

		int GetTargetRoomId() const
		{
			return m_targetRoomId;
		}

		inline bool operator==(int id)
		{
			return m_targetRoomId == id;
		}

	protected:
		int m_targetRoomId;
	};
}