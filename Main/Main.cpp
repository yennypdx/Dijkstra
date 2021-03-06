#include "stdafx.h"
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
#include <limits>
#include "ShortestPath.h"
#include "DijkstraAlg.h"
using namespace std;

int main()
{
	Dijkstra findRoute;
	findRoute.OverloadingDjikstraGraph("OregonMap.txt");
	ShortestPath result;
	string userOrigin;
	string userDestination;
	char next = 'x';
	
	cout << "------------------------------------------" << endl;
	cout << "::: Shortest Path Search via Dijkstra :::" << endl;
	cout << "------------------------------------------" << endl << endl;

	while(true) {
		cout << "Enter Origin: ";
		getline(cin, userOrigin);
		cout << "Enter Destination: ";
		getline(cin, userDestination);
		cout << endl << "Shortest path is being calculated..." << endl << endl;
		result = findRoute.FindShortestPath(userOrigin, userDestination);
		
		auto resultDestination = result.getDestination();
		auto resultDistance = result.getDistance();
		auto resultTime = result.getTime();

		//display result to user
		cout << "The shortest route is: " << endl;
		for (int i = 0; i < resultDestination.size(); i++) {
			cout << " >> " << resultDestination.top() << endl;
			resultDestination.pop();
		}
		if (resultDestination.empty() == false) {
			cout << " >> " << resultDestination.top() << endl;
		}
		cout << "The total distance is: " << resultDistance << " miles." << endl;
		cout << "The estimated total time is: " << resultTime << " hours." << endl;
		cout << endl << endl;
	}
	system("pause");
    return 0;
}