set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(IO_LIBRARY_SOURCES
    ${DIR}/PsiFile.h
    ${DIR}/PsiFile.cpp
    ${DIR}/PsiScene.h
    ${DIR}/PsiScene.cpp
    ${DIR}/PsiImporter.h
    ${DIR}/PsiImporter.cpp
    ${DIR}/PsiExporter.h
    ${DIR}/PsiExporter.cpp
    PARENT_SCOPE
)

set(IO_LIBRARY_PUBLIC_HEADERS
    ${DIR}/PsiScene.h
    ${DIR}/PsiImporter.h
    ${DIR}/PsiExporter.h
    PARENT_SCOPE
)

set(IO_TEST_SOURCES
    ${DIR}/Application.cpp
    PARENT_SCOPE
)
