#include "PsiExporter.h"

#include "PsiScene.h"

using namespace psi;

const uint8_t* nullchar = new uint8_t('\0');
const uint32_t materialCode = *(uint32_t*)"MAT";// 0x2054414D;
const uint32_t meshCode = *(uint32_t*)"MESH";
const uint32_t entityCode = *(uint32_t*)"ENT";

void writeBool(FILE* stream, const bool b)
{
    fwrite(&b, sizeof(bool), 1, stream);
}

void writeUnsignedChar(FILE* stream, unsigned char c)
{
    fwrite(&c, sizeof(unsigned char), 1, stream);
}

void writeInt(FILE* stream, const uint32_t i)
{
    fwrite(&i, sizeof(uint32_t), 1, stream);
}

void writeFloat(FILE* stream, const float f)
{
    fwrite(&f, sizeof(float), 1, stream);
}

void writeVector2f(FILE* stream, const Vector2f v)
{
    fwrite(&v, sizeof(Vector2f), 1, stream);
}

void writeVector3f(FILE* stream, const Vector3f v)
{
    fwrite(&v, sizeof(Vector3f), 1, stream);
}

void writeString(FILE* stream, const std::string s)
{
    fwrite(s.data(), sizeof(char), s.length(), stream);
    fwrite(nullchar, sizeof(char), 1, stream);
}
#include <iostream>
void PsiExporter::exportScene(std::string filePath, const Scene& scene)
{
    FILE* pFile;
    pFile = fopen(filePath.c_str(), "wb");

    fwrite(&scene.camera, sizeof(Camera), 1, pFile);

    // Write all materials to the file
    writeInt(pFile, scene.materials.size());
    for (const Material& material : scene.materials)
    {
        fwrite(&materialCode, sizeof(uint32_t), 1, pFile);
        writeString(pFile, material.name);
        writeVector3f(pFile, material.albedo);
        writeVector3f(pFile, material.emission);
        writeFloat(pFile, material.roughness);
        writeFloat(pFile, material.metalness);
        writeBool(pFile, material.hasAlbedoTexture);
        if (material.hasAlbedoTexture)
        {
            writeInt(pFile, material.albedoTex->width);
            writeInt(pFile, material.albedoTex->height);
            fwrite(material.albedoTex->data.data(), sizeof(unsigned char), material.albedoTex->data.size(), pFile);
        }
    }

    // Write all models to the file
    writeInt(pFile, scene.models.size());
    std::cout << "Num models: " << scene.models.size() << std::endl;
    for (const Model& model : scene.models)
    {
        std::cout << "Num meshes: " << model.meshes.size() << std::endl;
        writeInt(pFile, model.meshes.size());
        for (const Mesh& mesh : model.meshes)
        {
            fwrite(&meshCode, sizeof(uint32_t), 1, pFile);
            writeInt(pFile, mesh.vertices.size());
            writeInt(pFile, mesh.normals.size());
            writeInt(pFile, mesh.texCoords.size());
            writeInt(pFile, mesh.faces.size());
            std::cout << mesh.materialIndex << std::endl;
            writeInt(pFile, mesh.materialIndex);
            for (const Vector3f& v : mesh.vertices)
            {
                writeVector3f(pFile, v);
            }
            for (const Vector3f& n : mesh.normals)
            {
                writeVector3f(pFile, n);
            }
            for (const Vector2f& t : mesh.texCoords)
            {
                writeVector2f(pFile, t);
            }
            for (const Face& f : mesh.faces)
            {
                writeInt(pFile, f.i0);
                writeInt(pFile, f.i1);
                writeInt(pFile, f.i2);
            }
        }
    }

    // Write all entities to the file
    writeInt(pFile, scene.entities.size());
    for (const Entity& entity : scene.entities)
    {
        fwrite(&entityCode, sizeof(uint32_t), 1, pFile);
        writeString(pFile, entity.name);
        writeVector3f(pFile, entity.position);
        writeVector3f(pFile, entity.rotation);
        writeVector3f(pFile, entity.scale);

        writeInt(pFile, entity.modelIndex);
    }

    fclose(pFile);
}
