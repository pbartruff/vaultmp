#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <map>
#include <list>
#include <math.h>

#include "vaultmp.h"
#include "Inventory.h"
#include "Client.h"
#include "Data.h"

#ifdef VAULTMP_DEBUG
#include "Debug.h"
#endif

#include "RakNet/RakPeerInterface.h"

using namespace RakNet;
using namespace Data;
using namespace std;

class Player
{

private:
    static map<RakNetGUID, string> players;
    static map<RakNetGUID, Player*> playersguids;
    static map<string, Player*> playersrefs;

    static vector<string> GetRefs(string cmd, bool enabled, bool notself);

#ifdef VAULTMP_DEBUG
    static Debug* debug;
#endif

    RakNetGUID guid;
    string refID;
    Inventory* inventory;

    string name;
    float pos[3];
    float angle;
    DWORD gcell;
    DWORD ncell;
    float health;
    float baseHealth;
    float cond[6];
    bool dead;
    bool alerted;
    int moving;

    bool enabled;
    bool nowrite[MAX_SKIP_FLAGS];

public:
    Player(RakNetGUID guid);
    ~Player();

    static Parameter Param_EnabledPlayers;
    static Parameter Param_EnabledPlayers_NotSelf;
    static Parameter Param_AllPlayers;
    static Parameter Param_AllPlayers_NotSelf;

    static map<RakNetGUID, string> GetPlayerList();
    static Player* GetPlayerFromGUID(RakNetGUID guid);
    static Player* GetPlayerFromRefID(string refID);
    static vector<string> GetAllRefs(string cmd);
    static vector<string> GetAllRefs_NotSelf(string cmd);
    static vector<string> GetEnabledRefs(string cmd);
    static vector<string> GetEnabledRefs_NotSelf(string cmd);
    static void DestroyInstances();

#ifdef VAULTMP_DEBUG
    static void SetDebugHandler(Debug* debug);
#endif

    string GetPlayerName();
    float GetPlayerPos(int cell);
    float GetPlayerAngle();
    DWORD GetPlayerGameCell();
    DWORD GetPlayerNetworkCell();
    float GetPlayerHealth();
    float GetPlayerBaseHealth();
    float GetPlayerCondition(int cell);
    bool IsPlayerDead();
    bool IsPlayerAlerted();
    int GetPlayerMoving();
    string GetPlayerRefID();
    bool GetPlayerEnabled();
    Inventory* GetPlayerInventory();
    Parameter GetPlayerRefParam();
    Parameter GetPlayerNameParam();

    pPlayerUpdate GetPlayerUpdateStruct();
    pPlayerStateUpdate GetPlayerStateUpdateStruct();
    pPlayerCellUpdate GetPlayerCellUpdateStruct();
    pPlayerItemUpdate GetPlayerItemUpdateStruct(Item* item);
    bool UpdatePlayerUpdateStruct(pPlayerUpdate* data);
    bool UpdatePlayerStateUpdateStruct(pPlayerStateUpdate* data);
    bool UpdatePlayerCellUpdateStruct(pPlayerCellUpdate* data);
    bool UpdatePlayerItemUpdateStruct(list<pPlayerItemUpdate>* items, Inventory* inv);

    bool SetPlayerName(string name);
    bool SetPlayerPos(int cell, float pos);
    bool SetPlayerAngle(float angle);
    bool SetPlayerGameCell(DWORD cell);
    bool SetPlayerNetworkCell(DWORD cell);
    bool SetPlayerHealth(float health);
    bool SetPlayerBaseHealth(float baseHealth);
    bool SetPlayerCondition(int cell, float cond);
    bool SetPlayerDead(bool dead);
    bool SetPlayerAlerted(bool alerted);
    bool SetPlayerMoving(int moving);
    bool SetPlayerRefID(string refID);
    bool SetPlayerEnabled(bool enabled);
    void SetPlayerInventory(Inventory* inv);

    bool ToggleNoOverride(int skipflag, bool toggle);
    bool GetPlayerOverrideFlag(int skipflag);

    bool IsPlayerNearPoint(float X, float Y, float Z, float R);
    bool IsCoordinateInRange(int cell, float XYZ, float R);

};

#endif
