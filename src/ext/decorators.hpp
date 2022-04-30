#ifndef SBROWSER_DECORATORS_HPP
#define SBROWSER_DECORATORS_HPP

#define enforce_range /* throw an error if the number is out of range */

#define JS_BLOCK_START {
#define JS_BLOCK_END }


#define friends \
/*
 * friend classes, structs, methods or subroutines for a c++ class
 * JavaScript exposed: N/A
 * */

#define aliases \
/*
 * type aliases define with 'using ... = ...'
 * JavaScript exposed: NO
 */

#define static_constants \
/*
 * static constants for a class
 * JavaScript exposed: YES
 * */

#define enums \
/*
 * enumerations scoped by the class
 * JavaScript exposed: NO
 */

#define constructors \
/*
 * constructors (args, copy, move, =copy, =move) and the destructor
 * JavaScript exposed: SOMETIMES
 */

#define js_methods \
/*
 * class methods
 * JavaScript exposed: YES
 */

#define cpp_methods \
/*
 * class methods only ever executed by c++
 * JavaScript exposed: NO
 */

#define js_properties \
/*
 * class properties (can have custom getters, setters and deleters)
 * JavaScript exposed: YES
 */

#define cpp_properties \
/*
 * class properties only ever accessed by c++
 * javascript exposed: NO
 */

#define js_slot_methods \
/*
 * javascript slot methods accessible by [[slot_method]]() syntax
 * javascript exposed: YES
 */

#define js_slots \
/*
 * javascript slots accessible by [[slot_name]] syntax
 * javascript exposed: YES
 */

#define accessors \
/*
 * getter, setter and deleter methods that are attached to the properties
 * javascript exposed: NO (implicitly via the property but not explicitly callable)
 */

#define operators \
/*
 * overridden operators
 * javascript exposed: YES (through +, -, *, / operators etc) (but not overridable / explicitly callable)
 */

#define cpp_static_methods
 
#endif //SBROWSER_DECORATORS_HPP
