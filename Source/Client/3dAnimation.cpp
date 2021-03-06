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

#include "3dAnimation.h"

void AnimSet::Load(DataReader& reader)
{
    uint len = 0;
    reader.ReadPtr(&len, sizeof(len));
    animFileName.resize(len);
    reader.ReadPtr(&animFileName[0], len);
    reader.ReadPtr(&len, sizeof(len));
    animName.resize(len);
    reader.ReadPtr(&animName[0], len);
    reader.ReadPtr(&durationTicks, sizeof(durationTicks));
    reader.ReadPtr(&ticksPerSecond, sizeof(ticksPerSecond));
    reader.ReadPtr(&len, sizeof(len));
    bonesHierarchy.resize(len);
    for (uint i = 0, j = len; i < j; i++)
    {
        reader.ReadPtr(&len, sizeof(len));
        bonesHierarchy[i].resize(len);
        reader.ReadPtr(&bonesHierarchy[i][0], len * sizeof(bonesHierarchy[0][0]));
    }
    reader.ReadPtr(&len, sizeof(len));
    boneOutputs.resize(len);
    for (uint i = 0, j = len; i < j; i++)
    {
        AnimSet::BoneOutput& o = boneOutputs[i];
        reader.ReadPtr(&o.nameHash, sizeof(o.nameHash));
        reader.ReadPtr(&len, sizeof(len));
        o.scaleTime.resize(len);
        o.scaleValue.resize(len);
        reader.ReadPtr(&o.scaleTime[0], len * sizeof(o.scaleTime[0]));
        reader.ReadPtr(&o.scaleValue[0], len * sizeof(o.scaleValue[0]));
        reader.ReadPtr(&len, sizeof(len));
        o.rotationTime.resize(len);
        o.rotationValue.resize(len);
        reader.ReadPtr(&o.rotationTime[0], len * sizeof(o.rotationTime[0]));
        reader.ReadPtr(&o.rotationValue[0], len * sizeof(o.rotationValue[0]));
        reader.ReadPtr(&len, sizeof(len));
        o.translationTime.resize(len);
        o.translationValue.resize(len);
        reader.ReadPtr(&o.translationTime[0], len * sizeof(o.translationTime[0]));
        reader.ReadPtr(&o.translationValue[0], len * sizeof(o.translationValue[0]));
    }
}

void AnimSet::SetData(const string& fname, const string& name, float ticks, float tps)
{
    animFileName = fname;
    animName = name;
    durationTicks = ticks;
    ticksPerSecond = tps;
}

void AnimSet::AddBoneOutput(HashVec hierarchy, const FloatVec& st, const VectorVec& sv, const FloatVec& rt,
    const QuaternionVec& rv, const FloatVec& tt, const VectorVec& tv)
{
    boneOutputs.push_back(BoneOutput());
    BoneOutput& o = boneOutputs.back();
    o.nameHash = hierarchy.back();
    o.scaleTime = st;
    o.scaleValue = sv;
    o.rotationTime = rt;
    o.rotationValue = rv;
    o.translationTime = tt;
    o.translationValue = tv;
    bonesHierarchy.push_back(hierarchy);
}

const string& AnimSet::GetFileName()
{
    return animFileName;
}

const string& AnimSet::GetName()
{
    return animName;
}

uint AnimSet::GetBoneOutputCount()
{
    return (uint)boneOutputs.size();
}

float AnimSet::GetDuration()
{
    return durationTicks / ticksPerSecond;
}

HashVecVec& AnimSet::GetBonesHierarchy()
{
    return bonesHierarchy;
}

AnimController::AnimController(uint track_count)
{
    if (track_count)
    {
        sets = new vector<AnimSet*>();
        outputs = new vector<Output>();
        tracks.resize(track_count);
    }
}

AnimController::~AnimController()
{
    if (!cloned)
    {
        delete sets;
        delete outputs;
    }
}

AnimController* AnimController::Clone()
{
    AnimController* clone = new AnimController(0);
    clone->cloned = true;
    clone->sets = sets;
    clone->outputs = outputs;
    clone->tracks = tracks;
    clone->curTime = 0.0f;
    clone->interpolationDisabled = interpolationDisabled;
    return clone;
}

void AnimController::RegisterAnimationOutput(hash bone_name_hash, Matrix& output_matrix)
{
    outputs->push_back(Output());
    Output& o = outputs->back();
    o.nameHash = bone_name_hash;
    o.matrix = &output_matrix;
    o.valid.resize(tracks.size());
    o.factor.resize(tracks.size());
    o.scale.resize(tracks.size());
    o.rotation.resize(tracks.size());
    o.translation.resize(tracks.size());
}

void AnimController::RegisterAnimationSet(AnimSet* animation)
{
    sets->push_back(animation);
}

AnimSet* AnimController::GetAnimationSet(uint index)
{
    if (index >= sets->size())
        return nullptr;
    return (*sets)[index];
}

AnimSet* AnimController::GetAnimationSetByName(const string& name)
{
    for (auto it = sets->begin(), end = sets->end(); it != end; ++it)
    {
        if ((*it)->animName == name)
            return *it;
    }
    return nullptr;
}

float AnimController::GetTrackPosition(uint track)
{
    return tracks[track].position;
}

uint AnimController::GetNumAnimationSets()
{
    return (uint)sets->size();
}

void AnimController::SetTrackAnimationSet(uint track, AnimSet* anim)
{
    tracks[track].anim = anim;
    uint count = anim->GetBoneOutputCount();
    tracks[track].animOutput.resize(count);
    for (uint i = 0; i < count; i++)
    {
        hash link_name_hash = anim->boneOutputs[i].nameHash;
        Output* output = nullptr;
        for (uint j = 0; j < (uint)outputs->size(); j++)
        {
            if ((*outputs)[j].nameHash == link_name_hash)
            {
                output = &(*outputs)[j];
                break;
            }
        }
        tracks[track].animOutput[i] = output;
    }
}

void AnimController::ResetBonesTransition(uint skip_track, const HashVec& bone_name_hashes)
{
    // Turn off fast transition bones on other tracks
    for (size_t b = 0; b < bone_name_hashes.size(); b++)
    {
        hash bone_name_hash = bone_name_hashes[b];
        for (uint i = 0, j = (uint)tracks.size(); i < j; i++)
        {
            if (i == skip_track)
                continue;

            for (uint k = 0, l = (uint)tracks[i].animOutput.size(); k < l; k++)
            {
                if (tracks[i].animOutput[k] && tracks[i].animOutput[k]->nameHash == bone_name_hash)
                {
                    tracks[i].animOutput[k]->valid[i] = false;
                    tracks[i].animOutput[k] = nullptr;
                }
            }
        }
    }
}

void AnimController::Reset()
{
    curTime = 0.0f;
    for (uint i = 0; i < (uint)tracks.size(); i++)
        tracks[i].events.clear();
}

float AnimController::GetTime()
{
    return curTime;
}

void AnimController::AddEventEnable(uint track, bool enable, float start_time)
{
    tracks[track].events.push_back({Track::Event::Enable, enable ? 1.0f : -1.0f, start_time, 0.0f});
}

void AnimController::AddEventSpeed(uint track, float speed, float start_time, float smooth_time)
{
    tracks[track].events.push_back({Track::Event::Speed, speed, start_time, smooth_time});
}

void AnimController::AddEventWeight(uint track, float weight, float start_time, float smooth_time)
{
    tracks[track].events.push_back({Track::Event::Weight, weight, start_time, smooth_time});
}

void AnimController::SetTrackEnable(uint track, bool enable)
{
    tracks[track].enabled = enable;
}

void AnimController::SetTrackPosition(uint track, float position)
{
    tracks[track].position = position;
}

void AnimController::SetInterpolation(bool enabled)
{
    interpolationDisabled = !enabled;
}

void AnimController::AdvanceTime(float time)
{
    // Animation time
    curTime += time;

    // Track events
    for (uint i = 0, j = (uint)tracks.size(); i < j; i++)
    {
        Track& track = tracks[i];

        // Events
        for (auto it = track.events.begin(); it != track.events.end();)
        {
            Track::Event& e = *it;
            if (curTime >= e.startTime)
            {
                if (e.smoothTime > 0.0f && e.valueFrom == -1.0f)
                {
                    if (e.type == Track::Event::Speed)
                        e.valueFrom = track.speed;
                    else if (e.type == Track::Event::Weight)
                        e.valueFrom = track.weight;
                }

                bool erase = false;
                float value = e.valueTo;
                if (curTime < e.startTime + e.smoothTime)
                {
                    if (e.valueTo > e.valueFrom)
                        value = e.valueFrom + (e.valueTo - e.valueFrom) / e.smoothTime * (curTime - e.startTime);
                    else
                        value = e.valueFrom - (e.valueFrom - e.valueTo) / e.smoothTime * (curTime - e.startTime);
                }
                else
                {
                    erase = true;
                }

                if (e.type == Track::Event::Enable)
                    track.enabled = (value > 0.0f ? true : false);
                else if (e.type == Track::Event::Speed)
                    track.speed = value;
                else if (e.type == Track::Event::Weight)
                    track.weight = value;

                if (erase)
                {
                    it = track.events.erase(it);
                    continue;
                }
            }

            ++it;
        }

        // Add track time
        if (track.enabled)
            track.position += time * track.speed;
    }

    // Track animation
    for (uint i = 0, j = (uint)tracks.size(); i < j; i++)
    {
        Track& track = tracks[i];

        for (uint k = 0, l = (uint)outputs->size(); k < l; k++)
            (*outputs)[k].valid[i] = false;

        if (!track.enabled || track.weight <= 0.0f || !track.anim)
            continue;

        for (uint k = 0, l = (uint)track.anim->boneOutputs.size(); k < l; k++)
        {
            if (!track.animOutput[k])
                continue;

            AnimSet::BoneOutput& o = track.anim->boneOutputs[k];

            float time = fmod(track.position * track.anim->ticksPerSecond, track.anim->durationTicks);
            FindSRTValue<Vector>(time, o.scaleTime, o.scaleValue, track.animOutput[k]->scale[i]);
            FindSRTValue<Quaternion>(time, o.rotationTime, o.rotationValue, track.animOutput[k]->rotation[i]);
            FindSRTValue<Vector>(time, o.translationTime, o.translationValue, track.animOutput[k]->translation[i]);
            track.animOutput[k]->valid[i] = true;
            track.animOutput[k]->factor[i] = track.weight;
        }
    }

    // Blend tracks
    for (uint i = 0, j = (uint)outputs->size(); i < j; i++)
    {
        Output& o = (*outputs)[i];

        // Todo: add interpolation for tracks more than two
        if (tracks.size() >= 2 && o.valid[0] && o.valid[1])
        {
            float factor = o.factor[1];
            Interpolate(o.scale[0], o.scale[1], factor);
            Interpolate(o.rotation[0], o.rotation[1], factor);
            Interpolate(o.translation[0], o.translation[1], factor);
            Matrix ms, mr, mt;
            Matrix::Scaling(o.scale[0], ms);
            mr = Matrix(o.rotation[0].GetMatrix());
            Matrix::Translation(o.translation[0], mt);
            *o.matrix = mt * mr * ms;
        }
        else
        {
            for (uint k = 0, l = (uint)tracks.size(); k < l; k++)
            {
                if (o.valid[k])
                {
                    Matrix ms, mr, mt;
                    Matrix::Scaling(o.scale[k], ms);
                    mr = Matrix(o.rotation[k].GetMatrix());
                    Matrix::Translation(o.translation[k], mt);
                    *o.matrix = mt * mr * ms;
                    break;
                }
            }
        }
    }
}

void AnimController::Interpolate(Quaternion& q1, const Quaternion& q2, float factor)
{
    if (!interpolationDisabled)
        Quaternion::Interpolate(q1, q1, q2, factor);
    else if (factor >= 0.5f)
        q1 = q2;
}

void AnimController::Interpolate(Vector& v1, const Vector& v2, float factor)
{
    if (!interpolationDisabled)
    {
        v1.x = v1.x + (v2.x - v1.x) * factor;
        v1.y = v1.y + (v2.y - v1.y) * factor;
        v1.z = v1.z + (v2.z - v1.z) * factor;
    }
    else if (factor >= 0.5f)
    {
        v1 = v2;
    }
}
