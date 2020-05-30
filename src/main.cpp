
#include <string>
#include "cmdline.h"
#include "cpp_texture_packer.h"

int main(int argc, char *argv[])
{
  cmdline::parser parser;

  parser.add<std::string>("input_dir", 'i', "input dir", true, "");
  parser.add<std::string>("ouput_name", 'n', "output atlas name", false, "out");
  parser.add<std::string>("ouput_dir", 'o', "output dir", false, "./");
  
  parser.add<std::string>("base_image_path", 'p', "base image path", false, "");
  parser.add<std::string>("image_format", 'f', "output image format", false, "png");

  parser.add<unsigned int>("max_width", 'w', "max atlas width", false, 4096, cmdline::range(1, 65535));
  parser.add<unsigned int>("max_height", 'h', "max atlas height", false, 4096, cmdline::range(1, 65535));
  
  parser.add<bool>("enable_rotate", 'r', "enable rotate", false, false);
  parser.add<bool>("force_square", 's', "force square", false, false);

  parser.add<unsigned int>("border_padding", '\0', "border padding", false, 0, cmdline::range(1, 255));
  parser.add<unsigned int>("shape_padding", '\0', "shape padding", false, 0, cmdline::range(1, 255));
  parser.add<unsigned int>("inner_padding", '\0', "inner padding", false, 0, cmdline::range(1, 255));
  
  parser.add<bool>("reduce_border_artifacts", 'b', "reduce border artifacts", false, false);
  parser.add<unsigned int>("trim_mode", 't', "trim pixel alpha less than input value", false, 0, cmdline::range(0, 255));
  parser.add<unsigned int>("extrude", 'e', "extrude", false, 0, cmdline::range(0, 255));
  
  parser.parse_check(argc, argv);


  std::string path = "/Users/huangquanyong/Projects/CppTexturePacker/data";
  // std::string save_path = "C:/GITHUB/CppTexturePacker/p2.png";
  // std::string save_path2 = "C:/GITHUB/CppTexturePacker/p3.png";
  auto packer = CppTexturePacker::TexturePacker(
      parser.get<unsigned int>("max_width"),
      parser.get<unsigned int>("max_height"),
      parser.get<bool>("enable_rotate"),
      parser.get<bool>("force_square"),
      parser.get<unsigned int>("border_padding"),
      parser.get<unsigned int>("shape_padding"),
      parser.get<unsigned int>("inner_padding"),
      parser.get<bool>("reduce_border_artifacts"),
      parser.get<unsigned int>("trim_mode"),
      parser.get<unsigned int>("extrude")
  );
  packer.pack(
    parser.get<std::string>("input_dir"), 
    parser.get<std::string>("ouput_name"), 
    parser.get<std::string>("ouput_dir"),
  
    parser.get<std::string>("base_image_path"), 
    parser.get<std::string>("image_format")
      );
  return 0;
}

