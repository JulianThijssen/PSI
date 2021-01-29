#include "PsiImporter.h"

#include "PsiScene.h"
#include "PsiFile.h"

using namespace psi;

bool PsiImporter::importScene(std::string filePath, Scene& scene)
{
    File file;
    file.open(filePath, File::FileMode::READ);

    file.readByteArray(&scene.camera, sizeof(Camera), 1);

    // Read all materials from file
    uint32_t numMaterials = file.readInt();
    for (uint32_t i = 0; i < numMaterials; i++)
    {
        uint32_t matCode = file.readInt();
        if (matCode != File::MATERIAL_CODE)
            return false;
        
        Material material;
        material.name = file.readString();
        file.readVector3f(material.albedo);
        file.readVector3f(material.emission);
        material.roughness = file.readFloat();
        material.metalness = file.readFloat();
        material.hasAlbedoTexture = file.readBool();
        if (material.hasAlbedoTexture)
        {
            file.readTexture(material.albedoTex);
        }
        material.hasNormalTexture = file.readBool();
        if (material.hasNormalTexture)
        {
            file.readTexture(material.normalTex);
        }
        material.hasRoughnessTexture = file.readBool();
        if (material.hasRoughnessTexture)
        {
            file.readTexture(material.roughnessTex);
        }
        scene.materials.push_back(std::move(material));
    }

    // Read all models from file
    uint32_t numModels = file.readInt();
    for (uint32_t m = 0; m < numModels; m++)
    {
        Model model;
        uint32_t numMeshes = file.readInt();
        for (uint32_t i = 0; i < numMeshes; i++)
        {
            uint32_t meshCode = file.readInt();
            if (meshCode != File::MESH_CODE)
                return false;

            Mesh mesh;

            uint32_t vertexCount = file.readInt();
            uint32_t normalCount = file.readInt();
            uint32_t texCoordCount = file.readInt();
            uint32_t tangentCount = file.readInt();
            uint32_t faceCount = file.readInt();
            mesh.materialIndex = file.readInt();

            mesh.vertices.resize(vertexCount);
            mesh.normals.resize(normalCount);
            mesh.texCoords.resize(texCoordCount);
            mesh.tangents.resize(tangentCount);
            mesh.faces.resize(faceCount);

            file.readVector3fArray(mesh.vertices, vertexCount);
            file.readVector3fArray(mesh.normals, normalCount);
            file.readVector2fArray(mesh.texCoords, texCoordCount);
            file.readVector3fArray(mesh.tangents, tangentCount);
            file.readFaces(mesh.faces, faceCount);

            model.meshes.push_back(mesh);
        }
        scene.models.push_back(model);
    }

    // Read all entities from file
    uint32_t numEntities = file.readInt();
    for (uint32_t i = 0; i < numEntities; i++)
    {
        uint32_t entityCode = file.readInt();
        if (entityCode != File::ENTITY_CODE)
            return false;

        Entity entity;
        entity.name = file.readString();
        file.readVector3f(entity.position);
        file.readVector3f(entity.rotation);
        file.readVector3f(entity.scale);

        entity.modelIndex = file.readInt();

        scene.entities.push_back(entity);
    }

    file.close();

    return true;
}
