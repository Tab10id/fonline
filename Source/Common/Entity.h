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

#pragma once

#include "Common.h"

#include "MsgFiles.h"
#include "Properties.h"

#define PROPERTIES_HEADER() static PropertyRegistrator* PropertiesRegistrator

#define PROPERTIES_IMPL(class_name, script_name, is_server) \
    PropertyRegistrator* class_name::PropertiesRegistrator = new PropertyRegistrator(is_server)

#define CLASS_PROPERTY(access_type, prop_type, prop, ...) \
    static Property* Property##prop; \
    inline prop_type Get##prop() { return Props.GetValue<prop_type>(Property##prop); } \
    inline void Set##prop(prop_type value) { Props.SetValue<prop_type>(Property##prop, value); } \
    inline bool IsNonEmpty##prop() { return Props.GetRawDataSize(Property##prop) > 0; }

#define CLASS_PROPERTY_IMPL(class_name, access_type, prop_type, prop, ...) \
    Property* class_name::Property##prop = \
        class_name::PropertiesRegistrator->Register(Property::AccessType::access_type, typeid(prop_type), #prop)

enum class EntityType
{
    ItemProto,
    CritterProto,
    MapProto,
    LocationProto,
    Item,
    Client,
    Npc,
    Map,
    Location,
    ItemView,
    ItemHexView,
    CritterView,
    MapView,
    LocationView,
    Custom,
    Global,
    Max,
};

class Entity;
using EntityVec = vector<Entity*>;
using EntityMap = map<uint, Entity*>;
class ProtoEntity;
using ProtoEntityVec = vector<ProtoEntity*>;
using ProtoEntityMap = map<hash, ProtoEntity*>;

class Entity : public NonCopyable
{
public:
    uint GetId() const;
    void SetId(uint id);
    hash GetProtoId() const;
    string GetName() const;
    void AddRef() const;
    void Release() const;

    Properties Props;
    const uint Id;
    const EntityType Type;
    const ProtoEntity* Proto;
    mutable int RefCounter {1};
    bool IsDestroyed {};
    bool IsDestroying {};

protected:
    Entity(uint id, EntityType type, PropertyRegistrator* registartor, ProtoEntity* proto);
    virtual ~Entity();
};

class ProtoEntity : public Entity
{
public:
    bool HaveComponent(hash name) const;

    const hash ProtoId;
    UIntVec TextsLang {};
    vector<FOMsg*> Texts {};
    HashSet Components {};
    string CollectionName {};

protected:
    ProtoEntity(hash proto_id, EntityType type, PropertyRegistrator* registrator);
};

class CustomEntity : public Entity
{
public:
    CustomEntity(uint id, uint sub_type, PropertyRegistrator* registrator);

    const uint SubType;
};

class GlobalVars : public Entity
{
public:
    GlobalVars();

    PROPERTIES_HEADER();
#define FO_API_GLOBAL_PROPERTY CLASS_PROPERTY
#include "ScriptApi.h"
};

class ProtoItem : public ProtoEntity
{
public:
    ProtoItem(hash pid);
    bool IsStatic() { return GetIsStatic(); }
    bool IsAnyScenery() { return IsScenery() || IsWall(); }
    bool IsScenery() { return GetIsScenery(); }
    bool IsWall() { return GetIsWall(); }

    int64 InstanceCount {};

    PROPERTIES_HEADER();
#define FO_API_ITEM_PROPERTY CLASS_PROPERTY
#include "ScriptApi.h"
};
using ProtoItemVec = vector<ProtoItem*>;
using ProtoItemMap = map<hash, ProtoItem*>;

class ProtoCritter : public ProtoEntity
{
public:
    ProtoCritter(hash pid);

    PROPERTIES_HEADER();
#define FO_API_CRITTER_PROPERTY CLASS_PROPERTY
#include "ScriptApi.h"
};
using ProtoCritterMap = map<hash, ProtoCritter*>;
using ProtoCritterVec = vector<ProtoCritter*>;

class ProtoMap : public ProtoEntity
{
public:
    ProtoMap(hash pid);

    PROPERTIES_HEADER();
#define FO_API_MAP_PROPERTY CLASS_PROPERTY
#include "ScriptApi.h"
};
using ProtoMapVec = vector<ProtoMap*>;
using ProtoMapMap = map<hash, ProtoMap*>;

class ProtoLocation : public ProtoEntity
{
public:
    ProtoLocation(hash pid);

    PROPERTIES_HEADER();
#define FO_API_LOCATION_PROPERTY CLASS_PROPERTY
#include "ScriptApi.h"
};
using ProtoLocationVec = vector<ProtoLocation*>;
using ProtoLocationMap = map<hash, ProtoLocation*>;
