#ifndef EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP

#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <any>

// stores events and listensers (functions)
class Dispatcher
{
private:
    std::unordered_map<std::type_index, std::any> events;

    template<typename T>
    std::vector<std::function<void(T)>>* getEventArray()
    {
        std::type_index typeName = std::type_index(typeid(T));

        auto it = events.find(typeName);
        if (it == events.end())
        {
            throw std::runtime_error("Event not registered.");
        }

        return std::any_cast<std::vector<std::function<void(T)>>>(&(it->second));
    }

    Dispatcher() {}

    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

public:
    static Dispatcher& getInstance()
    {
        static Dispatcher instance;
        return instance;
    }

    template<typename T>
    void registerEvent()
    {
        std::type_index typeName = std::type_index(typeid(T));

        if (events.find(typeName) != events.end())
        {
            throw std::runtime_error("Event already registered.");
        }

        events.insert({ typeName, std::vector<std::function<void(T)>>{} });
    }

    template<typename T>
    void listen(std::function<void(T)> event)
    {
        getEventArray<T>()->push_back(event);
    }

    template<typename T>
    void call(T event)
    {
        for (std::function<void(T)> func : *getEventArray<T>())
        {
            func(event);
        }
    }
};

#endif