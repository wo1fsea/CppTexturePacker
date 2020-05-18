#include "cpp_texture_packer.h"


int main(int argc, char* argv[])
{
    std::string path = "C:/GITHUB/CppTexturePacker/data/";
    std::string save_path = "C:/GITHUB/CppTexturePacker/p2.png";

    auto image_infos = CppTexturePacker::load_image_infos_from_dir(path);

    auto packer = CppTexturePacker::Packer();
    std::vector<CppTexturePacker::ImageRect> image_rects;
    for(auto image_info: image_infos)
    {
        image_rects.emplace_back(image_info.get_image_rect());
    }
    packer.add_image_rects(image_rects);

    auto image = dump_altalas_to_image(packer.atlases[0], make_image_info_map(image_infos));

    CppTexturePacker::save_image_to_file(save_path, image);


    return 0;
}