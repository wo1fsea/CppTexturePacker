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
        bool reduce_border_artifacts;
        unsigned char trim_mode;
        unsigned char extrude;

    public:
        TexturePacker(
            unsigned int _max_width = 4096,
            unsigned int _max_height = 4096,
            bool _enable_rotated = false,
            bool _force_square = false,
            int _border_padding = 0,
            int _shape_padding = 0,
            int _inner_padding = 0,

            bool _reduce_border_artifacts = false,
            unsigned char _trim_mode = 0,
            unsigned int _extrude = 0) :
            RectPacker(
                _max_width,
                _max_height,
               _enable_rotated,
                _force_square,
               _border_padding,
               _shape_padding,
               _inner_padding
            ),
            trim_mode(_trim_mode),
            reduce_border_artifacts(_reduce_border_artifacts),
            extrude(_extrude)
        {
        }
        
        void pack(const std::vector<ImageInfo>& image_infos,
            const std::string & output_name,
            const std::string & output_path,
            const std::string & base_image_path="",
            const std::string & image_format="png")
        {
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
            
            assert((atlases.size() <=1 || output_name.find("%d") != std::string::npos));

            fs::path fp_out = output_path;

            for (int i=0; i < atlases.size(); ++i)
            {
                auto atlas = atlases[i];
                std::string atlas_name = output_name;
                if (output_name.find("%d") != std::string::npos)
                {
                    atlas_name = (boost::format(output_name) % i).str();
                }
                std::string image_file_name = atlas_name + "." + image_format;
                fs::path image_path = fp_out / image_file_name;
                fs::path plist_path = fp_out / (atlas_name + ".plist");
                auto image = dump_altalas_to_image(atlas, image_info_map);
                if (reduce_border_artifacts)
                {
                    alpha_bleeding(image);
                }
                save_image_to_file(image_path.string(), image);
                dump_altalas_to_plist(plist_path.string(), atlas, image_info_map, image_file_name, base_image_path);
            }
        }

        
        void pack(const std::string& images_dir,
            const std::string & output_name,
            const std::string & output_path,
            const std::string & base_image_path="",
            const std::string & image_format="png")
        {
            auto image_infos = load_image_infos_from_dir(images_dir);
            return pack(image_infos, output_name, output_path, base_image_path, image_format);
        }

        void pack(const std::vector<std::string>& image_paths,
            const std::string & output_name,
            const std::string & output_path,
            const std::string & base_image_path="",
            const std::string & image_format="png")
        {
            auto image_infos = load_image_infos_from_paths(image_paths);
            return pack(image_infos, output_name, output_path, base_image_path, image_format);
        }
    };
} // namespace CppTexturePacker
#endif