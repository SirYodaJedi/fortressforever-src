//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef FF_SHAREDDEFS_H
#define FF_SHAREDDEFS_H
#ifdef _WIN32
#pragma once
#endif

#ifdef CLIENT_DLL
	#include "materialsystem/imaterialsystem.h"
	#include "materialsystem/imesh.h"
	#include "clienteffectprecachesystem.h"
	#include "view_scene.h"
#endif

#define FF_PLAYER_VIEW_OFFSET	Vector( 0, 0, 53.5 )
#define EXTRA_LOCAL_ORIGIN_ACCURACY

#define DEFAULTWEAPON_SCOUT "shotgun"
#define DEFAULTWEAPON_SNIPER "sniperrifle"
#define DEFAULTWEAPON_SOLDIER "rpg"
#define DEFAULTWEAPON_DEMOMAN "grenadelauncher"
#define DEFAULTWEAPON_MEDIC "supernailgun"
#define DEFAULTWEAPON_HWGUY "assaultcannon"
#define DEFAULTWEAPON_PYRO "flamethrower"
#define DEFAULTWEAPON_ENGINEER "railgun"
#define DEFAULTWEAPON_SPY "knife"
#define DEFAULTWEAPON_CIVILIAN "umbrella"

// extended from shareddefs.h
#define DMG_AIRSHOT			(DMG_LASTGENERICFLAG<<1)		// damage done by an airshot

// Leave this commented out unless you're compiling for the beta testers!
// Mulch or Mirv should be the only ones messing with this!

//extern ConVar ffdev_spy_maxcloakspeed;
#define SPY_MAXCLOAKSPEED 220

#define MAX_WEAPON_SLOTS 6

enum FFPlayerGrenadeState
{
    FF_GREN_NONE,
    FF_GREN_PRIMEONE,
    FF_GREN_PRIMETWO
};

enum FFStatusIconTypes
{
    FF_STATUSICON_CONCUSSION,
    FF_STATUSICON_INFECTION,
    FF_STATUSICON_LEGINJURY,
	FF_STATUSICON_TRANQUILIZED,
	FF_STATUSICON_HALLUCINATIONS,
	FF_STATUSICON_BURNING,
	FF_STATUSICON_DROWNING,
	FF_STATUSICON_RADIATION,
	FF_STATUSICON_COLD,
	FF_STATUSICON_IMMUNE,
	FF_STATUSICON_RADIOTAG,
	FF_STATUSICON_BURNING1,
	FF_STATUSICON_BURNING2,
	FF_STATUSICON_BURNING3,
	FF_STATUSICON_LOCKEDON,
	FF_STATUSICON_SLIDING,
	FF_STATUSICON_SLOWMOTION,
	FF_STATUSICON_MAX
};

enum FF_View_Effects_t
{
	FF_VIEWEFFECT_BURNING,
	FF_VIEWEFFECT_TRANQUILIZED,
	FF_VIEWEFFECT_INFECTED,
	FF_VIEWEFFECT_GASSED,
	FF_VIEWEFFECT_MAX
};

// LUA Effect Flags
// Some of these just map to speed effect values
// but the mapping is through code and not actual
// integer values
enum LuaEffectType
{
	LUA_EF_ONFIRE = 0,		// put a player on fire
	LUA_EF_CONC,			// concuss a player
	LUA_EF_GAS,				// gas a player
	LUA_EF_INFECT,			// infect a player
	LUA_EF_RADIOTAG,		// radio tag a player
	LUA_EF_HEADSHOT,		// player is headshotted (currently only by sniperrifle)
	LUA_EF_LEGSHOT,			// left shot a player
	LUA_EF_TRANQ,			// tranq a player
	LUA_EF_ACSPINUP,		// ac spinning up
	LUA_EF_SNIPERRIFLE,		// player slowed due to sniper rifle charging
	LUA_EF_SPEED_LUA1,		// custom speed effect (etc.)
	LUA_EF_SPEED_LUA2,
	LUA_EF_SPEED_LUA3,
	LUA_EF_SPEED_LUA4,
	LUA_EF_SPEED_LUA5,
	LUA_EF_SPEED_LUA6,
	LUA_EF_SPEED_LUA7,
	LUA_EF_SPEED_LUA8,
	LUA_EF_SPEED_LUA9,
	LUA_EF_SPEED_LUA10,

	LUA_EF_MAX_FLAG
};

/////////////////////////////////////////////////////////////////////////////
// Purpose: Colors in lua
/////////////////////////////////////////////////////////////////////////////
enum LuaColors
{
	LUA_COLOR_DEFAULT = 0,
	LUA_COLOR_BLUE,
	LUA_COLOR_RED,
	LUA_COLOR_YELLOW,
	LUA_COLOR_GREEN,
	LUA_COLOR_WHITE,
	LUA_COLOR_BLACK,
	LUA_COLOR_ORANGE,
	LUA_COLOR_PINK,
	LUA_COLOR_PURPLE,
	LUA_COLOR_GREY,

	LUA_COLOR_INVALID,
};

struct SpyDisguiseWeapon
{
	char szWeaponClassName[MAX_WEAPON_STRING];
	char szWeaponModel[MAX_WEAPON_STRING];
	char szAnimExt[MAX_WEAPON_PREFIX];
};

// Decapitation flags
// These should be in the same order as the attachments + bodygroups in the model
#define DECAP_HEAD			( 1 << 0 )
#define DECAP_LEFT_ARM		( 1 << 1 )
#define DECAP_RIGHT_ARM		( 1 << 2 )
#define DECAP_LEFT_LEG		( 1 << 3 )
#define DECAP_RIGHT_LEG		( 1 << 4 )

// HUD Definitions
#define INTENSITYSCALE_COLOR_RED Color(255,0,0,255)
#define INTENSITYSCALE_COLOR_YELLOW Color(255,255,0,255)
#define INTENSITYSCALE_COLOR_ORANGE Color(255,128,0,255)
#define INTENSITYSCALE_COLOR_GREEN Color(0,255,0,255)
#define INTENSITYSCALE_COLOR_DEFAULT Color(255,255,255,255)

#define TEAM_COLOR_SPECTATOR Color( 0, 200, 200, 255 )
#define TEAM_COLOR_BLUE Color( 56, 100, 171, 255 )
#define TEAM_COLOR_RED Color( 188, 0, 0, 255 )
#define TEAM_COLOR_YELLOW Color( 202, 173, 33, 255 )
#define TEAM_COLOR_GREEN Color( 68, 144, 65, 255 )

// Some damage modifiers.
enum DamageTypes_t
{
	DAMAGETYPE_HEADSHOT = 1,
	DAMAGETYPE_BODYSHOUT,
	DAMAGETYPE_INFECTION,
	DAMAGETYPE_BURN_LEVEL1,
	DAMAGETYPE_BURN_LEVEL2,
	DAMAGETYPE_BURN_LEVEL3,
	DAMAGETYPE_GASSED,
	DAMAGETYPE_BACKSTAB,
	DAMAGETYPE_SENTRYGUN_DET,
	DAMAGETYPE_HEADCRUSH,
	DAMAGETYPE_RAILBOUNCE_1,
	DAMAGETYPE_RAILBOUNCE_2
};

//extern ConVar sniperrifle_chargetime;
#define FF_SNIPER_MAXCHARGE 5.0f //sniperrifle_chargetime.GetFloat()

#endif // FF_SHAREDDEFS_H
