#pragma once

#include <string>

namespace mz
{
	struct Bounds
	{
	public:
		int m_left;
		int m_top;
		int m_right;
		int m_bottom;

	public:
		Bounds(int x, int y, int right, int bottom)
			: m_left(x)
			, m_top(y)
			, m_right(right)
			, m_bottom(bottom)
		{
		}

		int Width() const
		{
			return m_right - m_left + 1;
		}

		int Height() const
		{
			return m_bottom - m_top + 1;
		}

		std::string ToString() const
		{
			std::string tmp = "Bounds (";
			tmp += m_left;
			tmp += ", ";
			tmp += m_top;
			tmp += ", ";
			tmp += m_right;
			tmp += ", ";
			tmp += m_bottom;
			tmp += ")";

			return tmp;
		}
	};
}