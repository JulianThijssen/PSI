#include "PsiImporter.h"

#include "PsiScene.h"

using namespace psi;

const uint8_t* NULL_BYTE = new uint8_t('\0');
const uint32_t MATERIAL_CODE = *(uint32_t*)"MAT";// 0x2054414D;
const uint32_t MESH_CODE = *(uint32_t*)"MESH";

bool PsiImporter::importScene(std::string filePath, Scene& scene)
{
    FILE* file;
    file = fopen(filePath.c_str(), "rb");

    fread(&scene.camera, sizeof(Camera), 1, file);

    uint32_t numMaterials;
    fread(&numMaterials, sizeof(uint32_t), 1, file);
    for (uint32_t i = 0; i < numMaterials; i++)
    {
        uint32_t matCode;
        fread(&matCode, sizeof(uint32_t), 1, file);
        if (matCode != MATERIAL_CODE)
            return false;
        
        Material material;
        fread(&material, sizeof(Material), 1, file);
        scene.materials.push_back(material);
    }

    uint32_t numMeshes;
    fread(&numMeshes, sizeof(uint32_t), 1, file);
    for (uint32_t i = 0; i < numMeshes; i++)
    {
        uint32_t meshCode;
        fread(&meshCode, sizeof(uint32_t), 1, file);
        if (meshCode != MESH_CODE)
            return false;

        Mesh mesh;
        uint32_t vertexCount, normalCount, faceCount;
        fread(&vertexCount, sizeof(vertexCount), 1, file);
        fread(&normalCount, sizeof(normalCount), 1, file);
        fread(&faceCount, sizeof(faceCount), 1, file);
        fread(&mesh.materialIndex, sizeof(uint32_t), 1, file);
        mesh.vertices.resize(vertexCount);
        mesh.normals.resize(normalCount);
        mesh.faces.resize(faceCount);
        
        fread(mesh.vertices.data(), sizeof(Vector3f), vertexCount, file);
        fread(mesh.normals.data(), sizeof(Vector3f), normalCount, file);
        fread(mesh.faces.data(), sizeof(Face), faceCount, file);

        scene.meshes.push_back(mesh);
    }
    return true;
}
