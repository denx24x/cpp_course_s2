#pragma once
#include "Image.h"

class SeamCarver
{
    using Seam = std::vector<size_t>;
    struct EnergyMap
    {
        EnergyMap(size_t w, size_t h);

        size_t GetWidth() const;
        size_t GetHeight() const;
        double Get(size_t x, size_t y) const;

        std::vector<std::vector<double>> m_data;
    };

public:
    SeamCarver(Image image);

    /**
     * Returns current image
     */
    const Image & GetImage() const;

    /**
     * Gets current image width
     */
    size_t GetImageWidth() const;

    /**
     * Gets current image height
     */
    size_t GetImageHeight() const;

    /**
     * Returns pixel energy
     * @param columnId column index (x)
     * @param rowId row index (y)
     */
    double GetPixelEnergy(size_t columnId, size_t rowId) const;

    /**
     * Returns sequence of pixel row indexes (y)
     * (x indexes are [0:W-1])
     */
    Seam FindHorizontalSeam() const;

    /**
     * Returns sequence of pixel column indexes (x)
     * (y indexes are [0:H-1])
     */
    Seam FindVerticalSeam() const;

    /**
     * Removes sequence of pixels from the image
     */
    void RemoveHorizontalSeam(const Seam & seam);

    /**
     * Removes sequence of pixes from the image
     */
    void RemoveVerticalSeam(const Seam & seam);

private:
    Seam FindSeam(const EnergyMap & energy, bool transposed) const;
    EnergyMap CalculateEnergyMap() const;
    Image m_image;
};