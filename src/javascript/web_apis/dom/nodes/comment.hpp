#pragma once
#ifndef SBROWSER_COMMENT_HPP
#define SBROWSER_COMMENT_HPP

#include <dom/nodes/character_data.hpp>

namespace dom::nodes {class comment;}


/*
 * https://dom.spec.whatwg.org/#interface-comment
 * https://developer.mozilla.org/en-US/docs/Web/API/Comment
 *
 * The Comment interface represents textual notations within markup; although it is generally not visually shown, such
 * comments are available to be read in the source view.
 *
 * Comments are represented in HTML and XML as content between '<!--' and '-->'. In XML, like inside SVG or MathML
 * markup, the character sequence '--' cannot be used within a comment.
 */
class dom::nodes::comment : public character_data
{
public: constructors
    comment(const ext::string& new_data = "");

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_COMMENT_HPP
