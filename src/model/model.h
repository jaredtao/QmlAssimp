#ifndef MODEL_H
#define MODEL_H

#include "glfunc.h"
#include "mesh.h"

#include <QUrl>
#include <vector>
#include <QOpenGLShaderProgram>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

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
    void loadNode(aiNode *node, const aiScene *scene);
    Mesh loadMesh(aiMesh *mesh, const aiScene *scene);
    QVector<Texture> loadMaterialTexture(aiMaterial *mat,
                                         aiTextureType type,
                                         QString typeName);
    GLint TextureFromFile(const char * path, QString m_scenePath);

    vector<Mesh> m_meshes;
    QVector<Texture> m_texturesLoaded;
    QString m_scenePath;
    QUrl m_source;
};

#endif // MODEL_H
