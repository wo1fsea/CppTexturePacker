#pragma once
#ifndef _CPP_TEXTURE_PACKER_UTILS_
#define _CPP_TEXTURE_PACKER_UTILS_

#include <vector>
#include <string>

#define cimg_use_jpeg
#define cimg_use_png
#include <CImg.h>

#include "image_info.h"

namespace CppTexturePacker
{
	using Image = cimg_library::CImg<unsigned char>;
	class ImageInfo;

	Image read_image_from_file(std::string file_path) 
	{
		return Image(file_path.c_str());
	}

	void save_image_to_file(std::string file_path, Image image)
	{
		if (file_path.size() < 4)
		{
			return;
		}

		auto suffix = file_path.substr(file_path.size()-4);
		if (suffix.compare(".bmp") == 0)
		{
			image.save_bmp(file_path.c_str());
		}
		else if (suffix.compare(".jpg") == 0)
		{
			image.save_jpeg(file_path.c_str());
		}
		else if (suffix.compare(".png") == 0)
		{
			image.save_png(file_path.c_str());
		}
	}

	ImageInfo read_image_info_from_file(std::string file_path)
	{
		return ImageInfo();
	}

	std::vector<ImageInfo> load_image_infos_from_paths(std::vector<std::string> file_paths)
	{
		std::vector<ImageInfo> image_infos;

		return image_infos;
	}
	
	std::vector<ImageInfo> load_image_infos_from_dir(std::string dir_path)
	{
		std::vector<ImageInfo> image_infos;

		return image_infos;
	}

	void alpha_bleeding(Image image, unsigned char alpha)
	{

	}

	void alpha_remove(Image image, unsigned char alpha)
	{

	}

	void clean_pixel_alpha_below(Image image, unsigned char alpha)
	{

	}


	void dump_altalas_to_plist()
	{

	}

	Image dump_altalas_to_image()
	{
		return Image();
	}
}

#endif