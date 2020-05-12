#pragma once
#ifndef _CPP_TEXTURE_PACKER_UTILS_
#define _CPP_TEXTURE_PACKER_UTILS_

#include <vector>
#include <string>

#define cimg_use_jpeg
#define cimg_use_png
#include <CImg.h>


namespace CppTexturePacker
{
	using Image = cimg_library::CImg<unsigned char>;

	Image read_file(std::string file_path) 
	{
		return Image(file_path.c_str());
	}

	void save_file(std::string file_path, Image image)
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
}

#endif