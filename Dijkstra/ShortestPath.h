#include "stdafx.h"
#pragma once

#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include <iostream>
#include <stack>
using namespace std;

class ShortestPath
{
public:
	ShortestPath();
	~ShortestPath() = default;
public:
	void setTime(double inTime) { m_time = inTime; }
	void setDistance(double inDistance) { m_distance = inDistance; }
	void setDestination(stack<string> inDestination) { m_destination = inDestination; }
public:
	double getTime() { return m_time; }
	double getDistance() { return m_distance; }
	stack<string> getDestination() { return m_destination; }

private:
	double m_time;
	double m_distance;
	stack<string> m_destination;
};

inline ShortestPath::ShortestPath()
{
	m_time = 0.0;
	m_distance = 0.0;
}

#endif