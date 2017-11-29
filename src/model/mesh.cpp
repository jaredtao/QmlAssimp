#include "Mesh.h"

Mesh::Mesh(const QVector<Vertex> &vertices, const QVector<GLuint> &indices, const QVector<Texture> &textures)
    : m_vertices(vertices), m_indices(indices), m_textures(textures)
{
    setupMesh();
}

Mesh::Mesh(const Mesh &other)
{
    m_vertices = other.m_vertices;
    m_indices  = other.m_indices;
    m_textures = other.m_textures;

    setupMesh();
}

Mesh &Mesh::operator=(const Mesh &other)
{
    m_vertices = other.m_vertices;
    m_indices  = other.m_indices;
    m_textures = other.m_textures;

    setupMesh();
    return *this;
}

void Mesh::Draw(const QOpenGLShaderProgram &program)
{
    GLuint diffuseNr	= 1;
    GLuint specularNr	= 1;
    QString name;
    for (int i = 0; i < m_textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        if (m_textures[i].type == "texture_diffuse") {
            name = QString("material.texture_diffuse%1").arg(diffuseNr++);
        } else if (m_textures[i].type == "texture_specular") {
            name = QString("material.texture_specular%1").arg(specularNr++);
        }

        glUniform1f(glGetUniformLocation(program.programId(), name.toStdString().c_str()), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    initializeOpenGLFunctions();
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

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
