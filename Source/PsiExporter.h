#pragma once

#include <string>

namespace psi
{
    class Scene;
}

class PsiExporter
{
public:
    void exportScene(std::string filePath, const psi::Scene& scene);
};
