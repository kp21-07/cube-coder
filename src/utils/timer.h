#pragma once

#include <time.h>

class Timer {
public:
	Timer() { start(); }

	void start()
	{
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		sec  = ts.tv_sec;
		nsec = ts.tv_nsec;

		stop_sec  = 0;
		stop_nsec = 0;
		running   = true;
	}

	void reset() { start(); }

	// Freeze the elapsed time. Further calls to elapsed_ms/elapsed_sec
	// return the duration up to this point, not the current time.
	void stop()
	{
		if (!running) return;

		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		stop_sec  = ts.tv_sec;
		stop_nsec = ts.tv_nsec;
		running   = false;
	}

	double elapsed_ms() const
	{
		long long end_sec, end_nsec;

		if (running) {
			timespec ts;
			clock_gettime(CLOCK_MONOTONIC, &ts);
			end_sec  = ts.tv_sec;
			end_nsec = ts.tv_nsec;
		} else {
			end_sec  = stop_sec;
			end_nsec = stop_nsec;
		}

		long long ds  = end_sec  - sec;
		long long dns = end_nsec - nsec;

		return ds * 1000.0 + dns / 1e6;
	}

	double elapsed_sec() const { return elapsed_ms() / 1000.0; }

	bool is_running() const { return running; }

private:
	long long sec,      nsec;
	long long stop_sec, stop_nsec;

	bool running = true;
};
