#ifndef SERVER_TYPES_H_GUARD
#define SERVER_TYPES_H_GUARD

#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 256

#include <string>
#include <cstdint>

enum ClientEventCode
{
    NoCodeSet,
    Info,
    PlayerUpdate,
    Login,
    PlayerLeft,
    WeaponPickUpAccepted,
    PlayerDamaged,
    PlayerRespawn,
    SpawnGun,
    Data,
    GameStateChanged,
    ServerRestart,
    AllWeapons,
    SeatAccepted,
    PingRecieved,
    CarUpdate,
    PlayerMarkerEvent,
    CarDamage,
    TeamDead,
    ItemDrop,
    RingUpdate,
    PlayerAirplaneDropped,
    PlaneUpdate,
    AllDrop,
    RoomInitRequestResponse,
    ServerShutDown,
    ChunkEntry,
    ChunkExit,
    ReviveState,
    PlayerEnteredChunk,
    PlayerLeftChunk,
    ChatMessage,
    ItemThrown,
    WeaponChanged,
    GearChange,
    ThrowChatMessage,
    PlayerHealed,
    PlayerDead,
    PlayerStateChanged,
    PlayerEffect,
    PlayerDeathLootDrop,
    PlayerFire,
    ACMessage,
    PlayerLand,
    KickPlayer,
    SpectatorForce,
    ACRequestedData,
    KickPlayerMessage,
    PlayerRegMessage,
    LootRemovedFromMap,
    SoulDrop,
    CatchPhrase,
    BlessingRecieved,
    CurseCleansed,
    TakeDamageEvent,
    PlayerHealthStateChanged,
    DropSpawned,
    DropOpened,
    SyncProjectileEvent,
    PlayerFireSyncReturn,
    LootDropped,
    ScoresChanged,
    BombPlanted,
    GunPurchasedAccepted,
    MoneyChanged,
    BombStartDefuse,
    BombStopDefuse,
    PlayerLootRecieved,
    GraveStoneSpawned,
    PlayerRespawnFromBoss,
    NetworkPlayerTransmittedPackage,
    LootPackGiven,
    RecievedSteamID,
    ServerBrowserHandshake = 100,
    SteamTicketAuthenticationResponse,
    AssignSteamID = 163,
    FlagMatchOver,
    BossFightResult = 166,
    PackedServerData,
    RequestStopDefuse,
    RequestDefuseBomb,
    RequestPlantBomb,
    RequestPurchaseGun,
    RequestSyncProjectileEvent,
    RequestClickInteract,
    RequestTakeDamageEvent,
    RequestCurseCleanse,
    RequestHealthState,
    RequestBlessing,
    SendCatchPhrase,
    RequestRespawnTeamMate,
    RingDeath,
    KickMessage,
    ACCacheState,
    SpectatorRequest = 184,
    CarTemporaryUpdate = 189,
    PassangerUpdate,
    PlayerState = 193,
    RequestHealing,
    PhotonCloseRoom,
    WeaponChange = 198,
    RequestItemThrow,
    RequestAirplaneDrop = 203,
    RoomInit = 202,
    RequestItemDrop = 206,
    AdminCommand,
    PlayerMarkerAdded,
    TABGPing = 210,
    RequestSeat,
    DamageEvent = 214,
    RequestWeaponPickUp,
    RequestWorldState = 218,
    LobbyStats = 224,
    GameServerOffline,
    AppStats,
    QueueState = 228,
    GameListUpdate,
    GameList,
    Ping = 248,
    EventCacheSlicePurged,
    CacheSliceChanged,
    ErrorInfo,
    Disconnect,
    PropertiesChanged,
    Leave,
    Join
};

enum GameMode
{
    BattleRoyale,
    Brawl,
    Test,
    Bomb,
    Deception
};

enum GameState
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

enum ServerResponse
{
    Error = 0,
    Accepted = 1,
    SquadDontFit = 2,
    SquadIsBiggerThanTeamSize = 4,
    DontHaveBooking = 8,
    MatchAlreadyStarted = 16
};

struct Vector3f
{
    float x;
    float y;
    float z;
};

struct LoginData
{
    std::string player_name;
    std::string gravestone_text;
    uint64_t login_key;
    bool squad_host;
    uint8_t squad_members;
    uint32_t gear_length;
};

#endif