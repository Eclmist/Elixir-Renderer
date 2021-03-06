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

#include "core/elixir.h"
#include "core/exporter/exporter.h"

exrBEGIN_NAMESPACE

static constexpr exrU32 WIDTH = 500;
static constexpr exrU32 HEIGHT = 500;

//! @brief A simple look-at camera that handles view rays generatino
//!
//! The camera class can be positioned using an origin and a look at point. FOV, aperture, focus distance
//! are some of the settings that can be adjusted such that the generated ray can be used in effects
//! such as motion blur or defocus blur.
class Camera
{
public:
    //! @brief Creates a camera object
    //!
    //! @param position          The position of the camera
    //! @param lookat            The look at point of the camera
    //! @param up                The camera's up vector, used to adjust roll
    //! @param vfov              The field of view in degrees
    //! @param aspect            The aspect ratio of the expected output
    //! @param aperture          The aperture of the camera
    //! @param focusDist         The distance away from the camera's focus plane
    Camera(exrPoint3 position, exrPoint3 lookat, exrVector3 up, exrFloat vfov, exrFloat aperture, exrFloat focusDist);

    //! @brief Creates a view ray based from a uv coordinate
    //!
    //! @param s                The u coordinate of the ray in screen space
    //! @param t                The v coordinate of the ray in screen space
    Ray GetViewRay(exrFloat s, exrFloat t);

public:
    std::unique_ptr<Exporter> m_Exporter;

private:
    exrPoint3 m_Position;
    exrPoint3 m_Min;
    exrVector3 m_HorizontalStep;
    exrVector3 m_VerticalStep;

    exrVector3 u, v, w;
    exrFloat lensRadius;
};

exrEND_NAMESPACE