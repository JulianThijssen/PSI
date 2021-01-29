#include "PsiExporter.h"

#include "PsiScene.h"
#include "PsiFile.h"

#include <iostream>

using namespace psi;

void writeMaterial(File& file, const Material& material)
{
    file.writeInt(File::MATERIAL_CODE);
    file.writeString(material.name);
    file.writeVector3f(material.albedo);
    file.writeVector3f(material.emission);
    file.writeFloat(material.roughness);
    file.writeFloat(material.metalness);
    file.writeBool(material.hasAlbedoTexture);
    if (material.hasAlbedoTexture)
    {
        file.writeInt(material.albedoTex->width);
        file.writeInt(material.albedoTex->height);
        file.writeUnsignedCharArray(material.albedoTex->data);
    }
    file.writeBool(material.hasNormalTexture);
    if (material.hasNormalTexture)
    {
        file.writeInt(material.normalTex->width);
        file.writeInt(material.normalTex->height);
        file.writeUnsignedCharArray(material.normalTex->data);
    }
    file.writeBool(material.hasRoughnessTexture);
    if (material.hasRoughnessTexture)
    {
        file.writeInt(material.roughnessTex->width);
        file.writeInt(material.roughnessTex->height);
        file.writeUnsignedCharArray(material.roughnessTex->data);
    }
}

void writeModel(File& file, const Model& model)
{
    std::cout << "Num meshes: " << model.meshes.size() << std::endl;
    file.writeInt(model.meshes.size());
    for (const Mesh& mesh : model.meshes)
    {
        file.writeInt(File::MESH_CODE);
        file.writeInt(mesh.vertices.size());
        file.writeInt(mesh.normals.size());
        file.writeInt(mesh.texCoords.size());
        file.writeInt(mesh.tangents.size());
        file.writeInt(mesh.faces.size());
        std::cout << "Material index: " << mesh.materialIndex << std::endl;
        file.writeInt(mesh.materialIndex);
        for (const Vector3f& v : mesh.vertices)
        {
            file.writeVector3f(v);
        }
        for (const Vector3f& n : mesh.normals)
        {
            file.writeVector3f(n);
        }
        for (const Vector2f& t : mesh.texCoords)
        {
            file.writeVector2f(t);
        }
        for (const Vector3f& t : mesh.tangents)
        {
            file.writeVector3f(t);
        }
        for (const Face& f : mesh.faces)
        {
            file.writeInt(f.i0);
            file.writeInt(f.i1);
            file.writeInt(f.i2);
        }
    }
}

void writeEntity(File& file, const Entity& entity)
{
    file.writeInt(File::ENTITY_CODE);
    file.writeString(entity.name);
    file.writeVector3f(entity.position);
    file.writeVector3f(entity.rotation);
    file.writeVector3f(entity.scale);

    file.writeInt(entity.modelIndex);
}

void PsiExporter::exportScene(std::string filePath, const Scene& scene)
{
    File file;
    file.open(filePath, File::FileMode::WRITE);

    file.writeByteArray(&scene.camera, sizeof(Camera), 1);

    // Write all materials to the file
    file.writeInt(scene.materials.size());
    for (const Material& material : scene.materials)
        writeMaterial(file, material);

    // Write all models to the file
    file.writeInt(scene.models.size());

    for (const Model& model : scene.models)
        writeModel(file, model);

    // Write all entities to the file
    file.writeInt(scene.entities.size());
    for (const Entity& entity : scene.entities)
        writeEntity(file, entity);

    file.close();
}
