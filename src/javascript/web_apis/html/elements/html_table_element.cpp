#include "html_table_element.hpp"

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_algorithms.hpp>

#include <html/elements/html_table_caption_element.hpp>
#include <html/elements/html_table_section_element.hpp>
#include <html/elements/html_table_row_element.hpp>

#include <html/helpers/table_internals.hpp>


html::elements::html_table_element::html_table_element() : html_element()
{
    // set the custom accessors
    caption.getter = [this] {return get_caption();};
    t_head.getter = [this] {return get_t_head();};
    t_foot.getter = [this] {return get_t_foot();};
    rows.getter = [this] {return get_rows();};

    caption.setter = [this](auto && PH1) {return set_caption(std::forward<decltype(PH1)>(PH1)); };
    t_head.setter = [this](auto && PH1) {return set_t_head(std::forward<decltype(PH1)>(PH1)); };
    t_foot.setter = [this](auto && PH1) {return set_t_foot(std::forward<decltype(PH1)>(PH1)); };
}


auto html::elements::html_table_element::create_caption()
        -> html_table_caption_element*
{
    // return the first caption in the children if there is one, otherwise a newly created table caption element
    return caption = caption ? caption : helpers::table_internals::table_create<html_table_caption_element>(owner_document, "caption");
}


auto html::elements::html_table_element::create_t_head()
    -> html_table_section_element*
{
    // return the first head element in the children if there is one, otherwise a newly created head element
    return t_head = t_head ? t_head : helpers::table_internals::table_create<html_table_section_element>(owner_document, "thead");
}


auto html::elements::html_table_element::create_t_foot()
        -> html_table_section_element*
{
    // return the first foot element in the children if there is one, otherwise a newly created foot element
    return t_foot = t_foot ? t_foot : helpers::table_internals::table_create<html_table_section_element>(owner_document, "tfoot");
}


auto html::elements::html_table_element::create_t_body()
        -> html_table_section_element*
{
    // create a new body element, and insert it after the last body element (or append it if there aren't any)
    auto* t_body_element = helpers::table_internals::table_create<html_table_section_element>(owner_document, "tbody");
    not t_bodies->empty()
            ? child_nodes->insert(t_body_element, child_nodes->find(t_bodies->back()))
            : child_nodes->append(t_body_element);

    // return the newly created body element
    return t_body_element;
}


auto html::elements::html_table_element::insert_t_row(
        const long index)
        -> html_table_row_element*
{
    // if the index is outside -1 and the number of rows, throw an index error
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "Index must be between -1 and the rows.length (inclusive)",
            [this, index] {return index <= -1 or index > rows->length();});

    // create the row element to insert
    auto* row = helpers::table_internals::table_create<html_table_row_element    >(owner_document, "tr");

    // if there are no rows and no body elements
    if (rows->empty() and t_bodies->empty())
    {
        // create a body element
        auto* body = helpers::table_internals::table_create<html_table_section_element>(owner_document, "tbody");

        // append the row to the body, the body to this table
        dom::helpers::mutation_algorithms::append(row, body);
        dom::helpers::mutation_algorithms::append(body, this);
    }

    else if (rows->empty())
        // append the row to the last body
        dom::helpers::mutation_algorithms::append(row, t_bodies->back());

    else if (index == -1 or index == rows->length())
        // append the row to the parent of the last row
        dom::helpers::mutation_algorithms::append(row, rows->back()->parent);

    else
        // insert the row to rows in this parent
        rows->insert(row, index);

    // return the row
    return row;
}


auto html::elements::html_table_element::delete_caption()
        -> void
{
    // remove the first popped caption element of the table if there is one
    if (auto caption_children = children->cast_all<html_table_caption_element*>(); not caption_children.empty())
        child_nodes->remove(caption_children.front());
}


auto html::elements::html_table_element::delete_t_head()
        -> void
{
    // remove the first head element in the children if there is one
    if (auto t_head_children = children->filter([](dom::nodes::element* element) {return element->local_name == "thead";}); not t_head_children.empty())
        child_nodes->remove(t_head_children.front());
}


auto html::elements::html_table_element::delete_t_foot()
        -> void
{
    // remove the first foot element in the children if there is one
    if (auto t_foot_children = children->filter([](dom::nodes::element* element) {return element->local_name == "tfoot";}); not t_foot_children.empty())
        child_nodes->remove(t_foot_children.front());
}


auto html::elements::html_table_element::delete_t_row(
        long index)
        -> void
{
    // if the index is outside -1 and the number of rows, throw an index error
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "Index must be between -1 and the rows.length (inclusive)",
            [this, index] {return index <= -1 or index > rows->length();});

    // pop the element from the list and return it
    rows->pop(index);
}


auto html::elements::html_table_element::get_caption() const
        -> html_table_caption_element*
{
    auto caption_children = child_nodes->cast_all<html_table_caption_element*>();
    return caption_children.empty() ? nullptr : caption_children.front();
}


auto html::elements::html_table_element::get_t_head() const
        -> html_table_section_element*
{
    auto t_head_children = children->cast_all<html_table_section_element*>().filter([](dom::nodes::element* element) {return element->local_name == "thead";});
    return t_head_children.empty() ? nullptr : t_head_children.front();
}


auto html::elements::html_table_element::get_t_foot() const
        -> html_table_section_element*
{
    auto t_foot_children = children->cast_all<html_table_section_element*>().filter([](dom::nodes::element* element) {return element->local_name == "tfoot";});
    return t_foot_children.empty() ? nullptr : t_foot_children.front();
}


auto html::elements::html_table_element::get_rows() const
        -> ext::vector<dom::nodes::element*>*
{
    auto* rows_vector = new ext::vector<dom::nodes::element*>{};

    auto is_row_element = [](dom::nodes::element* element) {return element->local_name == "tr";};

    auto t_head_children = t_head->children;
    auto t_body_children = t_bodies->transform([](dom::nodes::element* element) {return element->children;}).flatten();
    auto t_foot_children = t_foot->children;

    rows_vector // TODO : sorting by tree order in groups (children and body children together)
            ->reserve(children->length() + t_head_children->length() + t_body_children.length() + t_foot_children->length())
            .extend(t_head_children->filter(&is_row_element))
            .extend(children->filter(&is_row_element))
            .extend(t_body_children.filter(&is_row_element))
            .extend(t_foot_children->filter(&is_row_element))
            .shrink_to_fit();
}


auto html::elements::html_table_element::set_caption(
        html_table_caption_element* val)
        -> void
{
    if (caption) dom::helpers::mutation_algorithms::remove(caption);

    if (val)
        child_nodes->prepend(val);
}


auto html::elements::html_table_element::set_t_head(
        html_table_section_element* val)
        -> void
{
    // check that the new head is a head
    dom::helpers::exceptions::throw_v8_exception<HIERARCHY_REQUEST_ERR>(
            "new thead element must be a thead element",
            [val] {return val and val->local_name != "thead";});

    // remove the head if it exists
    if (t_head) dom::helpers::mutation_algorithms::remove(t_head);

    if (val)
    {
        // insert the new head before any other caption or col elements in the children
        auto first_caption_element = children->find(children->filter([](dom::nodes::element* element) {return element->local_name == "caption";}).front());
        auto first_col_element     = children->find(children->filter([](dom::nodes::element* element) {return element->local_name == "col";}).front());
        child_nodes->insert(val, std::min(first_caption_element, first_col_element));
    }
}


auto html::elements::html_table_element::set_t_foot(
        html_table_section_element* val)
        -> void
{
    // check that the new foot is a foot
    dom::helpers::exceptions::throw_v8_exception<HIERARCHY_REQUEST_ERR>(
            "new tfoot element must be a tfoot element",
            [val] {return val and val->local_name != "tfoot";});

    // remove the foot if it exists
    if (t_foot) dom::helpers::mutation_algorithms::remove(t_foot);

    if (val)
        // append the new foot to the end of the children in the table
        child_nodes->append(val);
}
