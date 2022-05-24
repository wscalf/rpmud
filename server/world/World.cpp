#include "world/World.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <map>

#include "world/Room.h"
#include "world/DirectTransition.h"
#include "util/Log.h"

void World::load(const std::string directory)
{
    std::multimap<std::string, DirectTransition&> openTransitions;

    populateGlobalProperties(directory + "/world.yml");

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory + "/rooms"))
        populateRoomsFromFile(entry.path(), openTransitions);
    
    if (!openTransitions.empty()) {
        for (const auto& it : openTransitions)
            Log::fatal("Exit bound to an undefined room! Room id: " + it.first);
    }
    //Should assert there are no open transitions left - these could result in undefined behavior
}

Room& World::getStartingRoom() const
{
    return *_rooms.at(_startRoomId);
}

Room& World::getSafeRoom() const
{
    return *_rooms.at(_safeRoomId);
}

Room* World::find(std::string id) const
{
    if (_rooms.count(id))
        return _rooms.at(id);
    else
        return nullptr;
}

std::string World::getName() const
{
    return _name;
}

void World::populateGlobalProperties(const std::string& filePath)
{
    auto world = YAML::LoadFile(filePath);

    _name = world["name"].as<std::string>();
    _startRoomId = world["start room"].as<std::string>();
    _safeRoomId = world["safe room"].as<std::string>();
}

void World::populateRoomsFromFile(const std::string& filePath, std::multimap<std::string, DirectTransition&>& openTransitions)
{
    auto nodes = YAML::LoadAllFromFile(filePath);

    for (const auto& node : nodes)
    {
        std::string id = node["id"].as<std::string>();
        Room* room = new Room(UUID::create(), id);

        room->setName(node["name"].as<std::string>());
        room->setDescription(node["description"].as<std::string>());

        _rooms.insert_or_assign(id, room);

        auto exits = node["exits"].as<YAML::Node>();
        for (const auto& exit : exits)
        {
            auto transition = new DirectTransition(UUID::create(), 
                                                    *room,
                                                    exit["name"].as<std::string>(),
                                                    exit["command"].as<std::string>());
            if (exit["description"])
                transition->setDescription(exit["description"].as<std::string>());

            if (exit["tags"])
                for (const auto& tag : exit["tags"])
                    transition->addTag(tag.as<std::string>());

            std::string to = exit["to"].as<std::string>(); //If an element is missing, it throws InvalidNode. Can also query whether an element exists without the .as
            
            room->addLink(std::unique_ptr<Transition>(transition));
            auto other = find(to);
            if (other)
                transition->setDestination(other);
            else
                openTransitions.insert({to, *transition});

            while (openTransitions.count(room->getRoomId())) //The binary search method from here is probably faster: https://www.geeksforgeeks.org/traverse-values-given-key-multimap/
            {
                const auto& pair = openTransitions.find(room->getRoomId());
                pair->second.setDestination(room);
                openTransitions.erase(pair);
            }
        }
    }
}