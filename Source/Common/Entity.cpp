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

#include "Entity.h"
#include "StringUtils.h"
#include "Testing.h"

Entity::Entity(uint id, EntityType type, PropertyRegistrator* registartor, ProtoEntity* proto) :
    Props {registartor}, Id {id}, Type {type}, Proto {proto}
{
    if (Proto)
    {
        Proto->AddRef();
        Props = Proto->Props;
    }
}

Entity::~Entity()
{
    if (Proto)
        Proto->Release();
}

uint Entity::GetId() const
{
    return Id;
}

void Entity::SetId(uint id)
{
    const_cast<uint&>(Id) = id;
}

hash Entity::GetProtoId() const
{
    return Proto ? Proto->ProtoId : 0;
}

string Entity::GetName() const
{
    switch (Type)
    {
    case EntityType::ItemProto:
    case EntityType::CritterProto:
    case EntityType::MapProto:
    case EntityType::LocationProto:
        return _str().parseHash(((ProtoEntity*)this)->ProtoId);
    default:
        break;
    }
    return Proto ? _str().parseHash(Proto->ProtoId) : "Unnamed";
}

void Entity::AddRef() const
{
    RefCounter++;
}

void Entity::Release() const
{
    if (--RefCounter == 0)
        delete this;
}

ProtoEntity::ProtoEntity(hash proto_id, EntityType type, PropertyRegistrator* registrator) :
    Entity(0, type, registrator, nullptr), ProtoId(proto_id)
{
    RUNTIME_ASSERT(ProtoId);
}

bool ProtoEntity::HaveComponent(hash name) const
{
    return Components.count(name) > 0;
}

CustomEntity::CustomEntity(uint id, uint sub_type, PropertyRegistrator* registrator) :
    Entity(id, EntityType::Custom, registrator, nullptr), SubType(sub_type)
{
}

PROPERTIES_IMPL(GlobalVars, "GlobalVars", true);
#define FO_API_GLOBAL_PROPERTY(access, type, name, ...) \
    CLASS_PROPERTY_IMPL(GlobalVars, access, type, name, __VA_ARGS__);
#include "ScriptApi.h"

GlobalVars::GlobalVars() : Entity(0, EntityType::Global, PropertiesRegistrator, nullptr)
{
}

PROPERTIES_IMPL(ProtoItem, "Item", true);
#define FO_API_ITEM_PROPERTY(access, type, name, ...) CLASS_PROPERTY_IMPL(ProtoItem, access, type, name, __VA_ARGS__);
#include "ScriptApi.h"

ProtoItem::ProtoItem(hash pid) : ProtoEntity(pid, EntityType::ItemProto, PropertiesRegistrator)
{
}

PROPERTIES_IMPL(ProtoCritter, "Critter", true);
#define FO_API_CRITTER_PROPERTY(access, type, name, ...) \
    CLASS_PROPERTY_IMPL(ProtoCritter, access, type, name, __VA_ARGS__);
#include "ScriptApi.h"

ProtoCritter::ProtoCritter(hash pid) : ProtoEntity(pid, EntityType::CritterProto, PropertiesRegistrator)
{
}

PROPERTIES_IMPL(ProtoMap, "Map", true);
#define FO_API_MAP_PROPERTY(access, type, name, ...) CLASS_PROPERTY_IMPL(ProtoMap, access, type, name, __VA_ARGS__);
#include "ScriptApi.h"

ProtoMap::ProtoMap(hash pid) : ProtoEntity(pid, EntityType::MapProto, PropertiesRegistrator)
{
}

PROPERTIES_IMPL(ProtoLocation, "Location", true);
#define FO_API_LOCATION_PROPERTY(access, type, name, ...) \
    CLASS_PROPERTY_IMPL(ProtoLocation, access, type, name, __VA_ARGS__);
#include "ScriptApi.h"

ProtoLocation::ProtoLocation(hash pid) : ProtoEntity(pid, EntityType::LocationProto, PropertiesRegistrator)
{
}
