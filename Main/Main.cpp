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
	findRoute.OverloadingDjikstraGraph("OregonMap.txt");
	//findRoute.OverloadingDjikstraGraph("TestFile.txt");
	ShortestPath result;
	string userOrigin;
	string userDestination;
	char next = 'x';
	
	cout << "------------------------------------------" << endl;
	cout << "::: Shortest Path Search via Dijkstra :::" << endl;
	cout << "------------------------------------------" << endl << endl;

	do {
		cout << "Enter Origin: ";
		//cin >> userOrigin;
		cout << "Enter Destination: ";
		//cin >> userDestination;
		cout << "Shortest path is being calculated..." << endl << endl;
		findRoute.FindShortestPath("Eugene", "Bend");
		
		auto resultDestination = result.getDestination();
		auto resultDistance = result.getDistance();
		auto resultTime = result.getTime();

		//display result to user
		cout << "The shortest route is: " << endl;
		while (!resultDestination.empty()) {
			auto content = resultDestination.top();
			cout << content << " -> " << endl;
			resultDestination.pop();
		}
		cout << "The total distance is: " << resultDistance << " miles." << endl;
		cout << "The estimated total time is: " << resultTime << " hours." << endl;

		cout << endl << "Find new route [y/n]? ";
		cin >> next;
	} while (next == 'y' || next == 'Y');
	
	system("pause");
    return 0;
}