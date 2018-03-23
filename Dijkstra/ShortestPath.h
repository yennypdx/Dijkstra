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
	ShortestPath(const ShortestPath & inCopy);
	ShortestPath & operator=(const ShortestPath & inRhs);
public:
	void setTime(double inTime);
	void setDistance(int inDistance);
	void setDestination(stack<string> inDestination);
public:
	double getTime() { return m_time; }
	int getDistance() { return m_distance; }
	stack<string> getDestination() { return m_destination; }

private:
	double m_time;
	int m_distance;
	stack<string> m_destination;
};

inline ShortestPath::ShortestPath()
{
	m_time = 0.0;
	m_distance = 0;
}

inline ShortestPath::ShortestPath(const ShortestPath & inCopy)
{
	m_time = inCopy.m_time;
	m_distance = inCopy.m_distance;
	m_destination = inCopy.m_destination;
}

inline ShortestPath & ShortestPath::operator=(const ShortestPath & inRhs)
{
	m_time = inRhs.m_time;
	m_distance = inRhs.m_distance;
	m_destination = inRhs.m_destination;
	return *this;
}

inline void ShortestPath::setTime(double inTime)
{
	m_time = inTime;
}

inline void ShortestPath::setDistance(int inDistance)
{
	m_distance = inDistance;
}

inline void ShortestPath::setDestination(stack<string> inDestination)
{
	m_destination = inDestination;
}

#endif