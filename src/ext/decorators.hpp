#ifndef SBROWSER_DECORATORS_HPP
#define SBROWSER_DECORATORS_HPP

#define enforce_range /* throw an error if the number is out of range */


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

#define js_static_constants \
/*
 * static constants for a class
 * JavaScript exposed: YES
 */

#define js_methods \
/*
 * class methods
 * JavaScript exposed: YES
 */

#define js_properties \
/*
 * class properties (can have custom getters, setters and deleters)
 * JavaScript exposed: YES
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

#define cpp_static_methods
/*
 * static class methods only ever executed by c++
 * javascript exposed: NO
 */

#define cpp_methods \
/*
 * class methods only ever executed by c++
 * JavaScript exposed: NO
 */

#define cpp_properties \
/*
 * class properties only ever accessed by c++
 * javascript exposed: NO
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

#define nested_class \
/*
 * nested class (private)
 * javascript exposed: NO
 */

#endif //SBROWSER_DECORATORS_HPP
