#ifndef TIME_H
#define TIME_H

#include <string>

struct Time
{
	public:
	Time();

	// for alarms, to indicate that the alarm starts on any
	// day, week, etc.
	static const int ANY = -1;
	void inc();

	// decrements time by one second, does not affect
	// days, months and years
	// note, circular
	void decPartial()
	{
		if(--sec_ >= 0)
			return;
		sec_ = 59;
		if(--min_ >= 0)
			return;
		min_ = 59;
		if(--hour_ >= 0)
			return;
		hour_ = 23;
	}

	void zero()
	{
		sec_ = min_ = hour_ = month_ = 0;
		weekday_ = 6;	// 01.01.2000 was Saturday
		year_ = 2000;
	}

	void setSeconds(int value)
	{ sec_ = value; checkInvariant(); }

	void setMinutes(int value)
	{ min_ = value; checkInvariant(); }

	void setHour(int value)
	{ hour_ = value; checkInvariant(); }

	void setMonthday(int value)
	{
		int delta = value - monthday_;
		monthday_ = value;
		weekday_ = (weekday_ + delta)%7;

		checkInvariant();
	}

	void setMonth(int value)
	{ month_ = value; checkInvariant(); }

	void setYear(int value)
	{ year_ = value; checkInvariant(); }

	void incSeconds()
	{
		if(++sec_ == 60)
			sec_ = 0;
	}

	void incMinutes()
	{
		if(++min_ == 60)
			min_ = 0;
	}

	void incHour()
	{
		if(++hour_ == 24)
			hour_ = 0;
	}

	void incMonth(bool isAlarm = false)
	{
		if(month_ == ANY)
			month_ = 0;
		else if(++month_ == 12)
			month_ = isAlarm? ANY : 0;
	}

	void incDay(bool isAlarm = false)
	{
		if(monthday_ == ANY)
			monthday_ = 0;

		else if(++monthday_ == maxMonthday())
			monthday_ = isAlarm? ANY : 0;

		if(++weekday_ == 7)
			weekday_ = 0;
	}
		

	void incYear()
	{
		if(year_ == 2099)
			return;
		++year_;
	}


	int seconds() const { return sec_; }
	int minutes() const { return min_; }
	int hour() const { return hour_; }
	int weekday() const { return weekday_; }
	int month() const { return month_; }
	int monthday() const { return monthday_; }
	int year() const { return year_; }

	private:
	int sec_;
	int min_;
	int hour_;
	int weekday_;
	int month_;
	int monthday_;
	int year_;

	private:
	void checkInvariant() const;

	bool isLeapYear() const
	{
		return (!year_ % 4 && year_ % 100) || (!year_ % 400);
	}

	int maxMonthday() const;
};


#endif
