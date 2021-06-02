#include "ImageMng.h"

#include <cassert>

#include <DxLib.h>

ImageMng& ImageMng::Instance()
{
    static ImageMng instance;
    return instance;
}

bool ImageMng::AddImage(const std::string& imageName, const std::string& fileName)
{
    if (m_imageIDs.count(imageName)) return false;
    m_imageIDs[imageName] = DxLib::LoadGraph(fileName.c_str());
    if (m_imageIDs[imageName] == -1) return false;

    return true;
}

int ImageMng::GetID(const std::string& imageName)
{
    return m_imageIDs.at(imageName);
}

ImageMng::ImageMng() {}
ImageMng::~ImageMng() {}

// Don't allow copy and move semantics
ImageMng::ImageMng(const ImageMng&) {}
ImageMng::ImageMng(ImageMng&&) noexcept {}
void ImageMng::operator=(const ImageMng&) {}
void ImageMng::operator=(ImageMng&&) noexcept {}
//
