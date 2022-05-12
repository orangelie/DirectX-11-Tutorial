/*
*
* < GameTimer.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include <Windows.h>

using i64 = __int64;

namespace orangelie {

	namespace Time {

		class GameTimer {
		public:
			GameTimer();
			GameTimer& operator=(const GameTimer&) = delete;
			GameTimer(const GameTimer&) = delete;
			~GameTimer();

			void Reset();
			void Stop();
			void Start();
			void Tick();

			float DeltaTime() const;
			float TotalTime() const;

		private:
			i64 m_StopTime, m_PausedTime, m_BaseTime, m_PrevTime, m_CurrTime;
			double m_DeltaTime, m_SecondsPerTick;
			bool m_IsStopped;

		};
	}
}