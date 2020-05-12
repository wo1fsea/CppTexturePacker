#pragma once
#ifndef _CPP_TEXTURE_PACKER_IMAGE_INFO_
#define _CPP_TEXTURE_PACKER_IMAGE_INFO_

#include <string>

#include "utils.h"
#include "image_rect.h"


namespace CppTexturePacker
{

class ImageInfo 
{
public:
    Image image ;
    std::string image_path;

    Rect<int> source_rect;
    Rect<int> source_bbox;

    bool trimmed;
    size_t extruded;

    int ex_key;

public:
    ImageInfo() : source_rect({0, 0, 0, 0}),
                  source_bbox({0, 0, 0, 0}),
                  trimmed(false),
                  extruded(0),
                  ex_key(0)
    {
    }

    bool is_trimmed()
    {
        return trimmed;
    }

    ImageRect get_image_rect()
    {
        ImageRect image_rect;
        
        return image_rect;
    }

};

}; // namespace CppTexturePacker

#endif