#ifndef SBROWSER_ATTRIBUTE_DESCRIPTORS_HPP
#define SBROWSER_ATTRIBUTE_DESCRIPTORS_HPP

#define new_obj \
/*
 * create a new object from the method (c++ object on the stack returned)
 */

#define same_obj \
/*
 * return the same object from the method (existing c++ pointer returned, object likely on the stack)
 */

#define unscopable \
/*
 * not accessible by javascript interface (overrides a cpp_keyword decorator ie 'method')
 */

#endif //SBROWSER_ATTRIBUTE_DESCRIPTORS_HPP
