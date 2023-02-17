#pragma once

#include <chrono>
#include <iostream>

using time_point = std::chrono::time_point<std::chrono::steady_clock>;
using duration_us = std::chrono::microseconds;

struct Timer
{
public:
	time_point start_time, end_time;
	duration_us duration;
public:
	void start()
	{
		start_time = std::chrono::high_resolution_clock::now();
	}
	void end()
	{
		end_time = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
	}
	void show()
	{
		std::cout << "Time taken: " << duration.count() << " microsecond\n";
	}
};
