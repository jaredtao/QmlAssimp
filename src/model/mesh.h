#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QString>
#include <QOpenGLShaderProgram>

#include "assimp/types.h"



#include "glfunc.h"
struct Vertex{
    QVector3D Position;
    QVector3D Normal;
	QVector3D Tangent;
    QVector2D TexCoords;
};
struct Texture{
    GLuint id;
    QString type;
    aiString path;
};
class Mesh : public GLFUNC
{
public:
    Mesh( const QVector<Vertex> & vertices,
          const QVector<GLuint> & indices,
          const QVector<Texture> & textures);
    Mesh(const Mesh & other);
    Mesh & operator= (const Mesh & other);

    void Draw(const QOpenGLShaderProgram & program);
private:
    void setupMesh();

    GLuint m_VAO, m_VBO, m_EBO;
    QVector<Vertex> m_vertices;
    QVector<GLuint> m_indices;
    QVector<Texture> m_textures;
};

#endif // MESH_H
