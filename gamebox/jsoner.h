#ifndef JSONER_H
#define JSONER_H
#include <nlohmann/json.hpp>
#include<list>


using json = nlohmann::json;

class JsonSerializable {
public:
    virtual json toJson() const = 0;
};

template<typename T>
json vectorToJson(const std::vector<T>& vec) {
    json j = json::array();
    for (const auto& item : vec) {
        j.push_back(item.toJson());
    }
    return j;
}

template<typename T>
json listToJson(const std::list<T>& list) {
    json j = json::array();
    for (const auto& item : list) {
        j.push_back(item.toJson());
    }
    return j;
}

template<typename T>
json ptrListToJson(const std::list<std::shared_ptr<T>>& list) {
    json j = json::array();
    for (const auto& item : list) {
        if (item) {
            j.push_back(item->toJson());
        }
    }
    return j;
}

template<typename T>
json ptrVecToJson(const std::list<std::shared_ptr<T>>& list) {
    json j = json::array();
    for (const auto& item : list) {
        if (item) {
            j.push_back(item->toJson());
        }
    }
    return j;
}

#endif