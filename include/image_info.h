#pragma once
#ifndef _CPP_TEXTURE_PACKER_IMAGE_INFO_
#define _CPP_TEXTURE_PACKER_IMAGE_INFO_

#include <string>

#include "image_rect.h"
#include "utils.h"

static unsigned int _current_ex_key = 0;

namespace CppTexturePacker
{
using Image = cimg_library::CImg<unsigned char>;

void clean_pixel_alpha_below(Image &image, unsigned char alpha);
Rect<int> get_bounding_box(const Image &image);
void trim_image(Image &image, const Rect<int> &rect);
Image enlarge_image_border(const Image &image, unsigned char pixel_num, bool repeat_border);

class ImageInfo
{
private:
    Image image;
    std::string image_path;

    Rect<int> source_rect;
    Rect<int> source_bbox;

    bool trimmed;
    unsigned char extruded;
    unsigned char inner_padding;

    unsigned int ex_key;

public:
    ImageInfo() : image_path(""),
                  source_rect({0, 0, image.width(), image.height()}),
                  source_bbox({0, 0, image.width(), image.height()}),
                  trimmed(false),
                  extruded(0),
                  inner_padding(0),
                  ex_key(_current_ex_key++)
    {
    }

    ImageInfo(Image _image, std::string _image_path = "") : image(_image),
                                                            image_path(_image_path),
                                                            source_rect({0, 0, image.width(), image.height()}),
                                                            source_bbox({0, 0, image.width(), image.height()}),
                                                            trimmed(false),
                                                            extruded(0),
                                                            inner_padding(0),
                                                            ex_key(_current_ex_key++)
    {
    }

    ImageInfo(const ImageInfo &image_info) : image(image_info.image),
                                             image_path(image_info.image_path),
                                             source_rect(image_info.source_rect),
                                             source_bbox(image_info.source_bbox),
                                             trimmed(image_info.trimmed),
                                             extruded(image_info.extruded),
                                             inner_padding(image_info.inner_padding),
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

    void set_inner_padding(unsigned char _inner_padding)
    {
        if (inner_padding)
        {
            return;
        }

        inner_padding = _inner_padding;
        image = enlarge_image_border(image, inner_padding, false);

        source_rect.x += inner_padding;
        source_rect.y += inner_padding;

        source_bbox.x += inner_padding;
        source_bbox.y += inner_padding;
    }

    unsigned char get_inner_padding()
    {
        return inner_padding;
    }

    void extrude(unsigned char size)
    {
        if (extruded)
        {
            return;
        }

        extruded = size;
        image = enlarge_image_border(image, extruded, true);

        source_rect.x += extruded;
        source_rect.y += extruded;

        source_bbox.x += extruded;
        source_bbox.y += extruded;
    }

    unsigned char get_extruded()
    {
        return extruded;
    }

    ImageRect get_image_rect() const
    {
        ImageRect image_rect;
        image_rect.x = 0;
        image_rect.y = 0;
        image_rect.width = image.width();
        image_rect.height = image.height();
        image_rect.ex_key = ex_key;
        return image_rect;
    }

    const Image &get_image() const
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