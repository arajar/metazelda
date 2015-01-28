#pragma once

#include <algorithm>

namespace map
{
	enum class Symbol
	{
		Start = -1,
		Goal = -2,
		Boss = -3,
		Switch_On = -4,
		Switch_Off = -5,
		Switch = -6,
		None = 0,
	};

	enum class State
	{
		Either,
		Off,
		On,
	};

	static const Symbol ToSymbol(const State& state)
	{
		switch (state)
		{
		case State::Off: return Symbol::Switch_Off;
		case State::On: return Symbol::Switch_On;
		}
		return Symbol::None;
	}

	static const State Invert(const State& state)
	{
		switch (state)
		{
		case State::Off: return State::On;
		case State::On: return State::Off;
		}
		return State::Either;
	}

	struct Bounds
	{
		int left_;
		int top_;
		int right_;
		int bottom_;

		explicit Bounds(int x, int y, int right, int bottom)
			: left_(x)
			, top_(y)
			, right_(right)
			, bottom_(bottom)
		{
		}

		int width() const
		{
			return right_ - left_ + 1;
		}

		int height() const
		{
			return bottom_ - top_ + 1;
		}
	};

	struct Condition
	{
		int keyLevel_;
		State state_;

		Condition(const Symbol& symbol)
			: keyLevel_(0)
			, state_(State::Either)
		{
			if (symbol == Symbol::Switch_Off)
			{
				state_ = State::Off;
			}
			else if (symbol == Symbol::Switch_On)
			{
				state_ = State::On;
			}
			else
			{
				keyLevel_ = static_cast<int>(symbol);
			}
		}

		Condition(const Condition& other)
			: keyLevel_(other.keyLevel_)
			, state_(other.state_)
		{
		}

		Condition(const State& state)
			: keyLevel_(0)
			, state_(state)
		{
		}

		bool operator==(const Condition& other)
		{
			return keyLevel_ == other.keyLevel_ && state_ == other.state_;
		}

	public:
		Condition And(const Symbol& sym)
		{
			Condition result(*this);
			result.Add(sym);
			return result;
		}

		Condition And(const Condition& other)
		{
			Condition result(*this);
			result.Add(other);
			return result;
		}

		bool Implies(const Symbol& sym)
		{
			return Implies(Condition(sym));
		}

		bool Implies(const Condition& other)
		{
			return keyLevel_ >= other.keyLevel_ && (state_ == other.state_ || other.state_ == State::Either);
		}

		Symbol SingleSymbolDifference(const Condition& other)
		{
			if (*this == other)
			{
				return Symbol::None;
			}

			if (state_ == other.state_)
			{
				return Symbol(std::max(keyLevel_, other.keyLevel_) - 1);
			}
			else
			{
				if (keyLevel_ != other.keyLevel_)
				{
					return Symbol::None;
				}

				if (state_ != State::Either && other.state_ != State::Either)
				{
					return Symbol::None;
				}

				State nonEither = state_ != State::Either ? state_ : other.state_;

				return Symbol(nonEither == State::On ? Symbol::Switch_On : Symbol::Switch_Off);
			}
		}

	private:
		void Add(const Symbol& sym)
		{
			switch (sym)
			{
			case Symbol::Switch_Off: state_ = State::Off; break;
			case Symbol::Switch_On: state_ = State::On; break;
			default: keyLevel_ = std::max(keyLevel_, static_cast<int>(sym)+1); break;
			}
		}

		void Add(const Condition& cond)
		{
			if (state_ == State::Either)
			{
				state_ = cond.state_;
			}

			keyLevel_ = std::max(keyLevel_, cond.keyLevel_);
		}
	};
}