#pragma once
#ifndef _CPP_TEXTURE_PACKER_ALTALAS_
#define _CPP_TEXTURE_PACKER_ALTALAS_

#include <vector>

#include "rect.h"
#include "image_rect.h"

#define DEFAULT_ALTALAS_MAX_WIDTH 2048
#define DEFAULT_ALTALAS_MAX_HEIGHT 2048

namespace CppTexturePacker
{

enum ExpandStrategy
{
    ExpandBoth = 0,
    ExpandeWidth = 1,
    ExpandeHeight = 2,
    ExpandeShortSide = 3,
    ExpandeLongSide = 4,
};

enum RankStrategy
{
    RankBSSF = 0,
    RankBLSF = 1,
    RankBAF = 2,
};

class Altalas
{
private:
    int width, height;
    int max_width, max_height;

    int border_padding;
    int shape_padding;
    int inner_padding;

    bool force_square;

    ExpandStrategy expand_strategy;
    RankStrategy rank_strategy;

    std::vector<ImageRect> image_rects;
    std::vector<Rect<int>> free_rects;

private:
    bool is_in_max_size(int new_width, int new_height)
    {
        return new_width <= max_width && new_height <= new_height;
    }

    void prune_free_rects()
    {
        std::vector<Rect<int>> new_free_rects;
        for(int i=0; i<free_rects.size(); ++i)
        {
            auto rect0 = free_rects[i];
            bool pruned = false;
            for(int j=i+1; j<free_rects.size(); ++j)
            {
                auto rect1 = free_rects[i];
                if(rect1.contains(rect0))
                {
                    pruned = true;
                    break;
                }
            }

            if(!pruned)
            {
                new_free_rects.emplace_back(rect0);
            }
        }

        free_rects = new_free_rects;
    }

public:
    Altalas(
        int _max_width = DEFAULT_ALTALAS_MAX_WIDTH,
        int _max_height = DEFAULT_ALTALAS_MAX_HEIGHT,
        int _border_padding = 0,
        int _shape_padding = 0,
        int _inner_padding = 0,
        ExpandStrategy _expand_strategy = ExpandStrategy::ExpandBoth,
        RankStrategy _rank_strategy = RankStrategy::RankBAF)
        : width(1),
          height(1),
          max_width(_max_width),
          max_height(_max_height),
          border_padding(_border_padding),
          shape_padding(_shape_padding),
          inner_padding(_inner_padding),
          expand_strategy(_expand_strategy),
          rank_strategy(_rank_strategy)
    {
        if (force_square)
        {
            expand_strategy = ExpandStrategy::ExpandBoth;
        }

        free_rects.emplace_back(Rect<int>(
            {border_padding,
             border_padding,
             width - 2 * border_padding,
             height - 2 * border_padding}));
    }

    const std::vector<ImageRect> &get_placed_image_rect() { return image_rects; }

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

        for (auto rect : free_rects)
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
                           new_width - width,
                           new_height - 2 * border_padding}));
        }

        if (height != new_height)
        {
            free_rects.emplace_back(
                Rect<int>({border_padding,
                           old_bottom,
                           new_width - 2 * border_padding,
                           new_height - height}));
        }

        width = new_width;
        height = new_height;

        prune_free_rects();

        return true;
    }

    bool place_image_rect_in_free_rect(int free_rect_idx, ImageRect image_rect)
    {
        auto free_rect = free_rects[free_rect_idx];
        image_rect.x = free_rect.x + inner_padding;
        image_rect.y = free_rect.y + inner_padding;

        ImageRect tmp_rect = image_rect;
        tmp_rect.enlarge_left_to(image_rect.get_left() - inner_padding);
        tmp_rect.enlarge_right_to(image_rect.get_right() + inner_padding + shape_padding);
        tmp_rect.enlarge_top_to(image_rect.get_top() - inner_padding);
        tmp_rect.enlarge_bottom_to(image_rect.get_bottom() + inner_padding + shape_padding);

        std::vector<Rect<int>> nonoverlapped_free_rects;
        std::vector<Rect<int>> new_free_rects;

        for(auto rect: free_rects)
        {
            if(tmp_rect.is_overlaped(rect))
            {
                for(auto new_rect: rect.cut(tmp_rect))
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

        for(auto rect: nonoverlapped_free_rects)
        {
            free_rects.emplace_back(rect);
        }

        image_rects.emplace_back(image_rect);
    }
};

} // namespace CppTexturePacker

#endif