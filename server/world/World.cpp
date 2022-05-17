#include "world/World.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>

#include "world/Room.h"

void World::load(const std::string directory)
{
    //Need a way to complete transitions as rooms are loaded - maybe a multimap from the room being linked to to transitions?
    //Transition could be updated so it can be created and bound to a target in two steps, and the room can be assigned the transition as soon as it's made
    populateGlobalProperties(directory + "/world.yml");

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory + "/rooms"))
        populateRoomsFromFile(entry.path());
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
    return _rooms.at(id);
}

std::string World::getName() const
{
    return _name;
}

void World::populateGlobalProperties(const std::string& filePath)
{
    YAML::Node world = YAML::LoadFile(filePath);

    _name = world["name"].as<std::string>();
    _startRoomId = world["start room"].as<std::string>();
    _safeRoomId = world["safe room"].as<std::string>();
}

void World::populateRoomsFromFile(const std::string& filePath)
{
    std::vector<YAML::Node> nodes = YAML::LoadAllFromFile(filePath);
    for (const auto& node : nodes)
    {
        std::string id = node["id"].as<std::string>();
        Room* room = new Room(UUID::create(), id);

        room->setName(node["name"].as<std::string>());
        room->setDescription(node["description"].as<std::string>());

        _rooms.insert_or_assign(id, room);
    }
}