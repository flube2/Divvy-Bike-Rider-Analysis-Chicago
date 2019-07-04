/*main.cpp*/

//
// Divvy analysis: station and ride analysis.
//
// Frank Lubek
// MS VS Pro 2015 on Windows 7 Ultimate
// U. of Illinois, Chicago
// CS341, Spring 2017
// Project #02
//
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <cmath>

using namespace std;
int debugMode = 0;

class Station
{

private:

	// What?
	int ID;
	string Name;

	// Where?
	pair<double, double> Location;

	// How many?
	int Capacity;
	int TotalRides; // Every unique ride from/to station, increment this

					// When/How?
	int Originating; // Every ride STARTING at station, increment this
	int Morning; // 6-9 a.m.
	int Afternoon; // Noon - 1 p.m.
	int Evening; // 3-7 p.m.

	double Distance;


public:

	// Constructor
	Station(int id, string name, pair<double, double>location, int cap)
		: ID(id), Name(name), Location(location), Capacity(cap),
		TotalRides(0), Originating(0), Morning(0), Afternoon(0), Evening(0), Distance(0.0)
	{
		if (debugMode == 1)	cout << ">>Station default constructor" << endl;
	}

	// Copy Constructor:
	Station(const Station &stc /*(stationToCopy)*/)
		: ID(stc.ID), Name(stc.Name), Location(stc.Location), Capacity(stc.Capacity),
		TotalRides(stc.TotalRides), Originating(stc.Originating), Morning(stc.Morning), Afternoon(stc.Afternoon), Evening(stc.Evening),
		Distance(stc.Distance)
	{
		if (debugMode == 1)	cout << "Station copy constructor" << endl;
	}

	// Destructor
	~Station()
	{
		if (debugMode == 1) cout << ">>Station destructor" << endl;
	}



	// Getters and Setters

	//ID, set in constructor
	int getID() const
	{
		return ID;
	}


	//Name, set in constructor
	string getName() const
	{
		return Name;
	}


	//Location, set in constructor
	pair<double, double> getLocation() const
	{
		return Location;
	}

	//Capacity, set in constructor
	int getCapacity() const
	{
		return Capacity;
	}

	//totalRides
	int getTotalRides() const
	{
		return TotalRides;
	}

	void incrementTotalRides()
	{
		++TotalRides;
	}


	//Originating
	int getOriginInfo() const
	{
		return Originating;
	}

	void incrementOrigin()
	{
		++Originating;
	}


	//Morning
	int getMorning() const
	{
		return Morning;
	}

	void incrementMorning()
	{
		++Morning;
	}

	//Afternoon
	int getAfternoon() const
	{
		return Afternoon;
	}

	void incrementAfternoon()
	{
		++Afternoon;
	}

	//Evening
	int getEvening() const
	{
		return Evening;
	}

	void incrementEvening()
	{
		++Evening;
	}

	double getDistance() const
	{
		return Distance;
	}

	void setDistance(double dist)
	{
		Distance = dist;
	}


};






//
// DistBetween2Points: returns the distance in miles between 2 points 
// (lat1, long1) and (lat2, long2).
// 
double distBetween2Points(double lat1, double long1, double lat2, double long2)
{
	//
	// Reference: http://www8.nau.edu/cvm/latlon_formula.html
	//
	constexpr double PI = 3.14159265;
	constexpr double earth_rad = 3963.1;  // statue miles:

	double lat1_rad = lat1 * PI / 180.0;
	double long1_rad = long1 * PI / 180.0;
	double lat2_rad = lat2 * PI / 180.0;
	double long2_rad = long2 * PI / 180.0;

	double dist = earth_rad * acos(
		(cos(lat1_rad)*cos(long1_rad)*cos(lat2_rad)*cos(long2_rad))
		+
		(cos(lat1_rad)*sin(long1_rad)*cos(lat2_rad)*sin(long2_rad))
		+
		(sin(lat1_rad)*sin(lat2_rad))
	);

	return dist;
}


//
// getFileName: inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be opened, an
// error message is output and the program is exited.
//
string getFileName()
{
	string filename;

	// input filename from the keyboard:
	getline(cin, filename);

	// make sure filename exists and can be opened:
	ifstream file(filename);
	if (!file.good())
	{
		cout << "**Error: cannot open file: " << filename << endl;
		std::terminate();
	}

	return filename;
}

/*
// these are just test cases that i used to ensure my struct/class was working correctly
void testFun(vector<Station> &stationList, pair<double, double> loc)
{
Station S1(144, "Station 1", loc, 15);
stationList.emplace_back(S1);
loc.first = 60.34;
loc.second = 88.34;

Station S2(145, "Station 2", loc, 13);
stationList.emplace_back(S2);
loc.first = 55.65;
loc.second = 64.38;

Station S3(146, "Station 3", loc, 12);
stationList.emplace_back(S3);
loc.first = 200.94;
loc.second = 134.59;

Station S4(147, "Station 4", loc, 16);
stationList.emplace_back(S4);
}
*/

void inputStations(vector<Station> &stationList, string stationsFileName, int &totalStations)
{

	if (debugMode == 1) cout << "Just entered inputStations()" << endl;
	ifstream rfile(stationsFileName);
	string line, id, name, lon, lat, cap, temp;

	// burn header
	getline(rfile, line);

	//while not EOF, gather info
	while (getline(rfile, line))
	{
		// parse values from file into temp fields
		stringstream  ss(line);
		getline(ss, id, ',');
		getline(ss, name, ',');
		getline(ss, lat, ',');
		getline(ss, lon, ',');
		getline(ss, cap, ',');
		getline(ss, temp);

		int ID = stoi(id);
		double Lat = stod(lat);
		double Lon = stod(lon);
		pair<double, double> pr(Lat, Lon);
		int Cap = stoi(cap);
		Station S(ID, name, pr, Cap);
		stationList.emplace_back(S);

		++totalStations;

	}
	rfile.close();
}



auto FindStation(vector<Station>& stationList, int stationID)
{
	//
	// TODO
	//

	auto result = find_if(stationList.begin(), stationList.end(),
		[=](Station &s) -> bool { return s.getID() == stationID; });
	return result;

}





void processRides(vector<Station> &stationList, string RidesFileName, int &totalRides, vector<int> &timesOfDay)
{
	if (debugMode == 1) cout << "Just entered processRides()" << endl;

	ifstream rfile(RidesFileName);
	string line, startTime, fromID, fromName, toID, toName, temp;

	// burn header
	getline(rfile, line);

	//while not EOF, gather info
	while (getline(rfile, line))
	{
		// parse values from file into temp fields
		stringstream  ss(line);
		getline(ss, temp, ',');			// 1
		getline(ss, temp, ' ');			// 2
		getline(ss, startTime, ':');	// 2
		getline(ss, temp, ',');			// 2
		getline(ss, temp, ',');			// 3
		getline(ss, temp, ',');			// 4
		getline(ss, temp, ',');			// 5
		getline(ss, fromID, ',');		// 6
		getline(ss, fromName, ',');		// 7
		getline(ss, toID, ',');		// 8
		getline(ss, toName, ',');		// 9
		getline(ss, temp);				// the rest


		int id = stoi(fromID);
		int dest = stoi(toID);
		//convert startTime
		int hour = stoi(startTime);
		auto s = FindStation(stationList, id);
		s->incrementTotalRides();
		s->incrementOrigin();

		if (hour >= 6 && hour < 9) s->incrementMorning();
		if (hour >= 12 && hour < 13) s->incrementAfternoon();
		if (hour >= 15 && hour < 19) s->incrementEvening();

		++timesOfDay[hour];

		auto d = FindStation(stationList, dest);
		d->incrementTotalRides();


		++totalRides;

	}
	rfile.close();


}


void printStationList(const vector<Station> &stationList)
{
	for (const auto &s : stationList)
	{
		cout << "ID:				" << s.getID() << endl;
		cout << "Name:				" << s.getName() << endl;
		cout << "Location:			" << s.getLocation().first << ", " << s.getLocation().second << endl;
		cout << "Capacity:			" << s.getCapacity() << endl;
		cout << "Rides:				" << s.getTotalRides() << endl;
		cout << "Rides from this station:	" << s.getOriginInfo() << endl;
		cout << "Morning:			" << s.getMorning() << endl;
		cout << "Afternoon:			" << s.getAfternoon() << endl;
		cout << "Evening:			" << s.getEvening() << endl << endl;
	}
}








int main()
{


	// setup output format:
	cout << std::fixed;
	cout << std::setprecision(4);

	// get filenames from the user/stdin:
	//string StationsFileName = getFileName();
	//string RidesFileName = getFileName();


	// For debugging
	//
	/*string StationsFileName = "stations-big.csv";*/
    string StationsFileName = "stations.csv";
    //string RidesFileName = "rides-2016-q1.csv";
	string RidesFileName = "rides.csv";


	//
	// TODO:
	//

	int totalStations = 0;
	int totalRides = 0;

	vector<Station> stationList;
	vector<int> timesOfDay(25, 0);

	inputStations(stationList, StationsFileName, totalStations);
	processRides(stationList, RidesFileName, totalRides, timesOfDay);

	/*
	if (debugMode == 1)
	{
	pair<double, double> loc(80.65, 90.20);
	testFun(stationList, loc);
	}
	*/

	cout << "**Num stations: " << totalStations << endl;
	cout << "**Num rides:    " << totalRides << endl;

	//
	// allow user to enter 0 or more commands, stop when 
	// the user inputs the command "exit":
	//
	string command, restOfLine;

	cin >> command;

	while (command != "exit")
	{
		if (command == "find")
		{
			//
			// find latitude longitude distance
			//
			// Example: find 41.86 -87.62 0.5
			//
			double latitude, longitude, distance;
			double key;
			multimap<double, Station> myMap;


			cin >> latitude;
			cin >> longitude;
			cin >> distance;

			for (auto &s : stationList)
			{
				key = distBetween2Points(latitude, longitude, s.getLocation().first, s.getLocation().second);
				s.setDistance(key);
				if (key != 0.0)
				{
					myMap.insert({ key, s });
				}
			}// end for

			for (auto &m : myMap)
			{
				if (m.first > distance) break;
				cout << "Station " << m.second.getID() << ": " << m.second.getDistance() << " miles" << endl;
			}




			//	cout << "todo find" << endl;


		}
		else if (command == "info")
		{
			//
			// info stationID:
			//
			// Example: info 338
			//
			int stationID;

			cin >> stationID;


			//	cout << "todo info" << endl;

			auto s = FindStation(stationList, stationID);
			if (s == stationList.end()) // Not found
			{
				cout << "**Not found..." << endl;

			}
			else
			{

				cout << "Name:     " << "'" << s->getName() << "'" << endl;
				cout << "Position: " << "(" << s->getLocation().first << ", " << s->getLocation().second << ")" << endl;
				cout << "Capacity: " << s->getCapacity() << endl;
				cout << "Total rides to/from: " << s->getTotalRides() << endl;
				cout << "Num rides originating:" << endl;
				cout << "  6-9am:    " << s->getMorning() << endl;
				cout << "  noon-1pm: " << s->getAfternoon() << endl;
				cout << "  3-7pm:    " << s->getEvening() << endl;
			}

		}
		else if (command == "top")
		{
			//
			// top N:
			//
			// Example: top 10
			//
			int N;

			cin >> N;

			if (N < 1)  // invalid input?  Default to top 10:
				N = 10;


			//cout << "todo top N" << endl;

			sort(stationList.begin(), stationList.end(),
				[](Station s1, Station s2)
			{
				if (s1.getTotalRides() == s2.getTotalRides())
				{
					return s1.getName() < s2.getName();
				}
				return s1.getTotalRides() > s2.getTotalRides();
			}
			);


			for (int k = 0; k < N; ++k)
			{
				cout << k + 1 << ". Station " << stationList[k].getID() << ": " << stationList[k].getTotalRides() << " rides @ '"
					<< stationList[k].getName() << "'" << endl;
			}

		}
		else if (command == "hourly")
		{
			//
			// hourly:
			//


			for (int j = 0; j < 24; ++j)
			{
				cout << j << ": " << timesOfDay[j] << endl;
			}
			int sum = accumulate(timesOfDay.begin(), timesOfDay.end(), 0);
			cout << "(" << sum << ")" << endl;
			//cout << "todo hourly rides" << endl;


		}
		else
		{
			cout << "**Invalid, try again" << endl;
		}

		getline(cin, restOfLine);  // discard rest of line:

								   //
								   // input next command and repeat:
		cin >> command;
	}// end while

	 //
	 // done:
	 //
	return 0;
}
