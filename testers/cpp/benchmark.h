#pragma once

#include <chrono>
#include <ctime>
#include <map>
#include <iostream>

#define BENCHMARK(iterations) for (benchmark::iteration_controller bic(iterations); bic.isDone() == false; bic.next())
#define COLLECT_METRIC(format, implementation, id, iterations) for (benchmark::metric_collector bmc(format, implementation, id); bmc.cont(); bmc.next()) BENCHMARK(iterations)

namespace benchmark
{
using iterations_t = int;
using time_t = std::chrono::microseconds;

template <typename T>
void doNotOptimizeAway(T&& x)
{
	// borrowed from https://github.com/DigitalInBlue/Celero/blob/master/include/celero/Utilities.h#L95

	// step one: do a comparison that will always be false but that the compiler can not optimize away
	if (std::chrono::system_clock::now() == std::chrono::time_point<std::chrono::system_clock>())
	{
		// step two: take a reference of the value. this is that actually prevents it from being optimized
		const auto *p = &x;
		// step three: also ensure that the previous line doesn't get optimized away
		std::putchar(*reinterpret_cast<const char *>(p));

		// none of the above will actually execute because of the if condition which always will be false
		// in case we still reach this point for whatever reason we do a hard abort
		std::abort();
	}
}

class iteration_controller
{
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
	iterations_t m_iterationsLeft;
	iterations_t m_iterations;
	static time_t m_lastResult;
	static std::map<iterations_t, time_t> m_emptyRuns;

public:
	explicit iteration_controller(iterations_t iterations = 1000)
	{
		if (m_emptyRuns.find(iterations) == m_emptyRuns.end() && iterations < 0)
		{
			BENCHMARK(-iterations) {}
			m_emptyRuns[-iterations] = lastResult();
		}

		m_iterationsLeft = m_iterations = std::abs(iterations);
		m_startTime = std::chrono::high_resolution_clock::now();
	}
	~iteration_controller()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
		m_lastResult = (std::chrono::duration_cast<time_t>(end - m_startTime) / m_iterations) - m_emptyRuns[m_iterations];
	}

	inline bool isDone() const { return m_iterationsLeft <= 0; }
	inline void next() { --m_iterationsLeft; }

	static time_t lastResult() { return m_lastResult; }
};
time_t lastResult() { return iteration_controller::lastResult(); }

class metric_collector
{
	const std::string m_format, m_implementation, m_id;
	bool m_continue = true;

	static std::string dashIfEmpty(const std::string &string)
	{
		return (string.empty() || string == "" || string == " ") ? "-" : string;
	}

public:
	explicit metric_collector(const std::string &format, const std::string &implementation, const std::string &id)
		: m_format(format), m_implementation(dashIfEmpty(implementation)), m_id(id) {}
	~metric_collector()
	{
		const double seconds = std::chrono::duration_cast<std::chrono::duration<double>>(lastResult()).count();
		std::cout << "value: " << m_format << ' ' << "cpp" << ' ' << m_implementation << ' ' << m_id << ' ' << seconds << std::endl;
	}

	inline bool cont() const { return m_continue; }
	inline void next() { m_continue = false; }
};

time_t iteration_controller::m_lastResult;
std::map<iterations_t, time_t> iteration_controller::m_emptyRuns;
}
