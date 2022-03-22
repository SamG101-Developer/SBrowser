#ifndef SBROWSER_DECORATORS_HPP
#define SBROWSER_DECORATORS_HPP

#define friends \
/*
 * friend classes or methods for a c++ class
 * JavaScript exposed: N/A
 * */

#define aliases \
/*
 * type aliases define with 'using ... = ...'
 * JavaScript exposed: NO
 */

#define static_constants\
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

#define methods \
/*
 * class methods
 * JavaScript exposed: YES
 */

#define internal_methods \
/*
 * class methods only ever executed by c++
 * JavaScript exposed: NO
 */

#define properties \
/*
 * class properties (can have custom getters, setters and deleters)
 * JavaScript exposed: YES
 */

#define internal_properties \
/*
 * class properties only ever executed by c++
 * javascript exposed: NO
 */

#define javascript_slot_methods \
/*
 * javascript slot methods accessible by [[slot_method]]() syntax
 * javascript exposed: YES
 */

#define javascript_slots \
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
 * javascript exposed: YES (through +, -, *, / operators etc)
 */
 
#endif //SBROWSER_DECORATORS_HPP