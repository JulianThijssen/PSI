#pragma once

#include <string>

namespace psi
{
    class Scene;
}

class PsiImporter
{
public:
    bool importScene(std::string filePath, psi::Scene& scene);
};
