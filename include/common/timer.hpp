#pragma once

#include <chrono>
#include <iostream>

namespace qlm
{
	using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using sec = std::chrono::seconds;
	using msec = std::chrono::milliseconds;
	using usec = std::chrono::microseconds;
	using nsec = std::chrono::nanoseconds;

	template<typename T>
	const char* GetType()
	{
		if constexpr (std::is_same<T, sec>::value)
		{
			return " sec\n";
		}
		else if constexpr (std::is_same<T, msec>::value)
		{
			return " msec\n";
		}
		else if constexpr (std::is_same<T, usec>::value)
		{
			return " usec\n";
		}
		else
		{
			return " nsec";
		}	
	}


	template<typename duration_t>
	struct Timer
	{
	public:
		time_point start_time, end_time;
		duration_t duration;
	public:
		void start()
		{
			start_time = std::chrono::high_resolution_clock::now();
		}
		void end()
		{
			end_time = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<duration_t>(end_time - start_time);
		}

		float Duration() const
		{
			return duration.count();
		}
		
		void show() const
		{
			std::cout << "Time taken: " << duration.count() << GetType<duration_t>();
		}
	};
}
