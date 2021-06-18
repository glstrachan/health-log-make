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
	ss << year << "/" << month << "/" << day << ","
	<< activity << "," << location << "," << component << ","
	<< duration << "," << light_time << "," << mod_time << "," << vig_time;

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
	string savelog = "Date,Activity,Location,Component,Duration (min),Light Time (min),Mod Time (min), Vig Time (min)\n";

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

	int light_time = 
	a.light > 0 ? int((double)duration * ((double)a.light / 100.0)) : 0;
	light_time -= light_time % 5;
	int mod_time = 
	a.mod > 0 ? int((double)duration * ((double)a.mod / 100.0)) : 0;
	mod_time -= mod_time % 5;
	int vig_time = 
	a.vig > 0 ? int((double)duration * ((double)a.vig / 100.0)) : 0;
	vig_time -= vig_time % 5;

	duration = light_time + mod_time + vig_time;

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

		/* 40% chance of no activity on weekend */
		if(!day && rand() % 10 > 5) continue;

		/* 10% chance of no activity on weekday */
		if(day && rand() % 10 > 8) continue;

		datalog a = pool.request(day);
		a.day = date;
		a.month = m.number;
		a.year = year;

		lc.add(a);

		/* 20% chance of another activity */
		if(rand() % 10 <= 7) continue;

		/* Add a second different activity */
		datalog b = pool.request(day);
		b.day = date;
		b.month = m.number;
		b.year = year;

		while(b.activity == a.activity)
		{
			b = pool.request(day);
			b.day = date;
			b.month = m.number;
			b.year = year;
		} 
		lc.add(b);
	}

	string savelog = lc.to_string();

	/* Save the savelog to a file */
	ofstream out(m.name + ".csv");
  out << savelog;
  out.close();

	cout << "Saved new log to " << m.name << ".csv";
}

int main()
{
	activity_pool sept_pool = 
	{{
		{"Running", 3, 3, 20, 30, 40, 40, 20, "Cardiovascular Endurance", {"River Heights", "Grant Park"}},
		{"Biking", 4, 2, 20, 40, 60, 20, 20, "Muscular Endurance", {"Assiniboine Park", "Grant Park", "River Heights"}},
		{"Soccer", 2, 4, 40, 60, 20, 60, 20, "Cardiovacular Endurance", {"Grant Park", "River Heights"}},
		{"Skate Boarding", 1, 1, 20, 40, 60, 40, 0, "Cardiovascular Endurance", {"River Heights"}},
		{"Basketball", 1, 2, 20, 50, 40, 40, 20, "Muscular Endurance", {"River Heights"}},
		{"Walk", 4, 4, 20, 40, 80, 20, 0, "Cardiovascular Endurance", {"River Heights"}},
		{"Frisbee", 1, 2, 15, 30, 70, 20, 10, "Cardiovascular Endurance", {"River Heights", "Grant Park"}}
	}};

	activity_pool oct_pool = 
	{{
		{"Running", 4, 3, 20, 30, 40, 40, 20, "Cardiovascular Endurance", {"River Heights", "Grant Park"}},
		{"Biking", 2, 1, 20, 40, 60, 20, 20, "Muscular Endurance", {"Assiniboine Park", "Grant Park", "River Heights"}},
		{"Soccer", 2, 4, 40, 60, 20, 60, 20, "Cardiovacular Endurance", {"Grant Park", "River Heights"}},
		{"Skate Boarding", 1, 0, 20, 40, 60, 40, 0, "Cardiovascular Endurance", {"River Heights"}},
		{"Basketball", 2, 3, 20, 50, 40, 40, 20, "Muscular Endurance", {"River Heights"}},
		{"Walk", 5, 5, 20, 40, 80, 20, 0, "Cardiovascular Endurance", {"River Heights"}},
		{"Frisbee", 1, 1, 15, 30, 70, 20, 10, "Cardiovascular Endurance", {"River Heights", "Grant Park"}}
	}};

	activity_pool nov_pool = 
	{{
		{"Running", 2, 1, 20, 30, 40, 40, 20, "Cardiovascular Endurance", {"River Heights", "Grant Park"}},
		{"Cold Soccer", 2, 1, 40, 60, 20, 60, 20, "Cardiovacular Endurance", {"Grant Park", "River Heights"}},
		{"Walk", 3, 2, 20, 40, 90, 10, 0, "Cardiovascular Endurance", {"River Heights"}},
		{"Shovelling", 0, 1, 20, 40, 80, 20, 0, "Muscular Endurance", {"House"}}
	}};

	activity_pool dec_pool = 
	{{
		{"Running", 0, 1, 20, 30, 40, 40, 20, "Cardiovascular Endurance", {"River Heights", "Grant Park"}},
		{"Cold Soccer", 1, 0, 40, 60, 20, 60, 20, "Cardiovacular Endurance", {"Grant Park", "River Heights"}},
		{"Walk", 3, 2, 20, 40, 80, 20, 0, "Cardiovascular Endurance", {"River Heights"}},
		{"Shovelling", 3, 2, 20, 40, 50, 50, 0, "Muscular Endurance", {"House"}},
		{"Sledding", 1, 1, 20, 40, 80, 20, 0, "Muscular Endurance", {"House"}},
		{"Skating", 2, 2, 30, 60, 40, 60, 0, "Muscular Endurance", {"River Heights"}},
		{"Hockey", 2, 2, 30, 60, 20, 60, 20, "Muscular Endurance", {"River Heights"}}
	}};

	activity_pool jan_pool = 
	{{
		{"Walk", 2, 1, 20, 40, 80, 20, 0, "Cardiovascular Endurance", {"River Heights"}},
		{"Shovelling", 3, 2, 20, 40, 60, 40, 0, "Muscular Endurance", {"House"}},
		{"Sledding", 3, 2, 20, 40, 80, 20, 0, "Muscular Endurance", {"House"}},
		{"Skating", 2, 2, 30, 60, 40, 60, 0, "Muscular Endurance", {"River Heights"}},
		{"Hockey", 2, 3, 30, 60, 20, 60, 20, "Muscular Endurance", {"River Heights"}}
	}};

	activity_pool feb_pool = 
	{{
		{"Walk", 3, 2, 20, 40, 80, 20, 0, "Cardiovascular Endurance", {"River Heights"}},
		{"Shovelling", 2, 1, 20, 40, 60, 40, 0, "Muscular Endurance", {"House"}},
		{"Sledding", 3, 2, 20, 40, 80, 20, 0, "Muscular Endurance", {"House"}},
		{"Skating", 2, 2, 30, 60, 40, 60, 0, "Muscular Endurance", {"River Heights"}},
		{"Hockey", 2, 3, 30, 60, 20, 60, 20, "Muscular Endurance", {"River Heights"}}
	}};

	make(0, september, sept_pool, 2020);
	make(1, october, oct_pool, 2020);
	make(2, november, nov_pool, 2020);
	make(3, december, dec_pool, 2020);
	make(4, january, jan_pool, 2021);
	make(5, february, feb_pool, 2021);
	make(6, march, mar_pool, 2021);
	make(7, april, apr_pool, 2021);
	make(8, may, may_pool, 2021);
	make(9, june, june_pool, 2021);

	return 0;
}