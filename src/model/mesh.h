#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QString>

#include "assimp/Importer.hpp"
#include "glfunc.h"
struct Vertex{
    QVector3D Position;
    QVector2D Normal;
    QVector2D TexCoords;
};
struct Texture{
    GLuint id;
    QString type;
    aiString path;
};
class Mesh : protected GLFUNC
{
public:
    QVector<Vertex> vertices;
    QVector<uint> indices;
    QVector<Texture> textures;

    Mesh( const   QVector<Vertex> & vertices,
          const QVector<uint> & indices,
          const QVector<Texture> & textures);
    void Draw();
private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
};

#endif // MESH_H
