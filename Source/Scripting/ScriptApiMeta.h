//      __________        ___               ______            _
//     / ____/ __ \____  / (_)___  ___     / ____/___  ____ _(_)___  ___
//    / /_  / / / / __ \/ / / __ \/ _ \   / __/ / __ \/ __ `/ / __ \/ _ \
//   / __/ / /_/ / / / / / / / / /  __/  / /___/ / / / /_/ / / / / /  __/
//  /_/    \____/_/ /_/_/_/_/ /_/\___/  /_____/_/ /_/\__, /_/_/ /_/\___/
//                                                  /____/
// FOnline Engine
// https://fonline.ru
// https://github.com/cvet/fonline
//
// MIT License
//
// Copyright (c) 2006 - present, Anton Tsvetinskiy aka cvet <cvet@tut.by>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#define FO_API_ARG(type, name) type name
#define FO_API_ARG_ARR(type, name) arr:type name
#define FO_API_ARG_OBJ(type, name) type name
#define FO_API_ARG_OBJ_ARR(type, name) arr:type name
#define FO_API_ARG_REF(type, name) type:ref name
#define FO_API_ARG_ARR_REF(type, name) arr:type:ref name
#define FO_API_ARG_ENUM(type, name) enum:type name
#define FO_API_ARG_CALLBACK(name) callback name
#define FO_API_ARG_PREDICATE(type, name) predicate:type name
#define FO_API_ARG_DICT(key, val, name) dict:key:val name
#define FO_API_RET(type) type
#define FO_API_RET_ARR(type) arr:type
#define FO_API_RET_OBJ(type) type
#define FO_API_RET_OBJ_ARR(type) arr:type
#define FO_API_PROPERTY_TYPE(type) type
#define FO_API_PROPERTY_TYPE_ARR(type) arr:type
#define FO_API_PROPERTY_TYPE_OBJ(type) type
#define FO_API_PROPERTY_TYPE_OBJ_ARR(type) arr:type
#define FO_API_PROPERTY_TYPE_ENUM(type) enum:type
#define FO_API_PROPERTY_MOD(mod) mod

#define FO_API_ENUM_GROUP(name) enum group name
#define FO_API_ENUM_GROUP_DOC
#define FO_API_ENUM_ENTRY(group, name, value) enum entry group name value
#define FO_API_ENUM_ENTRY_DOC
#define FO_API_COMMON_EVENT(name, ...) event common name __VA_ARGS__
#define FO_API_COMMON_EVENT_DOC
#define FO_API_SERVER_EVENT(name, ...) event server name __VA_ARGS__
#define FO_API_SERVER_EVENT_DOC
#define FO_API_CLIENT_EVENT(name, ...) event client name __VA_ARGS__
#define FO_API_CLIENT_EVENT_DOC
#define FO_API_MAPPER_EVENT(name, ...) event mapper name __VA_ARGS__
#define FO_API_MAPPER_EVENT_DOC
#define FO_API_SETTING(type, name, ...) setting type name __VA_ARGS__
#define FO_API_SETTING_DOC
#define FO_API_GLOBAL_COMMON_FUNC(name, ret, ...) method global-common name ret __VA_ARGS__
#define FO_API_GLOBAL_COMMON_FUNC_DOC
#define FO_API_GLOBAL_SERVER_FUNC(name, ret, ...) method global-server name ret __VA_ARGS__
#define FO_API_GLOBAL_SERVER_FUNC_DOC
#define FO_API_GLOBAL_CLIENT_FUNC(name, ret, ...) method global-client name ret __VA_ARGS__
#define FO_API_GLOBAL_CLIENT_FUNC_DOC
#define FO_API_GLOBAL_MAPPER_FUNC(name, ret, ...) method global-mapper name ret __VA_ARGS__
#define FO_API_GLOBAL_MAPPER_FUNC_DOC
#define FO_API_GLOBAL_PROPERTY(access, type, name, ...) property global rw name access type __VA_ARGS__
#define FO_API_GLOBAL_READONLY_PROPERTY(access, type, name, ...) property global ro name access type __VA_ARGS__
#define FO_API_GLOBAL_PROPERTY_DOC
#define FO_API_ITEM_METHOD(name, ret, ...) method item name ret __VA_ARGS__
#define FO_API_ITEM_METHOD_DOC
#define FO_API_ITEM_VIEW_METHOD(name, ret, ...) method item-view name ret __VA_ARGS__
#define FO_API_ITEM_VIEW_METHOD_DOC
#define FO_API_ITEM_PROPERTY(access, type, name, ...) property item rw name access type __VA_ARGS__
#define FO_API_ITEM_READONLY_PROPERTY(access, type, name, ...) property item ro name access type __VA_ARGS__
#define FO_API_ITEM_PROPERTY_DOC
#define FO_API_CRITTER_METHOD(name, ret, ...) method critter name ret __VA_ARGS__
#define FO_API_CRITTER_METHOD_DOC
#define FO_API_CRITTER_VIEW_METHOD(name, ret, ...) method critter-view name ret __VA_ARGS__
#define FO_API_CRITTER_VIEW_METHOD_DOC
#define FO_API_CRITTER_PROPERTY(access, type, name, ...) property critter rw name access type __VA_ARGS__
#define FO_API_CRITTER_READONLY_PROPERTY(access, type, name, ...) property critter ro name access type __VA_ARGS__
#define FO_API_CRITTER_PROPERTY_DOC
#define FO_API_MAP_METHOD(name, ret, ...) method map name ret __VA_ARGS__
#define FO_API_MAP_METHOD_DOC
#define FO_API_MAP_VIEW_METHOD(name, ret, ...) method map-view name ret __VA_ARGS__
#define FO_API_MAP_VIEW_METHOD_DOC
#define FO_API_MAP_PROPERTY(access, type, name, ...) property map rw name access type __VA_ARGS__
#define FO_API_MAP_READONLY_PROPERTY(access, type, name, ...) property map ro name access type __VA_ARGS__
#define FO_API_MAP_PROPERTY_DOC
#define FO_API_LOCATION_METHOD(name, ret, ...) method location name ret __VA_ARGS__
#define FO_API_LOCATION_METHOD_DOC
#define FO_API_LOCATION_VIEW_METHOD(name, ret, ...) method location-view name ret __VA_ARGS__
#define FO_API_LOCATION_VIEW_METHOD_DOC
#define FO_API_LOCATION_PROPERTY(access, type, name, ...) property location rw name access type __VA_ARGS__
#define FO_API_LOCATION_READONLY_PROPERTY(access, type, name, ...) property location ro name access type __VA_ARGS__
#define FO_API_LOCATION_PROPERTY_DOC

#include "ScriptApi.h"