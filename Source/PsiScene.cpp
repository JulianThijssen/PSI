#include "PsiScene.h"

#include <cassert>
#include <iostream>


template <class T>
inline bool compare(const T& lhs, const T& rhs)
{
#ifdef NDEBUG
    return lhs == rhs;
#else
    if (!(lhs == rhs))
    {
        std::cerr << typeid(lhs).name() << std::endl;
        throw std::runtime_error("Left-hand side does not equal right.");
    }
    return lhs == rhs;
#endif
}

namespace psi
{
    bool operator==(const psi::Vector3f& lhs, const psi::Vector3f& rhs)
    {
        return
            compare(lhs.x, rhs.x) &&
            compare(lhs.y, rhs.y) &&
            compare(lhs.z, rhs.z);
    }

    bool operator==(const psi::Camera& lhs, const psi::Camera& rhs)
    {
        return
            compare(lhs.position, rhs.position) &&
            compare(lhs.rotation, rhs.rotation) &&
            compare(lhs.fov, rhs.fov) &&
            compare(lhs.aspect, rhs.aspect) &&
            compare(lhs.near, rhs.near) &&
            compare(lhs.far, rhs.far);
    }

    bool operator==(const psi::Material& lhs, const psi::Material& rhs)
    {
        return
            compare(lhs.albedo, rhs.albedo) &&
            compare(lhs.emission, rhs.emission);
    }

    bool operator==(const psi::Face& lhs, const psi::Face& rhs)
    {
        return
            compare(lhs.i0, rhs.i0) &&
            compare(lhs.i1, rhs.i1) &&
            compare(lhs.i2, rhs.i2);
    }

    bool operator==(const psi::Mesh& lhs, const psi::Mesh& rhs)
    {
        return
            compare(lhs.vertices, rhs.vertices) &&
            compare(lhs.normals, rhs.normals) &&
            compare(lhs.faces, rhs.faces) &&
            compare(lhs.materialIndex, rhs.materialIndex);
    }

    bool operator==(const psi::Scene& lhs, const psi::Scene& rhs)
    {
        return
            compare(lhs.camera, rhs.camera) &&
            compare(lhs.materials, rhs.materials) &&
            compare(lhs.meshes, rhs.meshes);
    }
}
