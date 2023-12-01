#ifndef SERVER_ENUMS_H_GUARD
#define SERVER_ENUMS_H_GUARD

#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 256

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

inline ClientEventCode operator|(ClientEventCode a, ClientEventCode b)
{
    return static_cast<ClientEventCode>(static_cast<int>(a) | static_cast<int>(b));
}

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

enum PacketContainerFlags
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

inline PacketContainerFlags operator|(PacketContainerFlags a, PacketContainerFlags b)
{
    return static_cast<PacketContainerFlags>(static_cast<int>(a) | static_cast<int>(b));
}

enum DrivingState
{
    NoDriving = 0x0,
    InsideCar = 0x1,
    Driving = 0x2,
    Slow = 0x4
};

enum CarDrivingState
{
    NoDrivingState = 0,
    TokyoDrift = 1,
    Honking = 2
};

enum FiringMode
{
    NoFiringMode = 0x0,
    Semi = 0x1,
    Burst = 0x2,
    FullAutoStart = 0x4,
    FullAutoStop = 0x8,
    ContainsDirection = 0x10,
    RightGun = 0x20,
    WantsToBeSynced = 0x40,
    UseBulletEffect = 0x80
};

inline FiringMode operator|(FiringMode a, FiringMode b)
{
    return static_cast<FiringMode>(static_cast<int>(a) | static_cast<int>(b));
}

#endif