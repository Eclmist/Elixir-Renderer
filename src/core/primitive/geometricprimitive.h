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

class AABB;

class GeometricPrimitive : public Primitive
{
public:
    AABB GetBoundingVolume() const override;
    exrBool Intersect(const Ray& ray, SurfaceInteraction* interaction) const override;
    exrBool HasIntersect(const Ray& r, exrFloat& tHit) const override;
    void ComputeScatteringFunctions(SurfaceInteraction* interaction) const override;
public:
    AreaLight* GetAreaLight() const override;
    const Material* GetMaterial() const override;

public:
    std::unique_ptr<Shape> m_Shape;
    std::unique_ptr<Material> m_Material;
    std::unique_ptr<AreaLight> m_AreaLight;
};

exrEND_NAMESPACE