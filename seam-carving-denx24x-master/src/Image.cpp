#include "Image.h"

#include <cmath>

Image::Image(std::vector<std::vector<Image::Pixel>> table)
    : m_table(std::move(table))
{
}

Image::Pixel Image::Pixel::GetDifference(const Pixel & right) const
{
    return Pixel(abs(m_red - right.m_red), abs(m_green - right.m_green), abs(m_blue - right.m_blue));
}

Image::Pixel::Pixel(int red, int green, int blue)
    : m_red(red)
    , m_green(green)
    , m_blue(blue)
{
}

int Image::Pixel::GetSumOfSquaresOfComponents() const
{
    return pow(m_red, 2) + pow(m_green, 2) + pow(m_blue, 2);
}

size_t Image::GetColumnIdNormalized(size_t columnId) const
{
    return (GetWidth() + columnId) % GetWidth();
}

size_t Image::GetRowIdNormalized(size_t rowId) const
{
    return (GetHeight() + rowId) % GetHeight();
}

Image::Pixel Image::GetPixel(size_t columnId, size_t rowId) const
{
    return m_table[columnId][rowId];
}

size_t Image::GetWidth() const
{
    return m_table.size();
}

size_t Image::GetHeight() const
{
    return m_table.empty() ? 0 : m_table[0].size();
}
