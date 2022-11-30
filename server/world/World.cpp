#include "world/World.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <map>

#include "world/Room.h"
#include "world/DirectTransition.h"
#include "scripting/ScriptSystem.h"
#include "util/Log.h"
#include "World.h"

void World::load(const std::string directory)
{
    std::multimap<std::string, DirectTransition&> openTransitions;

    populateGlobalProperties(directory + "/world.yml");

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory + "/rooms"))
        populateRoomsFromFile(entry.path(), openTransitions);
    
    if (!openTransitions.empty()) {
        for (const auto& it : openTransitions)
            Log::error("Exit [" + it.second.getName() + "] from room [" + it.second.getFrom().getName() + "], bound to an undefined room id: " + it.first);
        
        throw domain_error("One or more exits were bound to an undefined room. See previous errors for details.");
    }
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
        auto room = new Room(UUID::create(), id);
        auto script = _scripting.create_object("Room");

        room->attachScript(script);

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

            std::string to = exit["to"].as<std::string>();
            
            room->addLink(std::unique_ptr<Transition>(transition));
            auto other = find(to);
            if (other)
                transition->setDestination(other);
            else
                openTransitions.insert({to, *transition});

            auto current = openTransitions.lower_bound(room->getRoomId());
            auto end = openTransitions.upper_bound(room->getRoomId());
            while (current != end)
            {
                const auto pair = *current;
                pair.second.setDestination(room);
                openTransitions.erase(current++);
            }
        }
    }
}

World::World(ScriptSystem& scripting)
    : _scripting {scripting}
{

}