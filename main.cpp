/*main.cpp*/

//
// Everardo Gutierrez
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project 06: DIVVY Data Analysis 

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#include "hash.h"
#include "hashmap.h"
#include "util.h"

using namespace std;

//
// Station
// 
struct Station
{
	int id, capacity;
	string abbrev, fullName, latitude, longitude, onlineDate;
	Station()
	{
		id = 0;
		abbrev = "";
		fullName = "";
		latitude = "";
		longitude = "";
		capacity = 0;
		onlineDate = "";
	}
};

struct StationNear{
	double miles; int station;
	StationNear(){
		miles = 0.0;
		station = 0;
	}
};
//
// Trip
// 
struct Trip
{
	string id, startTime, stopTime, bikeId, duration, fromStation, toStation, rider, birthYear;
	Trip()
	{
		id = "";
		startTime = "";
		stopTime = "";
		bikeId = "";
		duration = "";
		fromStation = "";
		toStation = "";
		rider = "";
		birthYear = "";
	}
};

//
// Bike
// 
struct Bike
{
	string id;
	int usage;
	Bike()
	{
		id = "";
		usage = 0;
	}
};

// mileSort
// Given two instances of struct StationNear we sort by miles 
// and if miles are the same by there station id.
bool mileSort(const StationNear& s1, const StationNear& s2){
	if(s1.miles == s2.miles)
		return s1.station < s2.station;
	return s1.miles < s2.miles;
}

//
// stationData
// Given a filename denoting station information,
// inputs that data into the given hash tables.
// 
bool stationData(string filename, hashmap<int, Station>& stationsById, hashmap<string, string>& stationsByAbbrev){
	ifstream  infile(filename);
	int count = 0;
	if (!infile.good()){
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}
	// file is open, start processing:
	string line;
	getline(infile, line);  // input and discard first row --- header row
 	while (getline(infile, line))
 	{
 		stringstream s(line);
 		string id, abbrev, fullName, latitude, longitude, capacity, onlineDate;
		
		getline(s, id, ','); 
		getline(s, abbrev, ',');
		getline(s, fullName, ',');   
		getline(s, latitude, ',');
		getline(s, longitude, ',');
		getline(s, capacity, ',');
		getline(s, onlineDate, ',');
		
		//
		// instance of data structure station
		// 
		Station stat;
		stat.id = stoi(id);
		stat.abbrev = abbrev;
		stat.fullName = fullName;
		stat.latitude = latitude;
		stat.longitude = longitude;
		stat.capacity = stoi(capacity);
		stat.onlineDate = onlineDate;

		//
		// store into hash table
		//
 		stationsById.insert(stat.id, stat, HashId);
		
		stationsByAbbrev.insert(abbrev, id, HashAbbrev);
		
 		count++;
 	}
	cout << "# of stations: " << count << endl;
	return true;
}

//
// tripData
// Given a filename denoting trip information,
// inputs that data into the given hash tables.
// 
bool tripData(string filename, hashmap<string, Trip>& tripsById, hashmap<string, Bike>& bikesById){
	ifstream  infile(filename);
	int trips = 0, bikes = 0;
	if (!infile.good()){
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}
	// file is open, start processing:
	string line;
	getline(infile, line);  // input and discard first row --- header row
	while (getline(infile, line))
	{
		stringstream s(line);
		string id, startTime, stopTime, bikeId, duration, fromStation, toStation, rider, birthYear;
		
		getline(s, id, ','); 
		getline(s, startTime, ',');
		getline(s, stopTime, ',');
		getline(s, bikeId, ',');   
		getline(s, duration, ',');
		getline(s, fromStation, ',');
		getline(s, toStation, ',');
		getline(s, rider, ',');
		getline(s, birthYear, ',');
		
		//
		// instance of data structure station
		// 
		Trip tr;
		tr.id = id;
		tr.startTime = startTime;
		tr.stopTime = stopTime;
		tr.bikeId = bikeId;
		tr.duration = duration;
		tr.fromStation = fromStation;
		tr.toStation = toStation;
		tr.rider = rider;
		tr.birthYear = birthYear;

		//
		// store into hash table
		//
		tripsById.insert(id, tr, HashTrip);
		
		Bike br;
		if(bikesById.search(bikeId, br, HashBike)){
		  br.usage = br.usage + 1;
		  bikesById.insert(bikeId, br, HashBike);
		}else{
		  br.id = bikeId;
		  br.usage = 1;
		  bikesById.insert(bikeId, br, HashBike);  
		  bikes++;
		}	
		trips++;	
	}
	cout << "# of trips: " << trips << endl;
	cout << "# of bikes: " << bikes << endl;
	
	return true;
}

//
// displayCommands
// Display all available commands for user to enter
// 
void displayCommands(){
	cout << "Available commands:" << endl;
	cout << "Enter a station id (e.g. 341)" << endl;
	cout << "Enter a station abbreviation (e.g. Adler)" << endl;
	cout << "Enter a trip id (e.g. Tr10426561)" << endl;
	cout << "Enter a bike id (e.g. B5218)" << endl;
	cout << "Nearby stations (e.g. nearby 41.86 -87.62 0.5)" << endl;
	cout << "Similar trips (e.g. similar Tr10424639)" << endl;
}

// stationID
// Given a station id passed as a parameter to the function. It will search 
// in the stationsById hashmap and print out the stations information if
// it exists, if it does not then display station not found.
// 
void stationID(string command, hashmap<int, Station>& stationsById){
	Station sid;
	bool found = stationsById.search(stoi(command), sid, HashId); // search for id
	if(!found){ // station does not exist 
		cout << "Station not found" << endl;
	}else{ // station exists display its information
		cout << "Station:" << endl;
		cout << "  ID: " << sid.id << endl;
		cout << "  Abbrev: " << sid.abbrev << endl;
		cout << "  Fullname: " << sid.fullName << endl;
		cout << "  Location: (" << stod(sid.latitude) << ", " << stod(sid.longitude) << ")" << endl;
		cout << "  Capacity: " << sid.capacity << endl;
		cout << "  Online date: " << sid.onlineDate << endl;
	}
}

// stationAbbrev
// Given a station abbreviation passed as a parameter to the function. It will search 
// in the stationsByAbbrev hashmap and then use the returned string value to search in the stationsById.
// If the station exists in the stationsById hashmap we display its information, if it does not then display station not found.
// 
void stationAbbrev(string command, hashmap<int, Station>& stationsById, hashmap<string, string>& stationsByAbbrev){
	Station sid;
	string ret;
	bool found = stationsByAbbrev.search(command, ret, HashAbbrev); // search for abbreviation
	if(!found){ // abbreviation does not exist 
		cout << "Station not found" << endl;
	}else{ // abbreviation exists display its information
		found = stationsById.search(stoi(ret), sid, HashId); 
		if(!found){
			cout << "Station not found" << endl;
		}else{
			cout << "Station:" << endl;
			cout << "  ID: " << sid.id << endl;
			cout << "  Abbrev: " << sid.abbrev << endl;
			cout << "  Fullname: " << sid.fullName << endl;
			cout << "  Location: (" << stod(sid.latitude) << ", " << stod(sid.longitude) << ")" << endl;
			cout << "  Capacity: " << sid.capacity << endl;
			cout << "  Online date: " << sid.onlineDate << endl;
		}
	}
}

// tripID
// Given a trip id passed as a parameter to the function. It will search 
// in the tripsById hashmap and print out the trips information if
// it exists, if it does not then display station not found.
// 
void tripID(string command, hashmap<string, Trip>& tripsById, hashmap<int, Station>& stationsById){
	Trip tr;
	bool found = tripsById.search(command, tr, HashTrip);
	if(!found){
		cout << "Trip not found" << endl;
	}else{
		Station st;
		cout << "Trip:" << endl;
		cout << "  ID: " << tr.id << endl;
		cout << "  Starttime: " << tr.startTime << endl;
		cout << "  Bikeid: " << tr.bikeId << endl;
		
		int total = stoi(tr.duration);
		int hours = total / 3600;
		total = total % 3600;
		int minutes = total / 60;
		total = total % 60;
		int seconds = total;
		cout << "  Duration: ";
		if(hours > 0)
			cout << hours << " hours, ";
		if(minutes > 0)
			cout << minutes << " minutes, ";
		cout << seconds << " seconds" << endl;
		
		stationsById.search(stoi(tr.fromStation), st, HashId); // search in stationsById hashmap to get from station abbreviation
		cout << "  From station: " << st.abbrev <<  " (" << tr.fromStation << ")" << endl; 
		stationsById.search(stoi(tr.toStation), st, HashId); // search in stationsById hashmap to get to station abbreviation
		cout << "  To station: " << st.abbrev << " (" << tr.toStation << ")" << endl; 
		cout << "  Rider identifies as: " << tr.rider << endl;
		cout << "  Birthyear: " << tr.birthYear << endl;
	}
}

// bikeID
// Given a bike id passed as a parameter to the function. It will search 
// in the bikesById hashmap and print out the bikes usage information if
// it exists, if it does not then display station not found.
// 
void bikeID(string command, hashmap<string, Bike>& bikesById){
	Bike br;
	bool found = bikesById.search(command, br, HashBike);
	if(!found){
		cout << "Bike not found" << endl;
	}else{
		cout << "Bike:" << endl;
		cout << "  ID: " << br.id << endl;
		cout << "  Usage: " << br.usage << endl;
	}
}

// nearbySearch
// Function is to find all near by stations in proximity to the latitude and longitude coordinates read as input.
// Distance function distBetween2Points will be calculated using the coordinates read and that of each station 
// in the stationsById hashmap. At the end we display all near by and how many miles away.
// 
void nearbySearch(string command, hashmap<int, Station>& stationsById){
	stringstream ss(command);
	string ignore, latitude, longitude, miles;
	getline(ss, ignore, ' ');
	getline(ss, latitude, ' ');
	getline(ss, longitude, ' ');
	getline(ss, miles, ' ');
	
	cout << "Stations within " << miles << " miles of " << "(" << latitude << ", " << longitude << ")" << endl;
	Station s;
	StationNear sn;
	unordered_set <int> keys = stationsById.getKeys(); // call function getKeys to get all the keys in the stationsById hashmap
	vector<StationNear> m;
	for(auto itr = keys.begin(); itr != keys.end(); itr++){ // loop through the unordered_set 
		bool found = stationsById.search(*itr, s, HashId);
		if(!found){ // check to see if the station exist
			continue;
		}
		// calculate distance between current read station and coordinates user entered
		double distance = distBetween2Points(stod(latitude), stod(longitude), stod(s.latitude), stod(s.longitude));
		if(distance <= stod(miles)){ // check to see if station distance within the miles user entered
			sn.miles = distance; 
			sn.station = *itr;
			m.push_back(sn); // station is nearby, push an instance of StationNear to the back of the vector 
		}
		 
	}
 	sort(m.begin(),m.end(), mileSort); // sort the vector by calling mileSort function
	if(m.size() == 0) // no stations are nearby
		cout << "  none found" << endl;
	else{ // look through the vector printing each individual station 
		for(auto i = m.begin(); i != m.end(); ++i){
			sn = *i;
			cout << "  station " <<  sn.station << ": " << sn.miles << " miles" << endl;
		}	
	}
}

//
// addStations
// Function is to calculate distance between a starting or ending station and the current trips starting and ending stations.
// If the distance for either the starting station or ending station is within the miles the user entered, add the from or 
// to station to the vector.
//
void addStations(double mile, double latitude, double longitude, Station& S, Station& D, vector<int>& vec){
		// calculate distance between our starting point and starting point of current trip form unordered_set
		double dist = distBetween2Points(latitude, longitude, stod(S.latitude), stod(S.longitude)); 
		
		// calculate distance between our starting point and ending point of current trip form unordered_set
		double dist2 = distBetween2Points(latitude, longitude, stod(D.latitude), stod(D.longitude));
		
		// check if distance calculated above is within the miles entered by the user, if so add the station id to the 
		// back of its corresponding vector if it doesnt already exist in the vector
		if(dist <= mile){ 
			if(find(vec.begin(), vec.end(), S.id) == vec.end())
				vec.push_back(S.id);
		}	
		if(dist2 <= mile){
			if(find(vec.begin(), vec.end(), D.id) == vec.end())
				vec.push_back(D.id);
		}
}

//
// nearbySearch
// Function is to find all similar trips as the one entered by the user. Along with finding all nearby by stations 
// to the station the trip began at and all stations nearby where it ended. 
// 
void similarSearch(string command, hashmap<int, Station>& stationsById, hashmap<string, Trip>& tripsById){
	stringstream ss(command);
	string ignore, trip, miles;
	getline(ss, ignore, ' ');
	getline(ss, trip, ' ');
	getline(ss, miles, ' ');
	
	double mile = stod(miles); 
	
	cout << "Trips that follow a similar path (+/-" << miles << " miles) as " << trip << endl;
	Trip tr;
	bool found = tripsById.search(trip, tr, HashTrip);
	if(!found){ // trip does not exist no need to continue
		cout << "  no such trip" << endl;
		return;
	}
	unordered_set<string> keys = tripsById.getKeys(); // call function getKeys to get all the keys in the tripsById hashmap
	
	Station S;
	stationsById.search(stoi(tr.fromStation), S, HashId); // search for from station to get latitude and longitude coordinates to save for later
	double latS = stod(S.latitude);
	double longS = stod(S.longitude);
	vector<int> starting; // vector for all nearby stations where we started
	
	Station D;
	stationsById.search(stoi(tr.toStation), D, HashId); // search for to station to get latitude and longitude coordinates to save for later
	double latD = stod(D.latitude);
	double longD = stod(D.longitude);
	vector<int> ending; // vector for all nearby stations where we ended
	
	int trips = 0;
	for(auto itr = keys.begin(); itr != keys.end(); itr++){ // look through each trip in the unordered_set
 		bool found = tripsById.search(*itr, tr, HashTrip);
		if(!found)
			continue;
		// search stationsById hashmap for station to get latitude and longitude coordinates
		stationsById.search(stoi(tr.fromStation), S, HashId);
		stationsById.search(stoi(tr.toStation), D, HashId);
		
		addStations(mile, latS, longS, S, D, starting);
		addStations(mile, latD, longD, S, D, ending);
		
		// we update the number of similar trips if and only if the current trip from station id 
		// exists in the starting vector and the current trip to station id exists in the ending vector
		if(find(starting.begin(), starting.end(), S.id) != starting.end() &&
		  find(ending.begin(), ending.end(), D.id) != ending.end())
			trips++;
	}
	
	// sort the two vectors by station id
	sort(starting.begin(), starting.end()); 
	sort(ending.begin(), ending.end());
	
	cout << "  nearby starting points: ";
	for(auto itr = starting.begin(); itr != starting.end(); itr++) // print out all the nearby starting point station ids
		cout << *itr << " ";
	cout << endl;
	cout << "  nearby ending points: ";
	for(auto itr = ending.begin(); itr != ending.end(); itr++) // print out all the nearby ending point station ids
		cout << *itr << " ";
	cout << endl;
	cout << "  trip count: " << trips << endl; // print the number of similar trips
}

int main()
{
	const int N1 = 10000;
	const int N2 = 2500000;
	const int N3 = 50000;
	hashmap<int, Station> stationsById(N1);
	hashmap<string, string> stationsByAbbrev(N1);
	hashmap<string, Trip> tripsById(N2);
	hashmap<string, Bike> bikesById(N3);
	string stationsFileName;
	string tripsFileName;
	string fileName, fileName2;
	
	cout << "** DIVVY analysis program **" << endl;
	cout << endl;
	
	//
	// get data file
	//
	cout << "Enter stations file> ";
	getline(cin , stationsFileName);
	cout << "Enter trips file> ";
	getline(cin, tripsFileName);
	cout << endl;
	cout << "Reading " << stationsFileName << endl;
	cout << "Reading " << tripsFileName << endl;
	cout << endl;
	
	//
	// read in the data and check if we were successful at doing so
	// 
	bool success = stationData(stationsFileName, stationsById, stationsByAbbrev);
	if(!success){
		cout << "No data, file not found?" << endl;
		return 0;
	}
	success = tripData(tripsFileName, tripsById, bikesById);
	if(!success){
		cout << "No data, file not found?" << endl;
		return 0;
	}

	//
	// allow the user to lookup data 
	// 
	string command;
	cout << endl;
	cout << "Please enter a command, help, or #> ";
	getline(cin, command);
	
	while(command != "#"){
		if(isNumeric(command)){ // search for a station by id
			stationID(command, stationsById);
		}else if(command.substr(0,2) == "Tr"){ // search for a trip by id 
			tripID(command, tripsById, stationsById);
		}else if(command[0] == 'B'){ // search for bike usage by id 
			bikeID(command, bikesById);
		}else if(command.substr(0,6) == "nearby"){ // find nearby stations to the coordinates passed
			nearbySearch(command, stationsById);
		}else if(command.substr(0,7) == "similar"){ // find similar trips to the one we entered
			similarSearch(command, stationsById, tripsById);
		}else if(command.substr(0, 4) == "help"){ // display all valid commands user can enter
			displayCommands();
		}else{  // search for a station by abbreviation, if all else doesnt pertain to what the user entered
			stationAbbrev(command, stationsById, stationsByAbbrev);
		}
		cout << endl;
		cout << "Please enter a command, help, or #> ";
		getline(cin, command);
	}
	
	return 0;
}
