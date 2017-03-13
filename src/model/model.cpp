#include "model.h"
#include <QImage>
#include <QDebug>

void Model::Init(QString path)
{
    initializeOpenGLFunctions();
    loadModel(path);
}
void Model::Draw(const QOpenGLShaderProgram & program)
{
    for (auto & i : meshes) {
        i.Draw(program);
    }
}
void Model::loadModel(QString path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path.toStdString(),
                                             aiProcess_Triangulate |
                                             aiProcess_FlipUVs);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qDebug() << "ERROR::ASSIMP " << importer.GetErrorString();
        return ;
    }
    this->directory = path.left(path.lastIndexOf('/'));
    this->processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (GLuint i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->processMesh(mesh, scene));
    }
    for (GLuint i = 0; i < node->mNumChildren; i++) {
        this->processNode(node->mChildren[i], scene);
    }
}
Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
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
        for (const auto & j :textures_loaded) {
            if(j.path == str) {
                textures.push_back(j);
                skip = true;
                break;
            }
        }
        if(!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
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
