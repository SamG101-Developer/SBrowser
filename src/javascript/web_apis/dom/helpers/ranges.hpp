#pragma once
#ifndef SBROWSER_RANGES_HPP
#define SBROWSER_RANGES_HPP

namespace dom {
    namespace helpers {struct ranges;}
    namespace nodes {class node;}
}


struct dom::helpers::ranges {
    static nodes::node* check_parent_exists(nodes::node* node);
};


#endif //SBROWSER_RANGES_HPP
