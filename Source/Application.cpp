#include "File.h"

#include "PsiImporter.h"
#include "PsiExporter.h"
#include "PsiScene.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "nlohmann/json_fwd.hpp"
#include "nlohmann/json.hpp"

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

bool loadScene(std::string sceneFile)
{
    Scene scene;

    std::cout << "LOADING SCENE" << std::endl;
    std::ifstream inFile;

    inFile.open(sceneFile.c_str(), std::ios::in | std::ios::binary);

    if (inFile.fail()) {
        std::cout << "Failed to load file: " << sceneFile << std::endl;
        return false;
    }

    std::string contents;
    try {
        contents = File::loadFile(sceneFile.c_str());
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "File not found" << std::endl;
    }

    const char* cont = contents.c_str();

    json j3 = json::parse(cont);

    for (json& element : j3["materials"]) {
        std::string name = element["id"].get<std::string>();
        std::string path = element["path"].get<std::string>();

        //MaterialDesc material(name, path);
        //std::cout << material.name << " " << material.path << std::endl;
        //scene.materials.push_back(material);
    }
    for (json& element : j3["objects"]) {
        std::string name = element["name"].get<std::string>();
        
        json& posElement = element["position"];
        Vector3f position = readVector3f(posElement);

        json& rotElement = element["rotation"];
        Vector3f rotation = readVector3f(rotElement);

        json& cameraElement = element["camera"];
        json& fovElement = cameraElement["fov"];
        json& aspectElement = cameraElement["aspect"];
        json& zNearElement = cameraElement["near"];
        json& zFarElement = cameraElement["far"];
        float fov = readFloat(fovElement);
        float aspect = readFloat(aspectElement);
        float zNear = readFloat(zNearElement);
        float zFar = readFloat(zFarElement);

        scene.camera = Camera{ Vector3f{ 278, 273, -600 }, Vector3f{ 0, 180, 0 }, fov, aspect, zNear, zFar };

        std::cout << name << " " << std::endl;
        std::cout << position.x << " " << position.y << " " << position.z << std::endl;
        std::cout << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;
        std::cout << fov << " " << aspect << " " << zNear << " " << zFar << std::endl;
    }

    scene.camera = Camera{ Vector3f{ 278, 273, -600 }, Vector3f{ 0, 180, 0 }, 60, 1, 0.1f, 2000.0f };

    Material whiteMaterial;
    whiteMaterial.albedo = Vector3f{ 0.725f, 0.71f, 0.68f };
    Material redMaterial;
    redMaterial.albedo = Vector3f{ 0.63f, 0.065f, 0.05f };
    Material greenMaterial;
    greenMaterial.albedo = Vector3f{ 0.14f, 0.45f, 0.091f };
    Material lightMaterial;
    lightMaterial.albedo = Vector3f{ 0.725f, 0.71f, 0.68f };
    lightMaterial.emission = Vector3f{ 85.0f, 60.0f, 20.0f };
    scene.materials.push_back(whiteMaterial);
    scene.materials.push_back(redMaterial);
    scene.materials.push_back(greenMaterial);
    scene.materials.push_back(lightMaterial);

    Mesh leftWall;
    leftWall.vertices.push_back(Vector3f{ 552.8f, 0, 0 });
    leftWall.vertices.push_back(Vector3f{ 549.6f, 0, 559.2f });
    leftWall.vertices.push_back(Vector3f{ 556.0f, 548.8f, 559.2f });
    leftWall.vertices.push_back(Vector3f{ 556.0f, 548.8f, 559.2f });
    leftWall.vertices.push_back(Vector3f{ 552.8f, 0, 0 });
    leftWall.vertices.push_back(Vector3f{ 556.0f, 548.8f, 0 });
    leftWall.normals.push_back(Vector3f{ -1, 0, 0 });
    leftWall.normals.push_back(Vector3f{ -1, 0, 0 });
    leftWall.normals.push_back(Vector3f{ -1, 0, 0 });
    leftWall.normals.push_back(Vector3f{ -1, 0, 0 });
    leftWall.normals.push_back(Vector3f{ -1, 0, 0 });
    leftWall.normals.push_back(Vector3f{ -1, 0, 0 });
    leftWall.faces.push_back(Face{ 0, 1, 2 });
    leftWall.faces.push_back(Face{ 3, 4, 5 });
    leftWall.materialIndex = 1;
    scene.meshes.push_back(leftWall);

    Mesh rightWall;
    rightWall.vertices.push_back(Vector3f{ 0, 0, 0 });
    rightWall.vertices.push_back(Vector3f{ 0, 0, 559.2f });
    rightWall.vertices.push_back(Vector3f{ 0, 548.8f, 559.2f });
    rightWall.vertices.push_back(Vector3f{ 0, 548.8f, 559.2f });
    rightWall.vertices.push_back(Vector3f{ 0, 0, 0 });
    rightWall.vertices.push_back(Vector3f{ 0, 548.8f, 0 });
    rightWall.normals.push_back(Vector3f{ 1, 0, 0 });
    rightWall.normals.push_back(Vector3f{ 1, 0, 0 });
    rightWall.normals.push_back(Vector3f{ 1, 0, 0 });
    rightWall.normals.push_back(Vector3f{ 1, 0, 0 });
    rightWall.normals.push_back(Vector3f{ 1, 0, 0 });
    rightWall.normals.push_back(Vector3f{ 1, 0, 0 });
    rightWall.faces.push_back(Face{ 0, 1, 2 });
    rightWall.faces.push_back(Face{ 3, 4, 5 });
    rightWall.materialIndex = 2;
    scene.meshes.push_back(rightWall);

    Mesh floor;
    floor.vertices.push_back(Vector3f{ 0, 0, 0 });
    floor.vertices.push_back(Vector3f{ 552.8f, 0, 0 });
    floor.vertices.push_back(Vector3f{ 0, 0, 559.2f });
    floor.vertices.push_back(Vector3f{ 0, 0, 559.2f });
    floor.vertices.push_back(Vector3f{ 552.8f, 0, 0 });
    floor.vertices.push_back(Vector3f{ 549.6f, 0, 559.2f });
    floor.normals.push_back(Vector3f{ 0, 1, 0 });
    floor.normals.push_back(Vector3f{ 0, 1, 0 });
    floor.normals.push_back(Vector3f{ 0, 1, 0 });
    floor.normals.push_back(Vector3f{ 0, 1, 0 });
    floor.normals.push_back(Vector3f{ 0, 1, 0 });
    floor.normals.push_back(Vector3f{ 0, 1, 0 });
    floor.faces.push_back(Face{ 0, 1, 2 });
    floor.faces.push_back(Face{ 3, 4, 5 });
    floor.materialIndex = 0;
    scene.meshes.push_back(floor);

    Mesh backWall;
    backWall.vertices.push_back(Vector3f{ 0, 0, 559.2f });
    backWall.vertices.push_back(Vector3f{ 549.6f, 0, 559.2f });
    backWall.vertices.push_back(Vector3f{ 0, 548.8f, 559.2f });
    backWall.vertices.push_back(Vector3f{ 0, 548.8f, 559.2f });
    backWall.vertices.push_back(Vector3f{ 549.6f, 0, 559.2f });
    backWall.vertices.push_back(Vector3f{ 556.0f, 548.8f, 559.2f });
    backWall.normals.push_back(Vector3f{ 0, 0, -1 });
    backWall.normals.push_back(Vector3f{ 0, 0, -1 });
    backWall.normals.push_back(Vector3f{ 0, 0, -1 });
    backWall.normals.push_back(Vector3f{ 0, 0, -1 });
    backWall.normals.push_back(Vector3f{ 0, 0, -1 });
    backWall.normals.push_back(Vector3f{ 0, 0, -1 });
    backWall.faces.push_back(Face{ 0, 1, 2 });
    backWall.faces.push_back(Face{ 3, 4, 5 });
    backWall.materialIndex = 0;
    scene.meshes.push_back(backWall);

    Mesh ceiling;
    ceiling.vertices.push_back(Vector3f{ 0, 548.8f, 0 });
    ceiling.vertices.push_back(Vector3f{ 556.0f, 548.8f, 0 });
    ceiling.vertices.push_back(Vector3f{ 0, 548.8f, 559.2f });
    ceiling.vertices.push_back(Vector3f{ 0, 548.8f, 559.2f });
    ceiling.vertices.push_back(Vector3f{ 556.0f, 548.8f, 0 });
    ceiling.vertices.push_back(Vector3f{ 556.0f, 548.8f, 559.2f });
    ceiling.normals.push_back(Vector3f{ 0, -1, 0 });
    ceiling.normals.push_back(Vector3f{ 0, -1, 0 });
    ceiling.normals.push_back(Vector3f{ 0, -1, 0 });
    ceiling.normals.push_back(Vector3f{ 0, -1, 0 });
    ceiling.normals.push_back(Vector3f{ 0, -1, 0 });
    ceiling.normals.push_back(Vector3f{ 0, -1, 0 });
    ceiling.faces.push_back(Face{ 0, 1, 2 });
    ceiling.faces.push_back(Face{ 3, 4, 5 });
    ceiling.materialIndex = 0;
    scene.meshes.push_back(ceiling);

    Mesh light;
    light.vertices.push_back(Vector3f{ 213.0f, 548.0f, 227.0f });
    light.vertices.push_back(Vector3f{ 343.0f, 548.0f, 227.0f });
    light.vertices.push_back(Vector3f{ 213.0f, 548.0f, 332.0f });
    light.vertices.push_back(Vector3f{ 213.0f, 548.0f, 332.0f });
    light.vertices.push_back(Vector3f{ 343.0f, 548.0f, 227.0f });
    light.vertices.push_back(Vector3f{ 343.0f, 548.0f, 332.0f });
    light.normals.push_back(Vector3f{ 0, -1, 0 });
    light.normals.push_back(Vector3f{ 0, -1, 0 });
    light.normals.push_back(Vector3f{ 0, -1, 0 });
    light.normals.push_back(Vector3f{ 0, -1, 0 });
    light.normals.push_back(Vector3f{ 0, -1, 0 });
    light.normals.push_back(Vector3f{ 0, -1, 0 });
    light.faces.push_back(Face{ 0, 1, 2 });
    light.faces.push_back(Face{ 3, 4, 5 });
    light.materialIndex = 3;
    scene.meshes.push_back(light);

    PsiExporter exporter;
    exporter.exportScene("ExportedScene.psi", scene);

    Scene checkScene;
    PsiImporter importer;
    bool importSuccess = importer.importScene("Scene.psi", checkScene);

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

    loadScene("Scene.json");

    return 0;
}
