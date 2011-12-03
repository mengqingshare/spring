/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef SPRINGTIME_H
#define SPRINGTIME_H

#if defined(DEBUG) && !defined(SPRING_TIME)
	#define STATIC_SPRING_TIME
#endif

#ifndef SPRING_TIME
	#define SPRING_TIME 1 // boost microsec timer SUCKS atm, when it works again, set to 0
#endif


#ifdef STATIC_SPRING_TIME
	// SDL Timers
	#include <SDL_timer.h>
	#include "System/creg/creg_cond.h"

	struct spring_time {
	private:
		CR_DECLARE(spring_time);
	public:
		spring_time() : x(0) {}
		spring_time(int x_) : x(x_) {}

		spring_time& operator=(const spring_time v)        { x = v.x; return *this; }
		spring_time  operator-(const spring_time& v) const { return spring_time(x - v.x); }
		spring_time  operator+(const spring_time& v) const { return spring_time(x + v.x); }
		bool         operator<(const spring_time& v) const { return (x < v.x); }
		bool         operator>(const spring_time& v) const { return (x > v.x); }
		bool        operator<=(const spring_time& v) const { return (x <= v.x); }
		bool        operator>=(const spring_time& v) const { return (x >= v.x); }
		
		//float tomsecs() const { return x; }
		int  tomsecs() const { return x; }
		bool istime() const { return (x > 0); }
		void sleep() const { SDL_Delay(x); }

	private:
		int x;
	};

	typedef spring_time spring_duration;

	#define spring_tomsecs(t) ((t).tomsecs())
	#define spring_istime(t) ((t).istime())
	#define spring_sleep(t) ((t).sleep())

	#define spring_msecs(msecs) (spring_time(msecs))
	#define spring_secs(secs) (spring_time(secs * 1000))
	#define spring_gettime() (spring_time(SDL_GetTicks()))

	static const spring_time spring_notime(0);

#elif defined(SPRING_TIME)

	// SDL Timers
	#include <SDL_timer.h>

	typedef unsigned spring_time;
	typedef int spring_duration;
	inline spring_time spring_gettime() { return SDL_GetTicks(); };
	#define spring_tomsecs(time) (time)
	#define spring_msecs(time) (time)
	#define spring_secs(time) ((time)*1000)
	#define spring_istime(time) ((time)>0)
	#define spring_sleep(time) SDL_Delay(time)
	#define spring_notime 0

#else

	// boost Timers
	#include <boost/date_time/posix_time/posix_time.hpp>
	#include <boost/date_time/posix_time/ptime.hpp>
	using namespace boost::posix_time;
	typedef ptime spring_time;
	typedef time_duration spring_duration;
	#define spring_tomsecs(time) ((time).total_milliseconds())
	inline spring_time spring_gettime() { return microsec_clock::local_time(); };
	#define spring_msecs(time) (milliseconds(time))
	#define spring_secs(time) (seconds(time))
	#define spring_istime(time) (!(time).is_not_a_date_time())
	#define spring_sleep(time) boost::this_thread::sleep(time)
	#define spring_notime not_a_date_time

#endif

#define spring_difftime(now, before) (now - before)

#endif // SPRINGTIME_H
