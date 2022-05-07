#ifndef SBROWSER_CPP_KEYWORDS_HPP
#define SBROWSER_CPP_KEYWORDS_HPP

// true and false alternatives (look nicer in code)
#define _T true
#define _F false

// make small try-except blocks look nicer
#define TRY try {
#define CATCH_ALL } catch (...&) {
#define CATCH(exception_t) } catch(exception_t& exception) {
#define END_TRY }

// syntactic sugar for an empty character (char type)
#define EMPTY_CHAR '\0'

#endif //SBROWSER_CPP_KEYWORDS_HPP
