#include "Player.h"

#include "GeneralHelper.h"

#include "Group.h"
#include "Car.h"
#include "Chunk.h"

Player::Player(uint8_t index, ClientData &&client) : index(index), client(client)
{
    game.aim_down_sight = false;
    game.curse_id = UINT8_MAX;
    game.direction = {0, 0, 0};
    game.flying = false;
    game.health = 100;
    game.knocked = false;
    game.markers.emplace(MarkerType::Ping, Marker{{0, 0, 0}, {0, 0, 0}, MarkerType::Ping, false});
    game.markers.emplace(MarkerType::Marker, Marker{{0, 0, 0}, {0, 0, 0}, MarkerType::Marker, false});
    game.movement_type = MovementType::NoFlags;
    game.position = {0, 0, 0};
    game.rotation = {0, 0};
    game.state = TABGPlayerState::None;
    firing.ammo_type = 0;
    firing.bullet_effect = BulletEffectType::Nothing;
    firing.extra_data.resize(0);
    firing.mode = FiringMode::None;
    driving.car = nullptr;
    driving.is_driving = false;
    driving.seat_id = 0;
    driving.state = DrivingState::None;
    group = nullptr;
    chunk = nullptr;
}

void Player::kill()
{
    game.health = 0;
}
