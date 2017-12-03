#ifndef MODEL_H
#define MODEL_H

#include "glfunc.h"
#include "Mesh.h"

#include <QUrl>
#include <vector>
#include <QOpenGLShaderProgram>
#define GLOB_MEASURE_TIME
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/LogStream.hpp"
#include "assimp/DefaultLogger.hpp"
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

    void loadScene(const aiScene * scene);
    void loadModel(const QString &path);
    //void loadNode(aiNode *node, const aiScene *scene);
    //Mesh loadMesh(aiMesh *mesh, const aiScene *scene);
    QVector<Texture> loadMaterialTexture(aiMaterial *mat,
                                         aiTextureType type,
                                         QString typeName);
    GLint TextureFromFile(const char * path, QString m_scenePath);

    vector<Mesh> m_meshes;
    QVector<Texture> m_textures;
    QString m_scenePath;
    QUrl m_source;
};
class myStream : public Assimp::LogStream

{

public:

    // Constructor

    myStream()

    {

        // empty

    }


    // Destructor

    ~myStream()

    {

        // empty

    }


    // Write womethink using your own functionality

    void write(const char* message)

    {

        qWarning() << message;

    }

};
#endif // MODEL_H
