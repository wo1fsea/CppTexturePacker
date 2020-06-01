#pragma once
#ifndef _CPP_TEXTURE_PACKER_ALTALAS_
#define _CPP_TEXTURE_PACKER_ALTALAS_

#include <vector>
#include <tuple>

#include "rect.h"
#include "image_rect.h"

#define DEFAULT_ALTALAS_MAX_WIDTH 2048
#define DEFAULT_ALTALAS_MAX_HEIGHT 2048

namespace CppTexturePacker
{

    enum class ExpandStrategy : unsigned char
    {
        ExpandBoth = 0,
        ExpandeWidth = 1,
        ExpandeHeight = 2,
        ExpandeShortSide = 3,
        ExpandeLongSide = 4,
    };

    enum class RankStrategy : unsigned char
    {
        RankBSSF = 0,
        RankBLSF = 1,
        RankBAF = 2,
    };

    constexpr unsigned int MAX_RANK = -1;

    class Atlas
    {
    private:
        unsigned int width, height;
        unsigned int max_width, max_height;

        int border_padding;
        int shape_padding;

        bool force_square;

        ExpandStrategy expand_strategy;
        RankStrategy rank_strategy;

        std::vector<ImageRect> image_rects;
        std::vector<Rect<int>> free_rects;

    private:
        bool is_in_max_size(int new_width, int new_height)
        {
            return new_width <= max_width && new_height <= max_height;
        }

        void prune_free_rects()
        {
            std::vector<Rect<int>> new_free_rects;
            for (int i = 0; i < free_rects.size(); ++i)
            {
                auto rect0 = free_rects[i];
                bool pruned = false;
                for (int j = 0; j < free_rects.size(); ++j)
                {
                    auto rect1 = free_rects[j];
                    if (i != j && rect1.contains(rect0))
                    {
                        pruned = true;
                        break;
                    }
                }

                if (!pruned)
                {
                    new_free_rects.emplace_back(rect0);
                }
            }

            free_rects = new_free_rects;
        }

    public:
        Atlas(
            unsigned int _max_width = DEFAULT_ALTALAS_MAX_WIDTH,
            unsigned int _max_height = DEFAULT_ALTALAS_MAX_HEIGHT,
            bool _force_square = false,
            int _border_padding = 0,
            int _shape_padding = 0,
            ExpandStrategy _expand_strategy = ExpandStrategy::ExpandeShortSide,
            RankStrategy _rank_strategy = RankStrategy::RankBAF)
            : width(1),
              height(1),
              max_width(_max_width),
              max_height(_max_height),
              force_square(_force_square),
              border_padding(_border_padding),
              shape_padding(_shape_padding),
              expand_strategy(_expand_strategy),
              rank_strategy(_rank_strategy)
        {
            if (force_square)
            {
                expand_strategy = ExpandStrategy::ExpandBoth;
            }

            assert(2 * border_padding <= max_width && 2 * border_padding <= max_height);

            while (2 * border_padding >= width || 2 * border_padding >= height)
            {
                width *= 2;
                height *= 2;
            }

            free_rects.emplace_back(Rect<int>(
                {border_padding,
                 border_padding,
                 static_cast<int>(width) - 2 * border_padding,
                 static_cast<int>(height) - 2 * border_padding}));
        }

        const std::vector<ImageRect> &get_placed_image_rect() const
        {
            return image_rects;
        }

        int get_width() const
        {
            return width;
        }

        int get_height() const
        {
            return height;
        }

        bool try_expand()
        {
            int new_width = width, new_height = height;
            switch (expand_strategy)
            {

            case ExpandStrategy::ExpandeWidth:
                new_width = width * 2;
                break;

            case ExpandStrategy::ExpandeHeight:
                new_height = height * 2;
                break;

            case ExpandStrategy::ExpandeShortSide:
                if (width < height)
                {
                    new_width = width * 2;
                }
                else
                {
                    new_height = height * 2;
                }
                break;

            case ExpandStrategy::ExpandeLongSide:
                if (width >= height)
                {
                    new_width = width * 2;
                }
                else
                {
                    new_height = height * 2;
                }
                break;

            default:
            case ExpandStrategy::ExpandBoth:
                new_width = width * 2;
                new_height = height * 2;
                break;
            }

            if (!is_in_max_size(new_width, new_height))
            {
                return false;
            }

            int old_right = width - border_padding;
            int old_bottom = height - border_padding;
            int new_right = new_width - border_padding;
            int new_bottom = new_height - border_padding;

            for (Rect<int> &rect : free_rects)
            {
                if (rect.get_right() == old_right)
                {
                    rect.enlarge_right_to(new_right);
                }
                if (rect.get_bottom() == old_bottom)
                {
                    rect.enlarge_bottom_to(new_bottom);
                }
            }

            if (width != new_width)
            {
                free_rects.emplace_back(
                    Rect<int>({old_right,
                               border_padding,
                               static_cast<int>(new_width) - static_cast<int>(width),
                               static_cast<int>(new_height) - 2 * border_padding}));
            }

            if (height != new_height)
            {
                free_rects.emplace_back(
                    Rect<int>({border_padding,
                               old_bottom,
                               static_cast<int>(new_width) - 2 * border_padding,
                               static_cast<int>(new_height) - static_cast<int>(height)}));
            }

            width = new_width;
            height = new_height;

            prune_free_rects();

            return true;
        }

        void place_image_rect_in_free_rect(int free_rect_idx, ImageRect &image_rect)
        {
            auto free_rect = free_rects[free_rect_idx];
            
            int sp_x = free_rect.x == border_padding? 0: shape_padding;
            int sp_y = free_rect.y == border_padding? 0: shape_padding;

            image_rect.x = free_rect.x + sp_x;
            image_rect.y = free_rect.y + sp_y;

            ImageRect tmp_rect = image_rect;
            tmp_rect.enlarge_left_to(image_rect.get_left() - sp_x);
            tmp_rect.enlarge_top_to(image_rect.get_top() - sp_y);

            std::vector<Rect<int>> nonoverlapped_free_rects;
            std::vector<Rect<int>> new_free_rects;

            for (auto rect : free_rects)
            {
                if (tmp_rect.is_overlaped(rect))
                {
                    for (auto new_rect : rect.cut(tmp_rect))
                    {
                        new_free_rects.emplace_back(new_rect);
                    }
                }
                else
                {
                    nonoverlapped_free_rects.emplace_back(rect);
                }
            }

            free_rects = new_free_rects;
            prune_free_rects();

            for (auto rect : nonoverlapped_free_rects)
            {
                free_rects.emplace_back(rect);
            }

            image_rects.emplace_back(image_rect);
        }

        unsigned int rank(Rect<int> free_rect, const ImageRect &image_rect, RankStrategy rank_stratege = RankStrategy::RankBAF) const
        {
            unsigned int r = MAX_RANK;
            switch (rank_stratege)
            {
            case RankStrategy::RankBSSF:
                r = image_rect.width >= image_rect.height ? free_rect.width - image_rect.width : free_rect.height - image_rect.height;
                break;
            case RankStrategy::RankBLSF:
                r = image_rect.width <= image_rect.height ? free_rect.width - image_rect.width : free_rect.height - image_rect.height;
                break;
            case RankStrategy::RankBAF:
                r = free_rect.get_area() - image_rect.get_area();
                break;
            }

            int sp_x = free_rect.x == border_padding? 0: shape_padding;
            int sp_y = free_rect.y == border_padding? 0: shape_padding;

            if (r < 0 ||
                free_rect.width - image_rect.width < sp_x ||
                free_rect.height - image_rect.height < sp_y)
            {
                return MAX_RANK;
            }

            return r;
        }

        std::tuple<unsigned int, unsigned int, bool> find_best_rank_without_rotate(const ImageRect &image_rect) const
        {
            unsigned int best_rank = MAX_RANK;
            unsigned int best_free_rect_index = -1;

            for (int index = 0; index < free_rects.size(); ++index)
            {
                auto r = rank(free_rects[index], image_rect);
                if (r < best_rank)
                {
                    best_rank = r;
                    best_free_rect_index = index;
                }
            }

            return std::make_tuple(best_rank, best_free_rect_index, false);
        }

        std::tuple<unsigned int, unsigned int, bool> find_best_rank_with_rotate(const ImageRect &image_rect) const
        {
            unsigned int best_rank, best_rank_r;
            unsigned int best_free_rect_index, best_free_rect_index_r;
            bool rotate, rotate_r;

            ImageRect image_rect_rotated = image_rect;
            image_rect_rotated.rotate();

            std::tie(best_rank, best_free_rect_index, rotate) = find_best_rank_without_rotate(image_rect);
            std::tie(best_rank_r, best_free_rect_index_r, rotate_r) = find_best_rank_without_rotate(image_rect_rotated);

            if (best_rank <= best_rank_r)
            {
                return std::make_tuple(best_rank, best_free_rect_index, false);
            }
            else
            {
                return std::make_tuple(best_rank_r, best_free_rect_index_r, true);
            }
        }

        std::tuple<unsigned int, unsigned int, bool> find_best_rank(const ImageRect &image_rect, bool enable_rotate) const
        {
            if (enable_rotate)
            {
                return find_best_rank_with_rotate(image_rect);
            }
            else
            {
                return find_best_rank_without_rotate(image_rect);
            }
        }
    };

} // namespace CppTexturePacker

#endif