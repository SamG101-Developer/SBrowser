#ifndef SBROWSER_CPP_KEYWORDS_HPP
#define SBROWSER_CPP_KEYWORDS_HPP

#define _FAST __fastcall
#define _INLINE __forceinline
#define _T true
#define _F false

#define TRY try {
#define CATCH_ALL } catch (...&) {
#define CATCH(exception_t) } catch(exception_t& exception) {
#define END_TRY }

#define EMPTY_CHAR '\0'

#endif //SBROWSER_CPP_KEYWORDS_HPP
