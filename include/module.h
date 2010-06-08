/*

    Gm_sqlite3 the improved sqlite library for Garry's Mod
    Copyright (C) 2010 James John Kelly Jr

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef _INCLUDE_MODULE_H_
#define _INCLUDE_MODULE_H_

#if _WIN32
#include <windows.h>
#endif

#include <GMLuaModule.h>

// Some versions of the GMod interface don't have SAFE_UNERF

#ifndef SAFE_UNREF
# define SAFE_UNREF( _obj_ ) if ( _obj_ ){ _obj_->UnReference(); _obj_ = NULL; } 
#endif

// I want to be able to declare lua functions in other files to keep my code clean and organized
// but unfortunately the default LUA_FUNCTION is static, so I have to redeclare it

#ifdef LUA_FUNCTION
# undef LUA_FUNCTION
#endif

#define LUA_FUNC( _function_ ) LuaFunc_##_function_
#define LUA_FUNCTION( _function_ ) int LuaFunc_##_function_( lua_State* L )
#define LUA_PROTOTYPE( _function_ ) int LuaFunc_##_function_( lua_State* L )

// Used to output error messages on unexpected conditions

#ifdef DEBUG
# define ASSERT( cnd ) if( cnd ) {} else { Msg("Assertion failed (" #cnd ") in file %s on line %d\n", __FILE__, __LINE__); }
#else
# define ASSERT( cnd )
#endif

// Verious global definitions

#define GLOBAL_TABLE	"sqlite3"

#define META_DATABASE	"sqlite3db"
#define META_STATEMENT	"sqlite3stmt"

enum MetaTypes {
	TYPE_DATABASE = 56173,
	TYPE_STATEMENT
};

// The almighty Lua interface

extern ILuaInterface* g_pLua;

#endif
