#pragma once
#ifndef SBROWSER_SECURITY_INFRASTRUCTURE_HPP
#define SBROWSER_SECURITY_INFRASTRUCTURE_HPP

#include <dom_object.hpp>

namespace html::mixins {template <typename T> class security_infrastructure;}


template <typename T>
class html::mixins::security_infrastructure : public dom_object
{
public constructors:
    security_infrastructure();
};


#endif //SBROWSER_SECURITY_INFRASTRUCTURE_HPP
