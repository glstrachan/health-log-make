#include "main.hpp"

month september = {9,  30, "september", TUE, {1, 2, 3, 4, 5, 6, 7}};
month october   = {10, 31, "october"  , THU, {12, 23}};
month november  = {11, 30, "november" , SUN, {11, 30}};
month december  = {12, 31, "december" , TUE, {7, 21, 24, 27, 30}};
month january   = {1,  31, "january"  , FRI, {}};
month february  = {2,  28, "february" , MON, {8}};
month march     = {3,  31, "march"    , MON, {8, 19, 29}};
month april     = {4,  30, "april"    , THU, {16}};
month may       = {5,  31, "may"      , SAT, {3}};
month june      = {6,  30, "june"     , TUE, {}};

string datalog::to_string()
{
	stringstream ss;
	ss << "Date:" << year << "/" << month << "/" << day << ","
	<< activity << "," << location << "," << component << ","
	<< light_time << "," << mod_time << "," << vig_time;

	return ss.str();
}

void log_collection::add(datalog l)
{
	/* Add to the log collection */
	logs.push_back(l);
}

string log_collection::to_string()
{
	/* Save the log in CSV format at the specified path */
	string savelog = "Date,Activity,Location,Component,Light Time,Mod Time, Vig Time\n";

	for(auto newlog : logs)
	{
		savelog += newlog.to_string() + "\n";
	}

	return savelog;
}

datalog activity_pool::request(bool day)
{
	/* Find total weighting */
	int total = 0;

	for(auto activity : pool)
	{
		total += (day ? activity.weight_day : activity.weight_end);
	}

	/* Select random activity */
	int choice = rand() % total;

	int i = 0;
	total = day ? pool[0].weight_day : pool[0].weight_end;
	activity a = pool[0];

	while(choice > total)
	{
		i++;
		total += day ? pool[i].weight_day : pool[i].weight_end;
		a = pool[i];
	}


	string location = a.locations[rand() % a.locations.size()];

	int shift = a.max_dur - a.min_dur;
	int duration = a.min_dur + (shift ? rand() % (a.max_dur - a.min_dur) : 0);
	duration -= duration % 10;

	int light_time = 
	light_time > 0 ? int((double)duration * ((double)a.light / 100.0)) : 0;
	int mod_time = 
	mod_time > 0 ? int((double)duration * ((double)a.mod / 100.0)) : 0;
	int vig_time = 
	vig_time > 0 ? int((double)duration * ((double)a.vig / 100.0)) : 0;

	return {0, 0, 0, a.name, location, a.component, duration, light_time, mod_time, vig_time};
}

// activity_pool parse_activities(const string& filepath)
// {
// 	ifstream stream(filepath);
// 	string line;

// 	enum class STATE
//   {
//     RESET, 
// 		NAME, 
// 		WEIGHT_DAY, WEIGHT_END, 
// 		MAX_DUR, MIN_DUR, 
// 		LIGHT, MOD, VIG,
// 		COMPONENT,
// 		LOCATIONS
//   };

// 	while (getline(stream, line))
//   {
// 		if (line.find("#activity") != std::string::npos)
//     {

// 		}
// 	}
// }

void make(int seed, month m, activity_pool pool, int year)
{
	srand (seed);

	log_collection lc;

	/* Add activites to the log collection */
	for(int date = 1; date <= m.days; date++)
	{
		/* Determine if the current day is reserved */
		if(find(m.reserved.begin(), m.reserved.end(), date) != m.reserved.end()) continue;

		/* Determine if its a weekend */
		bool day = (m.first_day + date + 1) % 7 >= 2;

		/* 20% chance of no activity on weekend */
		if(!day && rand() % 10 > 7) continue;

		datalog a = pool.request(day);
		a.day = date;
		a.month = m.number;
		a.year = year;

		lc.add(a);

		// /* 30% chance of another activity */
		// if(rand() % 10 <= 6) continue;

		// /* Add a second different activity */
		// datalog b = pool.request(day);
		// while(b.activity == a.activity) b = pool.request(day);
		// lc.add(b);
	}

	string savelog = lc.to_string();

	/* Save the savelog to a file */
	ofstream out(m.name + ".csv");
  out << savelog;
  out.close();
}

int main()
{
	activity_pool sept_pool = 
	{{
		{"Running", 4, 4, 20, 40, 40, 40, 20, "Cardiovascular Endurance", {"River Heights", "Grant Park"}},
		{"Biking", 4, 2, 20, 60, 60, 20, 20, "Muscular Endurance", {"Assiniboine Park", "Grant Park", "River Heights"}},
		{"Soccer", 2, 4, 40, 90, 20, 60, 20, "Cardiovacular Endurance", {"Grant Park", "River Heights"}},
		{"Skate Boarding", 1, 1, 20, 40, 60, 40, 0, "Cardiovascular Endurance", {"River Heights"}},
		{"Frisbee", 1, 2, 40, 60, 70, 20, 10, "Cardiovascular Endurance", {"River Heights", "Grant Park"}}
	}};

	activity_pool oct_pool = 
	{{
		{"Biking", 4, 2, 20, 60, 60, 20, 20, "Muscular Endurance", {"Assiniboine Park", "Grant Park", "River Heights"}},
		{"Frisbee", 1, 2, 40, 60, 70, 20, 10, "Cardiovascular Endurance", {"River Heights", "Grant Park"}},
		{"Running", 4, 4, 20, 40, 40, 40, 20, "Cardiovascular Endurance", {"River Heights", "Grant Park"}},
		{"Basketball", 2, 4, 20, 60, 20, 60, 20, "Muscular Endurance", {"River Heights"}},
		{"Soccer", 2, 4, 40, 90, 20, 60, 20, "Cardiovacular Endurance", {"Grant Park", "River Heights"}}
	}};

	activity_pool nov_pool = 
	{{
		{"Running", 4, 4, 20, 40, 40, 40, 20, "Cardiovascular Endurance", {"River Heights", "Grant Park"}},
		{"Soccer", 2, 4, 40, 90, 20, 60, 20, "Cardiovacular Endurance", {"Grant Park", "River Heights"}},
		{"Basketball", 2, 4, 20, 60, 20, 60, 20, "Muscular Endurance", {"River Heights"}}
	}};

	make(100, september, sept_pool, 2020);

	return 0;
}