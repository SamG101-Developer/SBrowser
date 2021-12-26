#include "cdata_section.hpp"


dom::nodes::cdata_section::cdata_section() : text() {

    node_type = CDATA_SECTION_NODE;
    node_name = "#cdata-section";
}
