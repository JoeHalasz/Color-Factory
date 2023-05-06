#pragma once
#include <memory>
#include <unordered_map>
#include <iostream>




template <class T> std::shared_ptr<T> GetTAtPos(float _x, float _y, std::unordered_map<int, std::unordered_map<int, std::shared_ptr<T>>>& m_List, bool printIt = false)
{
    int x = (int)std::floor(_x);
    int y = (int)std::floor(_y);
    if (printIt)
        std::cout << "was " << _x << " " << _y << " changed to " << x << " " << y << std::endl;
    if (m_List.find(x) == m_List.end())
        return NULL;

    if (m_List[x].find(y) == m_List[x].end())
        return NULL;

    return m_List[(int)std::floor(x)][(int)std::floor(y)];
}


enum Direction
{
	DirectionUp = 0,
	DirectionRight = 1,
	DirectionDown = 2,
	DirectionLeft = 3,
};

int InvertDirection(Direction d);