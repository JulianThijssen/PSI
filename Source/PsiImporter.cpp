#include "PsiImporter.h"

#include "PsiScene.h"

using namespace psi;

const uint8_t* NULL_BYTE = new uint8_t('\0');
const uint32_t MATERIAL_CODE = *(uint32_t*)"MAT";// 0x2054414D;
const uint32_t MESH_CODE = *(uint32_t*)"MESH";
const uint32_t ENTITY_CODE = *(uint32_t*)"ENT";

bool readBool(FILE* stream)
{
    bool b;
    fread(&b, sizeof(bool), 1, stream);
    return b;
}

uint32_t readInt(FILE* stream)
{
    uint32_t i;
    fread(&i, sizeof(uint32_t), 1, stream);
    return i;
}

float readFloat(FILE* stream)
{
    float f;
    fread(&f, sizeof(float), 1, stream);
    return f;
}

void readVector3f(FILE* stream, Vector3f& v)
{
    fread(&v, sizeof(Vector3f), 1, stream);
}

std::string readString(FILE* stream)
{
    std::string s;
    char c;
    while (fread(&c, sizeof(char), 1, stream) && c != '\0')
        s += c;
    return s;
}

void readTexture(FILE* stream, std::unique_ptr<Texture>& tex)
{
    tex = std::make_unique<Texture>();
    tex->width = readInt(stream);
    tex->height = readInt(stream);
    tex->data.resize(tex->width * tex->height * 4);
    fread(tex->data.data(), sizeof(unsigned char), tex->width * tex->height * 4, stream);
}

bool PsiImporter::importScene(std::string filePath, Scene& scene)
{
    FILE* file;
    file = fopen(filePath.c_str(), "rb");

    fread(&scene.camera, sizeof(Camera), 1, file);

    // Read all materials from file
    uint32_t numMaterials;
    fread(&numMaterials, sizeof(uint32_t), 1, file);
    for (uint32_t i = 0; i < numMaterials; i++)
    {
        uint32_t matCode;
        fread(&matCode, sizeof(uint32_t), 1, file);
        if (matCode != MATERIAL_CODE)
            return false;
        
        Material material;
        material.name = readString(file);
        readVector3f(file, material.albedo);
        readVector3f(file, material.emission);
        material.roughness = readFloat(file);
        material.metalness = readFloat(file);
        material.hasAlbedoTexture = readBool(file);
        if (material.hasAlbedoTexture)
        {
            readTexture(file, material.albedoTex);
        }
        scene.materials.push_back(std::move(material));
    }

    // Read all models from file
    uint32_t numModels = readInt(file);
    for (uint32_t m = 0; m < numModels; m++)
    {
        Model model;
        uint32_t numMeshes = readInt(file);
        for (uint32_t i = 0; i < numMeshes; i++)
        {
            uint32_t meshCode = readInt(file);
            if (meshCode != MESH_CODE)
                return false;

            Mesh mesh;

            uint32_t vertexCount = readInt(file);
            uint32_t normalCount = readInt(file);
            uint32_t texCoordCount = readInt(file);
            uint32_t faceCount = readInt(file);
            mesh.materialIndex = readInt(file);

            mesh.vertices.resize(vertexCount);
            mesh.normals.resize(normalCount);
            mesh.texCoords.resize(texCoordCount);
            mesh.faces.resize(faceCount);

            fread(mesh.vertices.data(), sizeof(Vector3f), vertexCount, file);
            fread(mesh.normals.data(), sizeof(Vector3f), normalCount, file);
            fread(mesh.texCoords.data(), sizeof(Vector2f), texCoordCount, file);
            fread(mesh.faces.data(), sizeof(Face), faceCount, file);

            model.meshes.push_back(mesh);
        }
        scene.models.push_back(model);
    }

    // Read all entities from file
    uint32_t numEntities = readInt(file);
    for (uint32_t i = 0; i < numEntities; i++)
    {
        uint32_t entityCode = readInt(file);
        if (entityCode != ENTITY_CODE)
            return false;

        Entity entity;
        entity.name = readString(file);
        readVector3f(file, entity.position);
        readVector3f(file, entity.rotation);
        readVector3f(file, entity.scale);

        entity.modelIndex = readInt(file);

        scene.entities.push_back(entity);
    }

    return true;
}
