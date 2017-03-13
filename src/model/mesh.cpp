#include "mesh.h"

Mesh::Mesh(const QVector<Vertex> &vertices, const QVector<GLuint> &indices, const QVector<Texture> &textures)
{
    this->vertices = vertices;
    this->indices  = indices;
    this->textures = textures;

    setupMesh();
}

Mesh::Mesh(const Mesh &other)
{
    this->vertices = other.vertices;
    this->indices  = other.indices;
    this->textures = other.textures;

    setupMesh();
}

Mesh &Mesh::operator=(const Mesh &other)
{
    this->vertices = other.vertices;
    this->indices  = other.indices;
    this->textures = other.textures;

    setupMesh();
    return *this;
}


void Mesh::Draw(const QOpenGLShaderProgram &program)
{
    GLuint diffuseNr	= 1;
    GLuint specularNr	= 1;
    QString name;
    for (GLuint i = 0; i < this->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        if (this->textures[i].type == "texture_diffuse") {
            name = QString("material.texture_diffuse%1").arg(diffuseNr++);
        } else if (this->textures[i].type == "texture_specular") {
            name = QString("material.texture_specular%1").arg(specularNr++);
        }

        glUniform1f(glGetUniformLocation(program.programId(), name.toStdString().c_str()), i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    initializeOpenGLFunctions();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    //position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    //normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

    //texcoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
