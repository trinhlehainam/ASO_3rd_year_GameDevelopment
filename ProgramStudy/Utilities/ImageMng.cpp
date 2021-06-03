#include "ImageMng.h"

#include <cassert>

#include <DxLib.h>

ImageMng& ImageMng::Instance()
{
    static ImageMng instance;
    return instance;
}

bool ImageMng::AddImage(const std::string& key, const std::string& fileName)
{
    if (m_imageIDs.count(key)) return false;
    m_imageIDs[key] = DxLib::LoadGraph(fileName.c_str());
    if (m_imageIDs[key] == -1) return false;

    return true;
}

int ImageMng::GetID(const std::string& key)
{
    return m_imageIDs.at(key);
}

ImageMng::ImageMng() {}
ImageMng::~ImageMng() {}

// Don't allow copy and move semantics
ImageMng::ImageMng(const ImageMng&) {}
ImageMng::ImageMng(ImageMng&&) noexcept {}
void ImageMng::operator=(const ImageMng&) {}
void ImageMng::operator=(ImageMng&&) noexcept {}
//
