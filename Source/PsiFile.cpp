#include "PsiFile.h"

namespace psi
{
    const uint8_t* File::NULL_BYTE = new uint8_t('\0');
    const uint32_t File::MATERIAL_CODE = *(uint32_t*)"MAT";// 0x2054414D;
    const uint32_t File::MESH_CODE = *(uint32_t*)"MESH";
    const uint32_t File::ENTITY_CODE = *(uint32_t*)"ENT";

    void File::open(std::string filePath, FileMode mode)
    {
        if (mode == FileMode::READ)
        {
            stream = fopen(filePath.c_str(), "rb");
        }
        else
        {
            stream = fopen(filePath.c_str(), "wb");
        }
    }

    void File::close()
    {
        fclose(stream);
    }

    void File::writeByteArray(const void* buffer, int size, int count)
    {
        fwrite(buffer, size, count, stream);
    }

    void File::readByteArray(void* buffer, int size, int count)
    {
        fread(buffer, size, count, stream);
    }

    void File::writeBool(const bool b)
    {
        fwrite(&b, sizeof(bool), 1, stream);
    }

    bool File::readBool()
    {
        bool b;
        fread(&b, sizeof(bool), 1, stream);
        return b;
    }

    void File::writeUnsignedChar(unsigned char c)
    {
        fwrite(&c, sizeof(unsigned char), 1, stream);
    }

    void File::writeUnsignedCharArray(std::vector<unsigned char>& c)
    {
        fwrite(c.data(), sizeof(unsigned char), c.size(), stream);
    }

    void File::writeInt(const uint32_t i)
    {
        fwrite(&i, sizeof(uint32_t), 1, stream);
    }

    uint32_t File::readInt()
    {
        uint32_t i;
        fread(&i, sizeof(uint32_t), 1, stream);
        return i;
    }

    void File::writeFloat(const float f)
    {
        fwrite(&f, sizeof(float), 1, stream);
    }

    float File::readFloat()
    {
        float f;
        fread(&f, sizeof(float), 1, stream);
        return f;
    }

    void File::writeVector2f(const Vector2f v)
    {
        fwrite(&v, sizeof(Vector2f), 1, stream);
    }

    void File::readVector2fArray(std::vector<Vector2f>& v, int count)
    {
        fread(v.data(), sizeof(Vector2f), count, stream);
    }

    void File::writeVector3f(const Vector3f v)
    {
        fwrite(&v, sizeof(Vector3f), 1, stream);
    }

    void File::readVector3f(Vector3f& v)
    {
        fread(&v, sizeof(Vector3f), 1, stream);
    }

    void File::readVector3fArray(std::vector<Vector3f>& v, int count)
    {
        fread(v.data(), sizeof(Vector3f), count, stream);
    }

    void File::writeString(const std::string s)
    {
        fwrite(s.data(), sizeof(char), s.length(), stream);
        fwrite(NULL_BYTE, sizeof(char), 1, stream);
    }

    std::string File::readString()
    {
        std::string s;
        char c;
        while (fread(&c, sizeof(char), 1, stream) && c != '\0')
            s += c;
        return s;
    }

    void File::readFaces(std::vector<Face>& faces, int count)
    {
        fread(faces.data(), sizeof(Face), count, stream);
    }

    void File::readTexture(std::unique_ptr<Texture>& tex)
    {
        tex = std::make_unique<Texture>();
        tex->width = readInt();
        tex->height = readInt();
        tex->data.resize(tex->width * tex->height * 4);
        fread(tex->data.data(), sizeof(unsigned char), tex->width * tex->height * 4, stream);
    }
}
