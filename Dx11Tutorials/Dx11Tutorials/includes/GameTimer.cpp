/*
*
* < GameTimer.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "GameTimer.h"

namespace orangelie {

	namespace Time {

		GameTimer::GameTimer() {
			m_DeltaTime = -1.0;
			m_StopTime = m_PausedTime = m_BaseTime = m_PrevTime = m_CurrTime = 0;
			m_IsStopped = false;

			i64 TicksPerSecond;
			QueryPerformanceFrequency((_LARGE_INTEGER*)&TicksPerSecond);
			m_SecondsPerTick = (1.0 / TicksPerSecond);
		}

		GameTimer::~GameTimer() {
		}

		void GameTimer::Reset() {
			i64 currTime;
			QueryPerformanceCounter((_LARGE_INTEGER*)&currTime);

			m_BaseTime = m_PrevTime = currTime;
			m_StopTime = 0;
			m_IsStopped = false;
		}

		void GameTimer::Stop() {
			if (m_IsStopped == false) {
				i64 currTime;
				QueryPerformanceCounter((_LARGE_INTEGER*)&currTime);

				m_StopTime = currTime;
				m_IsStopped = true;
			}
		}

		void GameTimer::Start() {
			if (m_IsStopped == true) {
				i64 currTime;
				QueryPerformanceCounter((_LARGE_INTEGER*)&currTime);

				m_PausedTime += (currTime - m_StopTime);
				m_PrevTime = currTime;
				m_StopTime = 0;
				m_IsStopped = false;
			}
		}

		void GameTimer::Tick() {
			if (m_IsStopped) {
				m_DeltaTime = 0.0;
				return;
			}

			i64 currTime;
			QueryPerformanceCounter((_LARGE_INTEGER*)&currTime);
			m_CurrTime = currTime;
			m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerTick;
			m_PrevTime = m_CurrTime;

			if (m_DeltaTime < 0.0) {
				m_DeltaTime = 0.0;
			}
		}

		float GameTimer::DeltaTime() const {
			return (float)m_DeltaTime;
		}

		float GameTimer::TotalTime() const {
			float minusTime = (float)m_CurrTime;

			if (m_IsStopped) {
				minusTime = (float)m_StopTime;
			}

			return (((minusTime - (float)m_PausedTime) - (float)m_BaseTime) * (float)m_SecondsPerTick);
		}
	}
}