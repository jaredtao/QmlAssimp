#ifndef MODEL_H
#define MODEL_H

#include "glfunc.h"
#include <QOpenGLShaderProgram>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "mesh.h"
#include <vector>
#include <QUrl>
using namespace std;
class Model : public QObject, protected GLFUNC
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
public:

    void Init();
    void Draw(const QOpenGLShaderProgram & program);
    const QUrl &source() const;

public slots:
    void setSource(const QUrl &source);

signals:
    void sourceChanged(const QUrl &source);

private:
    void loadModel(const QString &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    QVector<Texture> loadMaterialTexture(aiMaterial *mat,
                                         aiTextureType type,
                                         QString typeName);
    GLint TextureFromFile(const char * path, QString directory);

    vector<Mesh> meshes;
    vector<Texture> textures_loaded;
    QString directory;
    QUrl m_source;
};

#endif // MODEL_H
