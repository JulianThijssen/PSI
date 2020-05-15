#pragma once

#include "PsiScene.h"

#include <string>
#include <vector>

namespace psi
{
    class File
    {
    public:
        static const uint8_t* NULL_BYTE;
        static const uint32_t MATERIAL_CODE;
        static const uint32_t MESH_CODE;
        static const uint32_t ENTITY_CODE;

        enum class FileMode
        {
            READ, WRITE
        };

        void open(std::string filePath, FileMode mode);
        void close();

        void writeByteArray(const void* buffer, int size, int count);
        void readByteArray(void* buffer, int size, int count);

        void writeBool(const bool b);
        bool readBool();

        void writeUnsignedChar(unsigned char c);
        void writeUnsignedCharArray(std::vector<unsigned char>& c);

        void writeInt(const uint32_t i);
        uint32_t readInt();

        void writeFloat(const float f);
        float readFloat();

        void writeVector2f(const Vector2f v);
        void readVector2fArray(std::vector<Vector2f>& v, int count);

        void writeVector3f(const Vector3f v);
        void readVector3f(Vector3f& v);
        void readVector3fArray(std::vector<Vector3f>& v, int count);

        void writeString(const std::string s);
        std::string readString();

        void readFaces(std::vector<Face>& faces, int count);
        void readTexture(std::unique_ptr<Texture>& tex);

    private:
        FILE* stream;
    };
}
