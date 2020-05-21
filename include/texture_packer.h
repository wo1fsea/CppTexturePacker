#pragma once
#ifndef _CPP_TEXTURE_PACKER_TEXTURE_PACKER_
#define _CPP_TEXTURE_PACKER_TEXTURE_PACKER_

#include <vector>
#include <tuple>
#include <algorithm>
#include <string>

#include <boost/format.hpp>

#include "atlas.h" 
#include "utils.h" 
#include "rect_packer.h" 

namespace CppTexturePacker
{

    class TexturePacker: public RectPacker
    {
    private:
        unsigned int bg_color;
        unsigned char trim_mode;
        bool reduce_border_artifacts;
        bool extrude;

    public:
        TexturePacker(
            unsigned int _max_width = 4096,
            unsigned int _max_height = 4096,
            bool _enable_rotated = false,
            bool _force_square = false,
            unsigned int _border_padding = 0,
            unsigned int _shape_padding = 0,
            unsigned int _inner_padding = 0,

            unsigned int _bg_color = 0x00000000,
            unsigned char _trim_mode = 0,
            bool _reduce_border_artifacts = false,
            bool _extrude = false) :
            RectPacker(
                _max_width,
                _max_height,
               _enable_rotated,
                _force_square,
               _border_padding,
               _shape_padding,
               _inner_padding
            ),
            bg_color(_bg_color),
            trim_mode(_trim_mode),
            reduce_border_artifacts(_reduce_border_artifacts),
            extrude(_extrude)
        {
        }
        
        void pack(const std::vector<std::string>& image_paths,
            const std::string & output_name,
            const std::string & output_path="",
            const std::string & input_base_path="")
        {

            auto image_infos = load_image_infos_from_paths(image_paths);

            if (trim_mode)
            {
                for (auto image_info: image_infos)
                {
                    image_info.trim(trim_mode);
                }
            }

            if (extrude)
            {
                
            }

            std::vector<CppTexturePacker::ImageRect> image_rects;
            for (auto image_info : image_infos)
            {
                image_rects.emplace_back(image_info.get_image_rect());
            }
            
            add_image_rects(image_rects);
            auto image_info_map = make_image_info_map(image_infos);

            for (auto atlas: atlases)
            {
                auto image = dump_altalas_to_image(atlas, image_info_map);
                
                // save_image_to_file(save_path, image);

            }



            if (reduce_border_artifacts)
            {

            }
        }
    };
} // namespace CppTexturePacker
#endif