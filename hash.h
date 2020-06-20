/*hash.h*/

//
// Everardo Gutierrez
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 

#include <string>

using namespace std;

bool isNumeric(string s);
int HashId(int id, int N);
int HashAbbrev(string abbrev, int N);
int HashTrip(string trip, int N);
int HashBike(string bike, int N);