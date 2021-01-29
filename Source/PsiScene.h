#pragma once

#include <string>
#include <vector>
#include <memory>

namespace psi
{
    struct Vector2f
    {
        Vector2f() : x(0), y(0) { }
        Vector2f(float x, float y) : x(x), y(y) { }
        float x, y;
    };

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

    class Texture
    {
    public:
        int width;
        int height;
        std::vector<unsigned char> data;
    };

    class Material
    {
    public:
        std::string name;
        Vector3f albedo = Vector3f(1, 1, 1);
        Vector3f emission = Vector3f(0, 0, 0);
        float roughness = 1;
        float metalness = 0;

        std::unique_ptr<Texture> albedoTex = nullptr;
        std::unique_ptr<Texture> normalTex = nullptr;
        std::unique_ptr<Texture> roughnessTex = nullptr;
        bool hasAlbedoTexture = false;
        bool hasNormalTexture = false;
        bool hasRoughnessTexture = false;
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
        std::vector<Vector2f> texCoords;
        std::vector<Vector3f> tangents;
        std::vector<Face> faces;

        int materialIndex;
    };

    class Model
    {
    public:
        std::string name;
        std::vector<psi::Mesh> meshes;
    };

    class Entity
    {
    public:
        std::string name;
        Vector3f position;
        Vector3f rotation;
        Vector3f scale;

        int modelIndex;
    };

    class Scene
    {
    public:
        Camera camera;

        std::vector<psi::Material> materials;
        std::vector<psi::Model> models;
        std::vector<psi::Entity> entities;
    };

    bool operator==(const psi::Vector2f& lhs, const psi::Vector2f& rhs);
    bool operator==(const psi::Vector3f& lhs, const psi::Vector3f& rhs);
    bool operator==(const psi::Camera& lhs, const psi::Camera& rhs);
    bool operator==(const psi::Texture& lhs, const psi::Texture& rhs);
    bool operator==(const psi::Material& lhs, const psi::Material& rhs);
    bool operator==(const psi::Face& lhs, const psi::Face& rhs);
    bool operator==(const psi::Mesh& lhs, const psi::Mesh& rhs);
    bool operator==(const psi::Model& lhs, const psi::Model& rhs);
    bool operator==(const psi::Scene& lhs, const psi::Scene& rhs);
}
