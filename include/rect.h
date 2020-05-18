#pragma once
#ifndef _CPP_TEXTURE_PACKER_RECT_
#define _CPP_TEXTURE_PACKER_RECT_

#include <vector>

namespace CppTexturePacker
{

template <typename T>
struct Rect
{
    /*
    (x, y)
    (left, top)
            +----+
            |    |
            +----+
                (x + width, y + height)
                (right, bottom)
    */

    T x, y, width, height;

    T get_left() const
    {
        return x;
    }

    void set_left(T l)
    {
        x = l;
    }

    T get_right() const
    {
        return x + width;
    }

    void set_right(T r)
    {
        x = r - width;
    }

    T get_top() const
    {
        return y;
    }

    void set_top(T t)
    {
        y = t;
    }

    T get_bottom() const
    {
        return y + height;
    }

    void set_bottom(T b)
    {
        height = b - y;
    }

    void enlarge_left_to(T l)
    {
        width = get_right() - l;
        x = l;
    }

    void enlarge_right_to(T r)
    {
        width = r - get_left();
    }

    void enlarge_top_to(T t)
    {
        height = get_bottom() - t;
        y = t;
    }

    void enlarge_bottom_to(T b)
    {
        height = b - get_top();
    }

    T get_area() const
    {
        return width * height;
    }

    void rotate()
    {
        T tmp = width;
        height = width;
        width = tmp;
    }

    bool is_overlaped(Rect<T> rect) const
    {

        return !(
               x >= rect.x + rect.width ||
               y >= rect.y + rect.height ||
               x + width <= rect.x ||
               y + height <= rect.y
            );
    }

    bool contains(Rect<T> rect) const
    {
        return x <= rect.x &&
               y <= rect.y &&
               x + width >= rect.x + rect.width &&
               y + height >= rect.y + rect.height;
    }

    bool same(Rect<T> rect) const
    {
        return x == rect.x &&
               y == rect.y &&
               width == rect.width &&
               height == rect.height;
    }

    std::vector<Rect<T>> cut(Rect<T> rect) const
    {
        std::vector<Rect<T>> rects;
        if (contains(rect))
        {
            Rect<T> tmp_rect;

            if (get_left() < rect.get_left())
            {
                tmp_rect = *this;
                tmp_rect.enlarge_right_to(rect.get_left());
                if (tmp_rect.get_area() > 0)
                {
                    rects.emplace_back(tmp_rect);
                }
            }
            if (get_top() < rect.get_top())
            {
                tmp_rect = *this;
                tmp_rect.enlarge_bottom_to(rect.get_top());
                if (tmp_rect.get_area() > 0)
                {
                    rects.emplace_back(tmp_rect);
                }
            }
            if (get_right() > rect.get_right())
            {
                tmp_rect = *this;
                tmp_rect.enlarge_left_to(rect.get_right());
                if (tmp_rect.get_area() > 0)
                {
                    rects.emplace_back(tmp_rect);
                }
            }
            if (get_bottom() > rect.get_bottom())
            {
                tmp_rect = *this;
                tmp_rect.enlarge_top_to(rect.get_bottom());
                if (tmp_rect.get_area() > 0)
                {
                    rects.emplace_back(tmp_rect);
                }
            }
        }
        else
        {
            rects.emplace_back(*this);
        }

        return rects;
    }
};

}; // namespace CppTexturePacker

#endif