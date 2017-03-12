#ifndef MODEL_H
#define MODEL_H

#include "glfunc.h"
#include <QOpenGLShaderProgram>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "mesh.h"
#include <vector>
using namespace std;
class Model : protected GLFUNC
{
public:
    void Init(QString path);
    void Draw(const QOpenGLShaderProgram & program);
private:
    void loadModel(QString path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    QVector<Texture> loadMaterialTexture(aiMaterial *mat,
                                         aiTextureType type,
                                         QString typeName);
    GLint TextureFromFile(const char * path, QString directory);

    vector<Mesh> meshes;
    vector<Texture> textures_loaded;
    QString directory;
};

#endif // MODEL_H
