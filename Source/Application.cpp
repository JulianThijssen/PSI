#include "File.h"

#include "PsiImporter.h"
#include "PsiExporter.h"
#include "PsiScene.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "nlohmann/json_fwd.hpp"
#include "nlohmann/json.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace psi;

using json = nlohmann::json;

class Application
{
public:
    void init()
    {

    }
};

int readInt(json& element)
{
    return element.get<int>();
}

float readFloat(json& element)
{
    return element.get<float>();
}

Vector3f readVector3f(json& element)
{
    int numElements = element.size();
    if (numElements != 3)
        throw std::runtime_error("readVector3f()");// "Attempting to read Vector3f, but array has " + numElements + " elements.");

    return { element[0], element[1], element[2] };
}

std::vector<std::string> readStrings(json& element)
{
    std::vector<std::string> strings;
    int numElements = element.size();

    for (int i = 0; i < numElements; i++)
    {
        strings.push_back(element[i]);
    }
    return strings;
}

bool loadPng(std::string path, Texture& texture)
{
    int bpp;
    unsigned char* data;
    data = stbi_load(path.c_str(), &texture.width, &texture.height, &bpp, STBI_rgb_alpha);

    if (!data) {
        std::cout << "Failed to load image at: " << path << std::endl;
        return false;
    }
    std::cout << "Loaded image with width: " << texture.width << " height: " << texture.height << " bpp: " << bpp << std::endl;
    texture.data.resize(texture.width * texture.height * 4);
    std::memcpy(texture.data.data(), data, sizeof(unsigned char) * texture.width * texture.height * 4);
    delete[] data;

    return true;
}

bool convertScene(std::string inputPath, std::string outputPath)
{
    Scene scene;

    std::cout << "LOADING SCENE: " << inputPath.c_str() << std::endl;
    std::ifstream inFile;

    inFile.open(inputPath.c_str(), std::ios::in | std::ios::binary);

    if (inFile.fail()) {
        std::cout << "Failed to load file: " << inputPath << std::endl;
        return false;
    }

    std::string contents;
    try {
        contents = File::loadFile(inputPath.c_str());
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "File not found" << std::endl;
    }

    const char* cont = contents.c_str();
    std::cout << "Parsing file contents.." << std::endl;
    json j3 = json::parse(cont);
    std::cout << "Processing json.." << std::endl;
    for (json& element : j3["materials"]) {
        std::string name = element["id"].get<std::string>();
        std::string path = element["path"].get<std::string>();

        std::string line;
        std::ifstream materialFile(path);
        if (materialFile.is_open())
        {
            Material material;

            while (std::getline(materialFile, line))
            {
                std::vector<std::string> tokens;
                std::string token;

                std::istringstream is(line);
                while (std::getline(is, token, ' '))
                {
                    tokens.push_back(token);
                }

                if (tokens.empty())
                    continue;

                std::string key = tokens[0];

                if (key == "Name")
                {
                    if (tokens.size() != 2)
                    {
                        std::cout << "Missing material name" << std::endl;
                        continue;
                    }
                    material.name = tokens[1];
                    std::cout << "Name: " << material.name << std::endl;
                }
                if (key == "Albedo")
                {
                    if (tokens.size() != 4)
                    {
                        std::cout << "Wrong number of values for albedo" << std::endl;
                        continue;
                    }
                    material.albedo = Vector3f(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
                    std::cout << "Albedo: " << material.albedo.x << " " << material.albedo.y << " " << material.albedo.z << std::endl;
                }
                if (key == "Roughness")
                {
                    if (tokens.size() != 2)
                    {
                        std::cout << "Wrong number of arguments for roughness" << std::endl;
                        continue;
                    }
                    material.roughness = std::stof(tokens[1]);
                }
                if (key == "Metalness")
                {
                    if (tokens.size() != 2)
                    {
                        std::cout << "Wrong number of arguments for metalness" << std::endl;
                        continue;
                    }
                    material.metalness = std::stof(tokens[1]);
                }
                if (key == "Emission")
                {
                    if (tokens.size() != 4)
                    {
                        std::cout << "Wrong number of values for emission" << std::endl;
                        continue;
                    }
                    material.emission = Vector3f(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
                    std::cout << "Emission: " << material.emission.x << " " << material.emission.y << " " << material.emission.z << std::endl;
                }
                if (key == "AlbedoTex")
                {
                    if (tokens.size() != 2)
                    {
                        std::cout << "Wrong number of arguments for albedo texture" << std::endl;
                        continue;
                    }
                    std::string path = tokens[1];

                    material.albedoTex = std::make_unique<Texture>();
                    bool loaded = loadPng(path, *material.albedoTex);
                    material.hasAlbedoTexture = loaded;
                }
                if (key == "NormalTex")
                {
                    if (tokens.size() != 2)
                    {
                        std::cout << "Wrong number of arguments for normal texture" << std::endl;
                        continue;
                    }
                    std::string path = tokens[1];

                    material.normalTex = std::make_unique<Texture>();
                    bool loaded = loadPng(path, *material.normalTex);
                    material.hasNormalTexture = loaded;
                }
                if (key == "RoughnessTex")
                {
                    if (tokens.size() != 2)
                    {
                        std::cout << "Wrong number of arguments for roughness texture" << std::endl;
                        continue;
                    }
                    std::string path = tokens[1];

                    material.roughnessTex = std::make_unique<Texture>();
                    bool loaded = loadPng(path, *material.roughnessTex);
                    material.hasRoughnessTexture = loaded;
                }
            }
            scene.materials.push_back(std::move(material));
        }
    }
    for (json& element : j3["models"])
    {
        std::string name = element["id"].get<std::string>();
        std::string path = element["path"].get<std::string>();
        std::cout << "Model path: " << path << std::endl;

        Assimp::Importer importer;

        unsigned int flags = aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_SortByPType;
        const aiScene* aiScene = importer.ReadFile(path, flags);

        if (!aiScene)
        {
            std::cout << "Failed to load model: " << importer.GetErrorString() << std::endl;
            return false;
        }
        
        Model model;
        for (int i = 0; i < aiScene->mNumMeshes; i++)
        {
            const aiMesh* aiMesh = aiScene->mMeshes[i];

            Mesh mesh;
            mesh.vertices.resize(aiMesh->mNumVertices);
            mesh.normals.resize(aiMesh->mNumVertices);
            mesh.tangents.resize(aiMesh->mNumVertices);
            mesh.faces.resize(aiMesh->mNumFaces);

            for (int j = 0; j < aiMesh->mNumVertices; j++)
            {
                mesh.vertices[j] = Vector3f(aiMesh->mVertices[j].x, aiMesh->mVertices[j].y, aiMesh->mVertices[j].z);
                mesh.normals[j] = Vector3f(aiMesh->mNormals[j].x, aiMesh->mNormals[j].y, aiMesh->mNormals[j].z);
                mesh.tangents[j] = Vector3f(aiMesh->mTangents[j].x, aiMesh->mTangents[j].y, aiMesh->mTangents[j].z);
            }
            if (aiMesh->HasTextureCoords(0))
            {
                mesh.texCoords.resize(aiMesh->mNumVertices);
                for (int j = 0; j < aiMesh->mNumVertices; j++)
                    mesh.texCoords[j] = Vector2f(aiMesh->mTextureCoords[0][j].x, aiMesh->mTextureCoords[0][j].y);
            }
            for (int j = 0; j < aiMesh->mNumFaces; j++)
            {
                const aiFace& aiFace = aiMesh->mFaces[j];
                if (aiFace.mNumIndices < 3)
                    continue;
                mesh.faces[j] = Face{ aiFace.mIndices[0], aiFace.mIndices[1], aiFace.mIndices[2] };
            }

            aiMaterial* mat = aiScene->mMaterials[aiMesh->mMaterialIndex];
            aiString aiName;
            mat->Get(AI_MATKEY_NAME, aiName);
            std::string matName = std::string(aiName.C_Str());

            for (int m = 0; m < scene.materials.size(); m++)
            {
                const Material& material = scene.materials[m];

                if (material.name == matName)
                {
                    mesh.materialIndex = m;
                }
            }

            model.meshes.push_back(mesh);
        }
        model.name = name;
        scene.models.push_back(model);
    }

    for (json& element : j3["objects"]) {
        psi::Entity entity;

        entity.name = element["name"].get<std::string>();

        json& posElement = element["position"];
        entity.position = readVector3f(posElement);

        json& rotElement = element["rotation"];
        entity.rotation = readVector3f(rotElement);

        json& scaleElement = element["scale"];
        entity.scale = readVector3f(scaleElement);

        json& materialsElement = element["materials"];
        std::vector<std::string> materialNames = readStrings(materialsElement);

        std::cout << entity.name << std::endl;
        std::cout << entity.position.x << " " << entity.position.y << " " << entity.position.z << std::endl;
        std::cout << entity.rotation.x << " " << entity.rotation.y << " " << entity.rotation.z << std::endl;
        std::cout << entity.scale.x << " " << entity.scale.y << " " << entity.scale.z << std::endl;

        if (entity.name == "MainCamera")
        {
            json& cameraElement = element["camera"];
            json& fovElement = cameraElement["fov"];
            json& aspectElement = cameraElement["aspect"];
            json& zNearElement = cameraElement["near"];
            json& zFarElement = cameraElement["far"];
            float fov = readFloat(fovElement);
            float aspect = readFloat(aspectElement);
            float zNear = readFloat(zNearElement);
            float zFar = readFloat(zFarElement);

            scene.camera = Camera{ entity.position, entity.rotation, fov, aspect, zNear, zFar };
            std::cout << fov << " " << aspect << " " << zNear << " " << zFar << std::endl;
            continue;
        }
        else
        {
            std::string id = element["model"].get<std::string>();

            for (int m = 0; m < scene.models.size(); m++)
            {
                const Model& model = scene.models[m];

                if (model.name == id)
                {
                    entity.modelIndex = m;
                }
            }
        }

        scene.entities.push_back(entity);
    }
    
    PsiExporter exporter;
    exporter.exportScene(outputPath, scene);

    Scene checkScene;
    PsiImporter importer;
    bool importSuccess = importer.importScene(outputPath, checkScene);

    if (!importSuccess)
    {
        std::cerr << "Failed to import scene correctly" << std::endl;
        return false;
    }

    if (scene == checkScene)
    {
        std::cout << "Scene passed all checks" << std::endl;
    }
    else
    {
        std::cout << "Scene failed to export/import equally" << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char** argv)
{
    std::vector<std::string> cmdArgs(argv, argv + argc);

    std::cout << cmdArgs.size() << std::endl;
    if (cmdArgs.size() < 3)
    {
        std::cout << "Converter syntax: <converter.exe> <input.json> <output.psi>" << std::endl;
        return 0;
    }

    convertScene(cmdArgs[1], cmdArgs[2]);

    return 0;
}
