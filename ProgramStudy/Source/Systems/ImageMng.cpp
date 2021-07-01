#include "ImageMng.h"

#include <cassert>

#include <DxLib.h>

ImageMng* ImageMng::m_instance = nullptr;

void ImageMng::Create()
{
    if (m_instance == nullptr)
        m_instance = new ImageMng();
}

void ImageMng::Destroy()
{
    if (m_instance != nullptr)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

ImageMng& ImageMng::Instance()
{
    if (m_instance == nullptr)
        Create();
    return *m_instance;
}

bool ImageMng::AddImage(const std::string& fileName, const std::string& key)
{
    if (m_instance->m_handleMap.count(key)) return false;
    m_instance->m_handleMap[key] = DxLib::LoadGraph(fileName.c_str());
    if (m_instance->m_handleMap[key] == -1) return false;

    return true;
}

void ImageMng::DeleteImage(const std::string& key)
{
    if (!m_instance->m_handleMap.count(key)) return;
    DxLib::DeleteGraph(m_instance->m_handleMap.at(key));
    m_instance->m_handleMap.erase(key);
}

int ImageMng::GetID(const std::string& key)
{
    return m_instance->m_handleMap.at(key);
}

ImageMng::ImageMng() {}
ImageMng::~ImageMng() {}

// Don't allow copy and move semantics
ImageMng::ImageMng(const ImageMng&) {}
ImageMng::ImageMng(ImageMng&&) noexcept {}
void ImageMng::operator=(const ImageMng&) {}
void ImageMng::operator=(ImageMng&&) noexcept {}
//
