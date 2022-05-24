#pragma once

#include <string>
#include <map>

class Room;
class DirectTransition;

class World
{
    public:
        void load(const std::string directory);

        Room* find(std::string id) const;
        Room& getStartingRoom() const;
        Room& getSafeRoom() const;

        std::string getName() const;
    private:
        void populateGlobalProperties(const std::string& filePath);
        void populateRoomsFromFile(const std::string& filePath, std::multimap<std::string, DirectTransition&>& openTransitions);
        std::string _startRoomId;
        std::string _safeRoomId;
        std::string _name;

        std::map<std::string, Room*> _rooms;
};