#include "html_select_element.hpp"

#include <ext/infinity.hpp>


html::elements::html_select_element::html_select_element()
{
    // constrain the property values
    size.clamp_values(1, ext::infinity<ulong>());

    // create the widget representation
    auto widget = QPointer<QComboBox>{};
}
