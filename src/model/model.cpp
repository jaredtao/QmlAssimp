#include "model.h"
#include <QImage>
#include <QDebug>
#include <QQmlFile>
#include <QFileInfo>
#include <QElapsedTimer>
void Model::Init()
{
    QElapsedTimer time;
    time.start();
    initializeOpenGLFunctions();
    auto str = m_source.toString();
    if (str.startsWith("qrc:/")) {
        str.remove("qrc:/");
    }
    loadModel(str);
    auto cost = time.elapsed();
    qWarning() << "model load cost" << cost;
}
void Model::Draw(const QOpenGLShaderProgram & program)
{
    int count = m_meshes.size();
    for (int i = 0; i < count; ++i) {
        m_meshes[i].Draw(program);
    }
}

void Model::setSource(const QUrl &source)
{
    if (m_source == source)
        return;

    m_source = source;
    emit sourceChanged(m_source);
}

const QUrl &Model::source() const
{
    return m_source;
}
void Model::loadModel(const QString & path)
{
    Assimp::Importer importer;
     // SET THIS TO REMOVE POINTS AND LINES -> HAVE ONLY TRIANGLES
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
    // type and aiProcess_Triangulate discompose polygons with more than 3 points in triangles
    // aiProcess_SortByPType makes sure that meshes data are triangles
    const aiScene *scene = importer.ReadFile(path.toStdString(),
                                             aiProcess_CalcTangentSpace       |
                                             aiProcess_Triangulate            |
                                             aiProcess_JoinIdenticalVertices  |
                                             aiProcess_SortByPType);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qDebug() << "ERROR::ASSIMP " << importer.GetErrorString();
        return ;
    }
    m_scenePath = QFileInfo(path).absolutePath();

    loadNode(scene->mRootNode, scene);
}
void Model::loadNode(aiNode *node, const aiScene *scene)
{
    //add Meshes to node
    for (GLuint i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(loadMesh(mesh, scene));
    }
    //add child Meshes to node
    for (GLuint i = 0; i < node->mNumChildren; i++) {
        loadNode(node->mChildren[i], scene);
    }
}
Mesh Model::loadMesh(aiMesh *mesh, const aiScene *scene)
{
    QVector<Vertex>		vertices;
    QVector<GLuint>		indices;
    QVector<Texture>	textures;
    Vertex vertex;
    QVector3D vector;
    for (GLuint i = 0; i <mesh->mNumVertices; i++) {

        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.Position = vector;

        vector.setX(mesh->mNormals[i].x);
        vector.setY(mesh->mNormals[i].y);
        vector.setZ(mesh->mNormals[i].z);
        vertex.Normal = vector;

        if(mesh->mTextureCoords[0]) {
            QVector2D vector;
            vector.setX(mesh->mTextureCoords[0][i].x);
            vector.setY(mesh->mTextureCoords[0][i].y);
            vertex.TexCoords = vector;
        } else {
            vertex.TexCoords = QVector2D(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    for (GLuint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (GLuint j =0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        QVector<Texture> diffuseMaps = this->loadMaterialTexture(material,
                                                                 aiTextureType_DIFFUSE,
                                                                 "texture_diffuse");
        for (const auto & d : diffuseMaps) {
            textures.append(d);
        }
        //		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        QVector<Texture> specularMaps = this->loadMaterialTexture(material,
                                                                  aiTextureType_SPECULAR,
                                                                  "texture_specular");
        for (const auto & s: specularMaps) {
            textures.append(s);
        }
        //		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
     return Mesh(vertices, indices, textures);
}
QVector<Texture> Model::loadMaterialTexture(aiMaterial *mat,
                                     aiTextureType type,
                                     QString typeName)
{
    QVector<Texture> textures;
    aiString str;
    for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
        mat->GetTexture(type, i, &str);
        GLboolean skip = false;
        for (const auto & j :m_texturesLoaded) {
            if(j.path == str) {
                textures.push_back(j);
                skip = true;
                break;
            }
        }
        if(!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->m_scenePath);
            texture.type = typeName;
            textures.push_back(texture);
        }
    }
    return textures;
}
GLint Model::TextureFromFile(const char * path, QString directory)
{
    QString name = path;
    name =directory + "/" + name;
    GLuint textureID;
    glGenTextures(1, &textureID);
    QImage image(name);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}
