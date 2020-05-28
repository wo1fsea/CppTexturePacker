#pragma once
#ifndef _CPP_TEXTURE_PACKER_IMAGE_INFO_
#define _CPP_TEXTURE_PACKER_IMAGE_INFO_

#include <string>

#include "image_rect.h"
#include "utils.h"

static int _current_ex_key = 0;

namespace CppTexturePacker
{
using Image = cimg_library::CImg<unsigned char>;

void clean_pixel_alpha_below(Image& image, unsigned char alpha);
Rect<int> get_bounding_box(const Image& image);
void trim_image(Image& image, const Rect<int>& rect);

class ImageInfo
{
private:
    Image image;
    std::string image_path;

    Rect<int> source_rect;
    Rect<int> source_bbox;

    bool trimmed;
    size_t extruded;

    int ex_key;

public:
    ImageInfo() :
        image_path(""),
        source_rect({ 0, 0, image.width(), image.height() }),
        source_bbox({ 0, 0, image.width(), image.height() }),
        trimmed(false),
        extruded(0),
        ex_key(_current_ex_key++)
    {
    }

    ImageInfo(Image _image, std::string _image_path = "") :
        image(_image),
        image_path(_image_path),
        source_rect({ 0, 0, image.width(), image.height() }),
        source_bbox({ 0, 0, image.width(), image.height() }),
        trimmed(false),
        extruded(0),
        ex_key(_current_ex_key++)
    {
    }

    ImageInfo(const ImageInfo& image_info) : 
                  image(image_info.image),
                  image_path(image_info.image_path),
                  source_rect(image_info.source_rect),
                  source_bbox(image_info.source_bbox),
                  trimmed(image_info.trimmed),
                  extruded(image_info.extruded),
                  ex_key(image_info.ex_key)
    {
    }

    void trim(unsigned char alpha_threshold)
    {
        trimmed = true;
        clean_pixel_alpha_below(image, alpha_threshold);
        source_bbox = get_bounding_box(image);
        trim_image(image, source_bbox);
    }

    bool is_trimmed() const
    {
        return trimmed;
    }

    ImageRect get_image_rect() const
    {
        ImageRect image_rect;
        image_rect.x = source_bbox.x;
        image_rect.y = source_bbox.y;
        image_rect.width = source_bbox.width;
        image_rect.height = source_bbox.height;
        image_rect.ex_key = ex_key;
        return image_rect;
    }

    const Image& get_image() const
    {
        return image;
    }
    
    int get_ex_key() const
    {
        return ex_key;
    }

    Rect<int> get_source_rect() const
    {
        return source_rect;
    }

    Rect<int> get_source_bbox() const
    {
        return source_bbox;
    }

    std::string get_image_path() const
    {
        return image_path;
    }
};

}; // namespace CppTexturePacker

#endif