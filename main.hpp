#ifndef MAIN_H
#define MAIN_H

#include <bits/stdc++.h>
using namespace std;

enum FIRST_DAY {MON, TUE, WED, THU, FRI, SAT, SUN};

typedef struct activity
{
	string name;
	/* Weight for weekdays */
	int weight_day;

	/* Weight for weekends */
	int weight_end;

	/* Maximum and minimum durations for the activity (minutes) */
	/* Duration acts only as a hint */
	/* Computed duration varies to attain whole numbers */
	int max_dur;
	int min_dur;

 	/* Weighted time for activity (%) */
	int light;
	int mod;
	int vig;

	string component;

	/* Possible locations */
	vector<string> locations;
} activity ;

typedef struct month
{
	int number;
	int days;
	string name;

	FIRST_DAY first_day;

	/* vector of reserved days */
	/* No events will be scheduled for these days */
	vector<int> reserved;
} month;

/* Represents a single entry */
typedef class datalog
{
	public:
	int day;
	int month;
	int year;

	string activity;
	string location;
	string component;

	int duration;

	int light_time;
	int mod_time;
	int vig_time;

	string to_string();
} datalog;

/* Represents a month of datalogs */
typedef class log_collection
{
public:
	vector<datalog> logs;

	void add(datalog l);

	string to_string();
} log_collection;

/* Used for all of the activites and weights for a month */
typedef class activity_pool
{
	public:
	vector<activity> pool;

	datalog request(bool day);
} activity_pool;

/* To be implemented */
// activity_pool parse_activities(const string& filepath);

void make(int seed, month m, activity_pool pool);

int main();

#endif /* MAIN_H */