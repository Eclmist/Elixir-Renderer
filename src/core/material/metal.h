/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019 Samuel Van Allen - All rights reserved.

    Elixir is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "material.h"
#include "core/bsdf/lambert.h"
#include "core/bsdf/reflection.h"
#include "core/bsdf/bsdf.h"

exrBEGIN_NAMESPACE

class Metal : public Material
{
public:
    Metal(const exrSpectrum& specular)
        : m_Specular(specular) {};

    void ComputeScatteringFunctions(SurfaceInteraction* si, MemoryArena& arena) const override
    {
        si->m_BSDF = EXR_ARENA_ALLOC(arena, BSDF)(*si);
        si->m_BSDF->AddComponent(EXR_ARENA_ALLOC(arena, Reflection)(m_Specular));
    }

private:
    exrSpectrum m_Specular;
};

exrEND_NAMESPACE