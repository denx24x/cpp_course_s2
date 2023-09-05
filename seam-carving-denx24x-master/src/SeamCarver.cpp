#include "SeamCarver.h"

#include <algorithm>
#include <cmath>

SeamCarver::EnergyMap::EnergyMap(size_t w, size_t h)
    : m_data(w, std::vector<double>(h))
{
}

size_t SeamCarver::EnergyMap::GetWidth() const
{
    return m_data.size();
}

size_t SeamCarver::EnergyMap::GetHeight() const
{
    return m_data.empty() ? 0 : m_data[0].size();
}

double SeamCarver::EnergyMap::Get(size_t columnId, size_t rowId) const
{
    return m_data[columnId][rowId];
}

SeamCarver::SeamCarver(Image image)
    : m_image(std::move(image))
{
}

const Image & SeamCarver::GetImage() const
{
    return m_image;
}

size_t SeamCarver::GetImageWidth() const
{
    return m_image.GetWidth();
}

size_t SeamCarver::GetImageHeight() const
{
    return m_image.GetHeight();
}

double SeamCarver::GetPixelEnergy(size_t x, size_t y) const
{
    double x_component = m_image.GetPixel(m_image.GetColumnIdNormalized(x + 1), y).GetDifference(m_image.GetPixel(m_image.GetColumnIdNormalized(x - 1), y)).GetSumOfSquaresOfComponents();
    double y_component = m_image.GetPixel(x, m_image.GetRowIdNormalized(y + 1)).GetDifference(m_image.GetPixel(x, m_image.GetRowIdNormalized(y - 1))).GetSumOfSquaresOfComponents();
    return sqrt(x_component + y_component);
}

SeamCarver::Seam SeamCarver::FindHorizontalSeam() const
{
    return FindSeam(CalculateEnergyMap(), false);
}

SeamCarver::Seam SeamCarver::FindVerticalSeam() const
{
    return FindSeam(CalculateEnergyMap(), true);
}

SeamCarver::EnergyMap SeamCarver::CalculateEnergyMap() const
{
    EnergyMap result(GetImageWidth(), GetImageHeight());
    for (size_t i = 0; i < GetImageWidth(); i++) {
        for (size_t g = 0; g < GetImageHeight(); g++) {
            result.m_data[i][g] = GetPixelEnergy(i, g);
        }
    }
    return result;
}

SeamCarver::Seam SeamCarver::FindSeam(const SeamCarver::EnergyMap & energy, bool transposed) const
{
    if (GetImageHeight() == 0 || GetImageWidth() == 0) {
        return Seam();
    }
    size_t width = transposed ? energy.GetHeight() : energy.GetWidth();
    size_t height = transposed ? energy.GetWidth() : energy.GetHeight();
    std::vector<std::vector<double>> dp(width, std::vector<double>(height));
    std::vector<std::vector<size_t>> path(width, std::vector<size_t>(height));
    for (size_t g = 0; g < height; g++) {
        dp[0][g] = transposed ? energy.Get(g, 0) : energy.Get(0, g);
    }
    for (size_t i = 1; i < width; i++) {
        for (size_t g = 0; g < height; g++) {
            auto val = std::min_element(dp[i - 1].begin() + (g == 0 ? g : g - 1), dp[i - 1].begin() + (g == height - 1 ? g : g + 1) + 1);
            path[i][g] = val - dp[i - 1].begin();
            dp[i][g] = *val + (transposed ? energy.Get(g, i) : energy.Get(i, g));
        }
    }

    size_t pos = std::min_element(dp[width - 1].begin(), dp[width - 1].end()) - dp[width - 1].begin();
    Seam answer = {pos};
    for (size_t w = width - 1; w > 0; w--) {
        pos = path[w][pos];
        answer.push_back(pos);
    }
    std::reverse(answer.begin(), answer.end());
    return answer;
}

void SeamCarver::RemoveHorizontalSeam(const Seam & seam)
{
    for (size_t i = 0; i < GetImageWidth(); i++) {
        m_image.m_table[i].erase(m_image.m_table[i].begin() + seam[i]);
    }
    if (GetImageHeight() == 0) {
        m_image.m_table.clear();
    }
}

void SeamCarver::RemoveVerticalSeam(const Seam & seam)
{
    for (size_t i = 0; i < GetImageHeight(); i++) {
        for (size_t g = seam[i] + 1; g < GetImageWidth(); g++) {
            m_image.m_table[g - 1][i] = m_image.m_table[g][i];
        }
    }
    if (!m_image.m_table.empty()) {
        m_image.m_table.pop_back();
    }
}
