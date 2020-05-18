#pragma once
#ifndef _CPP_TEXTURE_PACKER_UTILS_
#define _CPP_TEXTURE_PACKER_UTILS_

#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <cassert>

#define cimg_use_jpeg
#define cimg_use_png
#include <CImg.h>

#include "image_info.h"
#include "atlas.h"

namespace CppTexturePacker
{
namespace fs = std::filesystem;
using Image = cimg_library::CImg<unsigned char>;
using ImageInfoMap = std::unordered_map<int, ImageInfo>;


Image convert_to_rgba_image(const Image& src)
{
	if (src.spectrum() == 3)
	{
		Image dst = Image(src.width(), src.height(), src.depth(), 4);

		for (int w = 0; w < src.width(); ++w) {
			for (int h = 0; h < src.height(); ++h) {
				for (int d = 0; d < src.depth(); ++d) {
					dst(w, h, d, 0) = src(w, h, d, 0);
					dst(w, h, d, 1) = src(w, h, d, 1);
					dst(w, h, d, 2) = src(w, h, d, 2);
					dst(w, h, d, 3) = 255;
				}
			}
		}

		return dst;
	}

	assert(("unknown image channels", src.spectrum() == 4));
	return src;
}

Image read_image_from_file(std::string file_path)
{
	return convert_to_rgba_image(Image(file_path.c_str()));
}

void save_image_to_file(std::string file_path, const Image& image)
{
	if (file_path.size() < 4)
	{
		return;
	}

	auto suffix = file_path.substr(file_path.size() - 4);
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
	fs::path fp = file_path;
	auto abs_fp = fs::absolute(fp);
	return ImageInfo(read_image_from_file(abs_fp.string()), abs_fp.string());
}

std::vector<ImageInfo> load_image_infos_from_paths(std::vector<std::string> file_paths)
{
	std::vector<ImageInfo> image_infos;

	for (auto file_path : file_paths)
	{
		image_infos.emplace_back(read_image_from_file(file_path));
	}

	return image_infos;
}

std::vector<ImageInfo> load_image_infos_from_dir(std::string dir_path)
{
	std::vector<std::string> file_paths;

	for (auto &fe : fs::directory_iterator(dir_path))
	{
		auto file_path = fe.path().string();
		auto suffix = file_path.substr(file_path.size() - 4);
		if (
			suffix.compare(".bmp") == 0 ||
			suffix.compare(".jpg") == 0 ||
			suffix.compare(".png") == 0)
		{
			file_paths.emplace_back(file_path);
		}
	}

	return load_image_infos_from_paths(file_paths);
}

bool _is_border_pixel(Image& image, int x, int y)
{
	int width = image.width();
	int height = image.height();

	if (image(x, y, 0, 3) == 0) 
	{
		return false;
	}

	int offsets[3] = { -1, 0, 1 };
	for (int offset_x : offsets)
	{
		int nx = x + offset_x;
		for (int offset_y: offsets)
		{
			int ny = y + offset_y;
			if (nx >= 0 && nx < width &&
				ny >= 0 && ny < height &&
				image(nx, ny, 0, 3) == 0
				)
			{
				return true;
			}
		}
	}

	return false;
}

void alpha_bleeding(Image& image, size_t bleeding_pixel=4)
{
	using Vec2 = std::tuple<int, int>;

	std::vector<Vec2> borders0;
	std::vector<Vec2> borders1;
	int width = image.width();
	int height = image.height();

	for (int x = 0; x < width; ++x) 
	{
		for (int y = 0; y < height; ++y)
		{
			if (_is_border_pixel(image, x, y))
			{
				borders0.emplace_back(Vec2{ x, y });
			}
		}
	}
	
	int offsets[3] = { -1, 0, 1 };
	std::vector<Vec2>* borders = &borders0;
	std::vector<Vec2>* new_borders = &borders1;

	for (int i = 0; i < bleeding_pixel; ++i) {
		for (auto border : *borders)
		{
			int x = std::get<0>(border);
			int y = std::get<1>(border);

			for (int offset_x : offsets)
			{
				int nx = x + offset_x;
				for (int offset_y : offsets)
				{
					int ny = y + offset_y;
					if (nx >= 0 && nx < width &&
						ny >= 0 && ny < height &&
						image(nx, ny, 0, 3) == 0
						)
					{
						image(nx, ny, 0, 0) = image(x, y, 0, 0);
						image(nx, ny, 0, 1) = image(x, y, 0, 1);
						image(nx, ny, 0, 2) = image(x, y, 0, 2);
						image(nx, ny, 0, 3) = 1;

						if (_is_border_pixel(image, nx, ny))
						{
							new_borders->emplace_back(Vec2{ nx, ny });
						}
					}
				}
			}
		}

		auto tmp = borders;
		borders = new_borders;
		new_borders = tmp;
		new_borders->clear();
	}
}


void alpha_remove(Image& image)
{
	for (int w = 0; w < image.width(); ++w) 
	{
		for (int h = 0; h < image.height(); ++h)
		{
			for (int d = 0; d < image.depth(); ++d) 
			{
				image(w, h, d, 3) = 255;
			}
		}
	}
}

void clean_pixel_alpha_below(Image& image, unsigned char alpha)
{
	for (int w = 0; w < image.width(); ++w)
	{
		for (int h = 0; h < image.height(); ++h)
		{
			for (int d = 0; d < image.depth(); ++d)
			{
				if (image(w, h, d, 3) < alpha) 
				{
					image(w, h, d, 0) = 0;
					image(w, h, d, 1) = 0;
					image(w, h, d, 2) = 0;
					image(w, h, d, 3) = 0;
				}
			}
		}
	}
}

Rect<int> get_bounding_box(Image image)
{
	int width = image.width(),
		height = image.height();
	
	int l = width,
		t = height,
		r = 0,
		b = 0;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			if (image(x, y, 0, 3) != 0)
			{
				if (x < l)
				{
					l = x;
				}
				if (y < t)
				{
					t = y;
				}
				if (x > r)
				{
					r = x;
				}

				if (y > b)
				{
					b = y;
				}
			}
		}
	}
	
	return Rect<int>{ l, t, r - l, b - t };
}

void dump_altalas_to_plist()
{
}

void draw_image_in_image(Image& main_image, const Image& sub_image, int start_x, int start_y) 
{
	int m_width = main_image.width(),
		m_height = main_image.height(),
		s_width = sub_image.width(),
		s_height = sub_image.height();

	for (int offset_x = 0; offset_x < s_width && start_x + offset_x < m_width; ++offset_x)
	{
		for (int offset_y = 0; offset_y < s_height && start_y + offset_y < m_height; ++offset_y)
		{
			main_image(start_x + offset_x, start_y + offset_y, 0, 0) = sub_image(offset_x, offset_y, 0, 0);
			main_image(start_x + offset_x, start_y + offset_y, 0, 1) = sub_image(offset_x, offset_y, 0, 1);
			main_image(start_x + offset_x, start_y + offset_y, 0, 2) = sub_image(offset_x, offset_y, 0, 2);
			main_image(start_x + offset_x, start_y + offset_y, 0, 3) = sub_image(offset_x, offset_y, 0, 3);
		}
	}
}

Image dump_altalas_to_image(Atlas atlas, ImageInfoMap image_info_map)
{
	Image image(atlas.get_width(), atlas.get_height(), 1, 4, 0);
	for (auto image_rect : atlas.get_placed_image_rect())
	{
		auto image_info = image_info_map[image_rect.ex_key];
		draw_image_in_image(image, image_info.get_image(), image_rect.x, image_rect.y);
	}
	return image;
}

ImageInfoMap make_image_info_map(std::vector<ImageInfo> image_infos)
{
	ImageInfoMap image_info_map;
	for (const ImageInfo & image_info : image_infos) {
		image_info_map.emplace(image_info.get_ex_key(), image_info);
	}
	return image_info_map;
}

} // namespace CppTexturePacker

#endif