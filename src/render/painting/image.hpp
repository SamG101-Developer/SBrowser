#pragma once
#ifndef SBROWSER_IMAGE_HPP
#define SBROWSER_IMAGE_HPP

#include <ext/decorators.hpp>
#include <QtGui/QImage>

namespace render::painting {class image;}


class render::painting::image
{
public:
    auto reset() -> void;

private cpp_properties:
    QImage* m_image;
};


#endif //SBROWSER_IMAGE_HPP
