#pragma once

#include <string>
#include <vector>

namespace psi
{
    struct Vector3f
    {
        Vector3f() : x(0), y(0), z(0) { }
        Vector3f(float x, float y, float z) : x(x), y(y), z(z) { }
        float x, y, z;
    };

    struct Camera
    {
        Vector3f position;
        Vector3f rotation;
        float fov;
        float aspect;
        float near;
        float far;
    };

    class Material
    {
    public:
        Vector3f albedo;
        Vector3f emission;
    };

    struct Face
    {
        unsigned int i0, i1, i2;
    };

    class Mesh
    {
    public:
        std::vector<Vector3f> vertices;
        std::vector<Vector3f> normals;
        std::vector<Face> faces;

        int materialIndex;
    };

    class Scene
    {
    public:
        Camera camera;

        std::vector<psi::Material> materials;
        std::vector<psi::Mesh> meshes;
    };

    bool operator==(const psi::Vector3f& lhs, const psi::Vector3f& rhs);
    bool operator==(const psi::Camera& lhs, const psi::Camera& rhs);
    bool operator==(const psi::Material& lhs, const psi::Material& rhs);
    bool operator==(const psi::Face& lhs, const psi::Face& rhs);
    bool operator==(const psi::Mesh& lhs, const psi::Mesh& rhs);
    bool operator==(const psi::Scene& lhs, const psi::Scene& rhs);
}