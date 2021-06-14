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
    if (m_handleMap.count(key)) return false;
    m_handleMap[key] = DxLib::LoadGraph(fileName.c_str());
    if (m_handleMap[key] == -1) return false;

    return true;
}

void ImageMng::DeleteImage(const std::string& key)
{
    if (!m_handleMap.count(key)) return;
    DxLib::DeleteGraph(m_handleMap.at(key));
    m_handleMap.erase(key);
}

int ImageMng::GetID(const std::string& key)
{
    return m_handleMap.at(key);
}

ImageMng::ImageMng() {}
ImageMng::~ImageMng() {}

// Don't allow copy and move semantics
ImageMng::ImageMng(const ImageMng&) {}
ImageMng::ImageMng(ImageMng&&) noexcept {}
void ImageMng::operator=(const ImageMng&) {}
void ImageMng::operator=(ImageMng&&) noexcept {}
//
