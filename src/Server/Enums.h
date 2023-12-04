#ifndef SERVER_ENUMS_H_GUARD
#define SERVER_ENUMS_H_GUARD

#include <type_traits>

enum class ClientEventCode
{
    NoCodeSet = 0x00,
    Info = 0x01,
    PlayerUpdate = 0x02,
    Login = 0x03,
    PlayerLeft = 0x04,
    WeaponPickUpAccepted = 0x05,
    PlayerDamaged = 0x06,
    PlayerRespawn = 0x07,
    SpawnGun = 0x08,
    Data = 0x09,
    GameStateChanged = 0x0a,
    ServerRestart = 0x0b,
    AllWeapons = 0x0c,
    SeatAccepted = 0x0d,
    PingRecieved = 0x0e,
    CarUpdate = 0x0f,
    PlayerMarkerEvent = 0x10,
    CarDamage = 0x11,
    TeamDead = 0x12,
    ItemDrop = 0x13,
    RingUpdate = 0x14,
    PlayerAirplaneDropped = 0x15,
    PlaneUpdate = 0x16,
    AllDrop = 0x17,
    RoomInitRequestResponse = 0x18,
    ServerShutDown = 0x19,
    ChunkEntry = 0x1a,
    ChunkExit = 0x1b,
    ReviveState = 0x1c,
    PlayerEnteredChunk = 0x1d,
    PlayerLeftChunk = 0x1e,
    ChatMessage = 0x1f,
    ItemThrown = 0x20,
    WeaponChanged = 0x21,
    GearChange = 0x22,
    ThrowChatMessage = 0x23,
    PlayerHealed = 0x24,
    PlayerDead = 0x25,
    PlayerStateChanged = 0x26,
    PlayerEffect = 0x27,
    PlayerDeathLootDrop = 0x28,
    PlayerFire = 0x29,
    ACMessage = 0x2a,
    PlayerLand = 0x2b,
    KickPlayer = 0x2c,
    SpectatorForce = 0x2d,
    ACRequestedData = 0x2e,
    KickPlayerMessage = 0x2f,
    PlayerRegMessage = 0x30,
    LootRemovedFromMap = 0x31,
    SoulDrop = 0x32,
    CatchPhrase = 0x33,
    BlessingRecieved = 0x34,
    CurseCleansed = 0x35,
    TakeDamageEvent = 0x36,
    PlayerHealthStateChanged = 0x37,
    DropSpawned = 0x38,
    DropOpened = 0x39,
    SyncProjectileEvent = 0x3a,
    PlayerFireSyncReturn = 0x3b,
    LootDropped = 0x3c,
    ScoresChanged = 0x3d,
    BombPlanted = 0x3e,
    GunPurchasedAccepted = 0x3f,
    MoneyChanged = 0x40,
    BombStartDefuse = 0x41,
    BombStopDefuse = 0x42,
    PlayerLootRecieved = 0x43,
    GraveStoneSpawned = 0x44,
    PlayerRespawnFromBoss = 0x45,
    NetworkPlayerTransmittedPackage = 0x46,
    LootPackGiven = 0x47,
    RecievedSteamID = 0x48,
    ServerBrowserHandshake = 0x64,
    SteamTicketAuthenticationResponse = 0x65,
    AssignSteamID = 0xa3,
    FlagMatchOver = 0xa4,
    BossFightResult = 0xa6,
    PackedServerData = 0xa7,
    RequestStopDefuse = 0xa8,
    RequestDefuseBomb = 0xa9,
    RequestPlantBomb = 0xaa,
    RequestPurchaseGun = 0xab,
    RequestSyncProjectileEvent = 0xac,
    RequestClickInteract = 0xad,
    RequestTakeDamageEvent = 0xae,
    RequestCurseCleanse = 0xaf,
    RequestHealthState = 0xb0,
    RequestBlessing = 0xb1,
    SendCatchPhrase = 0xb2,
    RequestRespawnTeamMate = 0xb3,
    RingDeath = 0xb4,
    KickMessage = 0xb5,
    ACCacheState = 0xb6,
    SpectatorRequest = 0xb8,
    CarTemporaryUpdate = 0xbd,
    PassangerUpdate = 0xbe,
    PlayerState = 0xc1,
    RequestHealing = 0xc2,
    PhotonCloseRoom = 0xc3,
    WeaponChange = 0xc6,
    RequestItemThrow = 0xc7,
    RoomInit = 0xca,
    RequestAirplaneDrop = 0xcb,
    RequestItemDrop = 0xce,
    AdminCommand = 0xcf,
    PlayerMarkerAdded = 0xd0,
    TABGPing = 0xd2,
    RequestSeat = 0xd3,
    DamageEvent = 0xd6,
    RequestWeaponPickUp = 0xd7,
    RequestWorldState = 0xda,
    LobbyStats = 0xe0,
    GameServerOffline = 0xe1,
    AppStats = 0xe2,
    QueueState = 0xe4,
    GameListUpdate = 0xe5,
    GameList = 0xe6,
    Ping = 0xf8,
    EventCacheSlicePurged = 0xf9,
    CacheSliceChanged = 0xfa,
    ErrorInfo = 0xfb,
    Disconnect = 0xfc,
    PropertiesChanged = 0xfd,
    Leave = 0xfe,
    Join = 0xff
};

enum class GameMode
{
    BattleRoyale,
    Brawl,
    Test,
    Bomb,
    Deception
};

enum class GameState
{
    WaitingForPlayers,
    CountDown,
    Flying,
    Started,
    Ended,
    OpenDoors,
    RoundOver,
    Intermission,
    Voting,
    VotingOver
};

enum class ServerResponse
{
    Error = 0,
    Accepted = 1,
    SquadDontFit = 2,
    SquadIsBiggerThanTeamSize = 4,
    DontHaveBooking = 8,
    MatchAlreadyStarted = 16
};

enum class PacketContainerFlags
{
    Nothing = 0x0,
    PlayerPosition = 0x1,
    PlayerRotation = 0x2,
    PlayerDirection = 0x4,
    CarPosition = 0x8,
    CarRotation = 0x10,
    CarInput = 0x20,
    All = 0x40
};

inline PacketContainerFlags operator|(PacketContainerFlags lhs, PacketContainerFlags rhs)
{
    return static_cast<PacketContainerFlags>(static_cast<std::underlying_type_t<PacketContainerFlags>>(lhs) | static_cast<std::underlying_type_t<PacketContainerFlags>>(rhs));
}

inline bool operator&(PacketContainerFlags lhs, PacketContainerFlags rhs)
{
    return static_cast<std::underlying_type_t<PacketContainerFlags>>(lhs) & static_cast<std::underlying_type_t<PacketContainerFlags>>(rhs);
}

enum class DrivingState
{
    None = 0x0,
    InsideCar = 0x1,
    Driving = 0x2,
    Slow = 0x4
};

enum class CarDrivingState
{
    None = 0,
    TokyoDrift = 1,
    Honking = 2
};

enum class FiringMode
{
    None = 0x0,
    Semi = 0x1,
    Burst = 0x2,
    FullAutoStart = 0x4,
    FullAutoStop = 0x8,
    ContainsDirection = 0x10,
    RightGun = 0x20,
    WantsToBeSynced = 0x40,
    UseBulletEffect = 0x80
};

inline FiringMode operator|(FiringMode lhs, FiringMode rhs)
{
    return static_cast<FiringMode>(static_cast<std::underlying_type_t<FiringMode>>(lhs) | static_cast<std::underlying_type_t<FiringMode>>(rhs));
}

inline bool operator&(FiringMode lhs, FiringMode rhs)
{
    return static_cast<std::underlying_type_t<FiringMode>>(lhs) & static_cast<std::underlying_type_t<FiringMode>>(rhs);
}

enum class EquipSlots
{
    WeaponSlot01,
    WeaponSlot02,
    WeaponSlot03,
    ThrowableSlot,
    BlessingSlot01,
    BlessingSlot02,
    BlessingSlot03
};

enum class AttatchSlot
{
    Barrel,
    Scope,
    Mag,
    Underbarrel,
    SideBarrel
};

enum class WeaponType
{
    Health,
    Grenade,
    WeaponAttatchment,
    Weapon,
    OtherConsumable,
    Armor,
    Ammo,
    Blessing
};

enum class MarkerType
{
    Ping,
    Marker
};

enum class MarkerActionType
{
    Add,
    Remove
};

#endif