#pragma once
#include <memory>
#include <unordered_map>





template <class T> std::shared_ptr<T> GetTAtPos(float _x, float _y, std::unordered_map<int, std::unordered_map<int, std::shared_ptr<T>>>& m_List)
{
    int x = (int)std::floor(_x);
    int y = (int)std::floor(_y);
    if (m_List.find(x) == m_List.end())
        return NULL;

    if (m_List[x].find(y) == m_List[x].end())
        return NULL;

    return m_List[(int)std::floor(x)][(int)std::floor(y)];
}
