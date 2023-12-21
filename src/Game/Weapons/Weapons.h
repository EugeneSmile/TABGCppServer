#ifndef GAME_WEAPONS_WEAPONS_H_GUARD
#define GAME_WEAPONS_WEAPONS_H_GUARD

#include <vector>
#include <cstdint>
#include <memory>

#include "ServerPtr.h"
#include "Weapon.h"

class Weapons : public ServerPtr
{
private:
public:
    std::vector<std::shared_ptr<Weapon>> weapons;
    void addWeapon(uint32_t id, uint32_t quantity, Vector3f position);
};

#endif