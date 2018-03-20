#include "stdafx.h"
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include "ShortestPath.h"
#include "DijkstraAlg.h"
using namespace std;

int main()
{
	Dijkstra findRoute;
	string userOrigin;
	string userDestination;
	
	cout << "----------------------------------------" << endl;
	cout << "Shortest Path Search via Dijkstra" << endl;
	cout << "----------------------------------------" << endl << endl;

	cout << "Enter Origin: ";
	cin >> userOrigin;
	cout << "Enter Destination: ";
	cin >> userDestination;
	cout << endl << endl << "Shortest path is being calculated..." << endl;
	findRoute.FindShortestPath(userOrigin, userDestination);
	cout << "Route suggested: " << endl;
	//TODO: to be continued


	system("pause");
    return 0;
}