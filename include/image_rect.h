#pragma once
#ifndef _CPP_TEXTURE_PACKER_IMAGE_RECT_
#define _CPP_TEXTURE_PACKER_IMAGE_RECT_

#include "rect.h"

namespace CppTexturePacker
{

class ImageRect:  public Rect<int>
{
private:
    bool rotated;

public:
    int ex_key;

public:
    ImageRect() : Rect({0, 0, 0, 0}),
                  rotated(false),
                  ex_key(0)
    {
    }

    void rotate()
    {
        rotated = !rotated;
        Rect::rotate();
    }

    bool is_rotated()
    {
        return rotated;
    }
};

}; // namespace CppTexturePacker

#endif