/*
#include "cpp_texture_packer.h"


int main(int argc, char* argv[])
{
    std::string path = "C:/GITHUB/CppTexturePacker/data/";
    std::string save_path = "C:/GITHUB/CppTexturePacker/p2.png";
    std::string save_path2 = "C:/GITHUB/CppTexturePacker/p3.png";
    auto image_infos = CppTexturePacker::load_image_infos_from_dir(path);

    auto packer = CppTexturePacker::RectPacker();
    std::vector<CppTexturePacker::ImageRect> image_rects;
    for(auto image_info: image_infos)
    {
        image_rects.emplace_back(image_info.get_image_rect());
    }
    packer.add_image_rects(image_rects);

    auto image = dump_altalas_to_image(packer.atlases[0], make_image_info_map(image_infos));

    CppTexturePacker::save_image_to_file(save_path, image);

    CppTexturePacker::trim_image(image, CppTexturePacker::get_bounding_box(image));
    CppTexturePacker::save_image_to_file(save_path2, image);
    return 0;
}
*/
#include "plist/plist++.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif


int main(int argc, char *argv[])
{
    char *plist_xml = NULL;
    uint32_t size_out = 0;
    char *file_out = "C:/GITHUB/CppTexturePacker/a.plist";

    PList::Dictionary dd;
    PList::String s("aaa");
    dd.Set("a", s);

    plist_to_xml(dd.GetPlist(), &plist_xml, &size_out);
    if (!plist_xml)
    {
        printf("PList XML writing failed\n");
        return 8;
    }
    else
        printf("PList XML writing succeeded\n");

    if (plist_xml)
    {
        FILE *oplist = NULL;
        oplist = fopen(file_out, "wb");
        fwrite(plist_xml, size_out, sizeof(char), oplist);
        fclose(oplist);
    }

    free(plist_xml);

    //success
    return 0;
}