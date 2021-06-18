#pragma once

#include <string>
#include <vector>

// #include <bits/stdc++.h>

enum FIRST_DAY
{
	MON,
	TUE,
	WED,
	THU,
	FRI,
	SAT,
	SUN
};

struct Activity
{
	std::string name;
	/* Weight for weekdays */
	int weight_day;

	/* Weight for weekends */
	int weight_end;

	/* Maximum and minimum durations for the activity (minutes) */
	int max_dur;
	int min_dur;

	/* Weighted time for activity (%) */
	int light;
	int mod;
	int vig;

	std::string component;

	/* Possible locations */
	std::vector<std::string> locations;
};

struct Month
{
	int number;
	int days;
	std::string name;

	FIRST_DAY first_day;

	/* vector of reserved days */
	/* No events will be scheduled for these days */
	std::vector<int> reserved;
};

/* Represents a single entry */
class Datalog
{
public:
	int day;
	int month;
	int year;

	std::string activity;
	std::string location;
	std::string component;

	int duration;

	int light_time;
	int mod_time;
	int vig_time;

	std::string to_string();
};

/* Represents a month of datalogs */
class LogCollection
{
public:
	std::vector<Datalog> logs;

	void add(Datalog l);

	std::string to_string();
};

/* Used for all of the activites and weights for a month */
class ActivityPool
{
public:
	std::vector<Activity> pool;

	Datalog request(bool day);
};

/* To be implemented */
// activity_pool parse_activities(const string& filepath);

void make(int seed, Month m, ActivityPool pool);