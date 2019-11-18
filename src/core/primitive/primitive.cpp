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

#include "primitive.h"

exrBEGIN_NAMESPACE

AABB Primitive::GetBoundingVolume() const
{
    return m_Shape->ComputeBoundingVolume();
}

exrBool Primitive::Intersect(const Ray& ray, SurfaceInteraction* interaction) const
{
    exrFloat tHit;

    if (!m_Shape->Intersect(ray, tHit, interaction)) return false;

    ray.m_TMax = tHit;
    interaction->m_Primitive = this;

    return true;
}

exrBool Primitive::HasIntersect(const Ray& r, exrFloat& tHit) const
{
    return m_Shape->HasIntersect(r, tHit);
}

exrBool Primitive::HasIntersect(const Ray& r) const
{
    exrFloat temp;
    return m_Shape->HasIntersect(r, temp);
}

void Primitive::ComputeScatteringFunctions(SurfaceInteraction* interaction) const
{
    if (m_Material)
        m_Material->ComputeScatteringFunctions(interaction);
}

const Material* Primitive::GetMaterial() const
{
    return m_Material;
}

exrEND_NAMESPACE
