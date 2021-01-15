#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    x = x < 0 ? 0 : (x >= im.w ? im.w - 1 : x);
    y = y < 0 ? 0 : (y >= im.h ? im.h - 1 : y);
    c = c < 0 ? 0 : (c >= im.c ? im.c - 1 : c);
    int idx = c * im.h * im.w + y * im.w + x;

    return im.data[idx];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if (x < 0 || x >= im.w || y < 0 || y >= im.h || c < 0 || c >= im.c) return;
    int idx = c * im.h * im.w + y * im.w + x;
    im.data[idx] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int i = 0; i < im.w; ++i)
    {
        for (int j = 0; j < im.h; ++j)
        {
//            float v_gray = (get_pixel(im, i, j, 0) + get_pixel(im, i, j, 1) + get_pixel(im, i, j, 2)) / 3;
            float v_gray = get_pixel(im, i, j, 0) * 0.299
                           + get_pixel(im, i, j, 1) * 0.587 + get_pixel(im, i, j, 2) * 0.114;
            set_pixel(gray, i, j, 0, v_gray);
        }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    if (c < 0 || c >= im.c)
        return;
    for (int i = 0; i < im.w; ++i)
    {
        for (int j = 0; j < im.h; ++j)
        {
            set_pixel(im, i, j, c, get_pixel(im, i, j, c) + v);
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    int total_num = im.w * im.h * im.c;
    for (int i = 0; i < total_num; ++i)
    {
        im.data[i] = im.data[i] < 0 ? 0 :
                     im.data[i] > 1 ? 1 :
                     im.data[i];
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    float R, G, B, V, m, C, S, H_, H;
    for (int i = 0; i < im.w; ++i)
    {
        for (int j = 0; j < im.h; ++j)
        {
            R = get_pixel(im, i, j, 0);
            G = get_pixel(im, i, j, 1);
            B = get_pixel(im, i, j, 2);
            V = three_way_max(R, G, B);
            m = three_way_min(R, G, B);
            C = V - m;
            S = V < 1e-5 ? 0 : C / V;

            if (C < 1e-5)
            {
                H = 0;
            } else
            {
                if (V == R) H_ = (G - B) / C;
                else if (V == G) H_ = (B - R) / C + 2;
                else H_ = (R - G) / C + 4;

                H = H_ < 0 ? H_ / 6 + 1 : H_ / 6;
            }

            set_pixel(im, i, j, 0, H);
            set_pixel(im, i, j, 1, S);
            set_pixel(im, i, j, 2, V);
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    float R, G, B, V, m, C, S, H_, H;
    for (int i = 0; i < im.w; ++i)
    {
        for (int j = 0; j < im.h; ++j)
        {
            H = get_pixel(im, i, j, 0);
            S = get_pixel(im, i, j, 1);
            V = get_pixel(im, i, j, 2);
            C = S * V;
            m = V - C;

            if (H < 1e-5)
            {
                R = 0;
                G = 0;
                B = 0;
            } else
            {
                if (H >= (5.f / 6.f))
                {
                    R = V;
                    H_ = (H - 1.f) * 6.f;
                    if (H_ > 0)
                    {
                        B = m;
                        G = H_ * C + B;
                    } else
                    {
                        G = m;
                        B = -(H_ * C - G);
                    }
                } else if (H >= 1.f / 2.f)
                {
                    H_ = H * 6.f;
                    B = V;
                    if (H_ > 4)
                    {
                        G = m;
                        R = (H_ - 4) * C + G;
                    } else
                    {
                        R = m;
                        G = -((H_ - 4) * C - R);
                    }
                } else
                {
                    H_ = H * 6.f;
                    G = V;
                    if (H_ > 2)
                    {
                        R = m;
                        B = (H_ - 2) * C + R;
                    } else
                    {
                        B = m;
                        R = -((H_ - 2) * C - B);
                    }
                }
            }
            set_pixel(im, i, j, 0, R);
            set_pixel(im, i, j, 1, G);
            set_pixel(im, i, j, 2, B);
        }
    }

}