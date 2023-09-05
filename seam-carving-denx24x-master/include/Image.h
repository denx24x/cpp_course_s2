#pragma once
#include <vector>

struct Image
{
    struct Pixel
    {
        Pixel(int red, int green, int blue);
        Pixel GetDifference(const Pixel & right) const;
        int GetSumOfSquaresOfComponents() const;

        int m_red;
        int m_green;
        int m_blue;
    };

    Image(std::vector<std::vector<Pixel>> table);
    Pixel GetPixel(size_t columnId, size_t rowId) const;
    size_t GetColumnIdNormalized(size_t columnId) const;
    size_t GetRowIdNormalized(size_t rowId) const;
    size_t GetWidth() const;
    size_t GetHeight() const;

    std::vector<std::vector<Pixel>> m_table;
};