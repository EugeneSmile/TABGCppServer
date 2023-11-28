#include "Player.h"

Player::Player(const LoginData &data)
    : group(data.squad_members), name(data.player_name)
{
}