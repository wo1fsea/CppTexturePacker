#pragma once
#ifndef _CPP_TEXTURE_PACKER_RECT_PACKER_
#define _CPP_TEXTURE_PACKER_RECT_PACKER_

#include <vector>
#include <tuple>
#include <algorithm>

#include "atlas.h"

namespace CppTexturePacker
{

    class TexturePacker
    {
    private:
        unsigned int bg_color;
        unsigned char trim_mode;
        bool reduce_border_artifacts;
        bool extrude;

    public:
        TexturePacker(
            unsigned int _bg_color = 0x00000000,
            unsigned char _trim_mode = 0,
            bool _reduce_border_artifacts = false,
            bool _extrude = false) :
            bg_color(_bg_color),
            trim_mode(_trim_mode),
            reduce_border_artifacts(_reduce_border_artifacts),
            extrude(_extrude),
        {
        }
    }
} // namespace CppTexturePacker
#endif