#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <tinyobj/tiny_obj_loader.h>

namespace our {

class Object {
public:
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    explicit Object(const std::string &filename);
};

} // our

#endif //OBJECT_HPP
