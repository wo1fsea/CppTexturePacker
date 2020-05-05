#pragma once
#ifndef _CPP_TEXTURE_PACKER_RECT_
#define _CPP_TEXTURE_PACKER_RECT_

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

    T get_left()
    {
        return x;
    }

    void set_left(T l)
    {
        x = l;
    }

    T get_right()
    {
        return x + width;
    }

    void set_right(T r)
    {
        x = r - width;
    }

    T get_top()
    {
        return y;
    }

    void set_top(T t)
    {
        y = t;
    }

    T get_bottom()
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

    T get_area()
    {
        return width * height;
    }

    void rotate()
    {
        T tmp = width;
        height = width;
        width = tmp;
    }
};

}; // namespace CppTexturePacker

#endif