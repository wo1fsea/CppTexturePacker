#include "cpp_texture_packer.h"


int main(int argc, char* argv[])
{
    std::string path = "e:/Projects/CppTexturePacker/data/";
    // std::string save_path = "C:/GITHUB/CppTexturePacker/p2.png";
    // std::string save_path2 = "C:/GITHUB/CppTexturePacker/p3.png";
    auto p = CppTexturePacker::TexturePacker();
    p.pack(path, "CppTexturePacker", "e:/Projects/CppTexturePacker/", "e:/Projects/CppTexturePacker/");
    return 0;
}
