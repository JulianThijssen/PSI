#include "PsiExporter.h"

#include "PsiScene.h"

using namespace psi;

const uint8_t* nullchar = new uint8_t('\0');
const uint32_t materialCode = *(uint32_t*)"MAT";// 0x2054414D;
const uint32_t meshCode = *(uint32_t*)"MESH";

void writeInt(FILE* stream, const uint32_t i)
{
    fwrite(&i, sizeof(uint32_t), 1, stream);
}

void writeFloat(FILE* stream, const float f)
{
    fwrite(&f, sizeof(float), 1, stream);
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

void PsiExporter::exportScene(std::string filePath, const Scene& scene)
{
    FILE* pFile;
    pFile = fopen("Scene.psi", "wb");

    fwrite(&scene.camera, sizeof(Camera), 1, pFile);

    writeInt(pFile, scene.materials.size());
    for (const Material& material : scene.materials)
    {
        fwrite(&materialCode, sizeof(uint32_t), 1, pFile);
        writeVector3f(pFile, material.albedo);
        writeVector3f(pFile, material.emission);
    }

    writeInt(pFile, scene.meshes.size());
    for (const Mesh& mesh : scene.meshes)
    {
        fwrite(&meshCode, sizeof(uint32_t), 1, pFile);
        writeInt(pFile, mesh.vertices.size());
        writeInt(pFile, mesh.normals.size());
        writeInt(pFile, mesh.faces.size());
        writeInt(pFile, mesh.materialIndex);
        for (const Vector3f& v : mesh.vertices)
        {
            writeVector3f(pFile, v);
        }
        for (const Vector3f& n : mesh.normals)
        {
            writeVector3f(pFile, n);
        }
        for (const Face& f : mesh.faces)
        {
            writeInt(pFile, f.i0);
            writeInt(pFile, f.i1);
            writeInt(pFile, f.i2);
        }
    }

    fclose(pFile);
}
