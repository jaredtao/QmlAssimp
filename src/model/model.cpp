#include "Model.h"
#include <QImage>
#include <QDebug>
#include <QQmlFile>
#include <QFileInfo>
#include <QElapsedTimer>
void Model::Init()
{
#ifdef SHOW_TIME_COST
    QElapsedTimer time;
    time.start();
#endif
    initializeOpenGLFunctions();
    auto str = m_source.toString();
    if (str.startsWith("qrc:/")) {
        str.remove("qrc:/");
    }
    loadModel(str);
#ifdef SHOW_TIME_COST
    auto cost = time.elapsed();
    qWarning() << "model load cost" << cost;
#endif
}
void Model::Draw(const QOpenGLShaderProgram & program)
{
    }
}

void Model::setSource(const QUrl &source)
{
        return;

    emit sourceChanged(m_source);
}

const QUrl &Model::source() const
{
    return m_source;
}
void Model::loadModel(const QString & path)
{
	QElapsedTimer time;
	time.start();
	Assimp::Importer importer;
	// SET THIS TO REMOVE POINTS AND LINES -> HAVE ONLY TRIANGLES
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
	// type and aiProcess_Triangulate discompose polygons with more than 3 points in triangles
	// aiProcess_SortByPType makes sure that meshes data are triangles
	const aiScene *scene = importer.ReadFile(path.toStdString(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		qDebug() << "ERROR::ASSIMP " << importer.GetErrorString();
		return;
	}
	qWarning() << "Importer readFile cost" << time.elapsed();
	m_scenePath = QFileInfo(path).absolutePath();

	loadScene(scene);
}

void Model::loadScene(const aiScene * scene)
{

	for (auto i = 0; i < scene->mNumMeshes; ++i) {
		const auto * mesh = scene->mMeshes[i];
		QVector<Vertex> vertices;
		QVector<GLuint> indices;
		QVector<Texture> textures;
		//load  vertex
		const aiVector3D zeroVector(0.0f, 0.0f, 0.0f);
		Vertex ver;
		for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
			const auto pPos = &(mesh->mVertices[j]);
			const auto pNormal = &(mesh->mNormals[j]);
			const auto pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][j]) : &zeroVector;
			const auto pTangent = mesh->HasTangentsAndBitangents() ? &(mesh->mTangents[j]) : &zeroVector;
			ver.Position = QVector3D(pPos->x, pPos->y, pPos->z);
			ver.Normal = QVector3D(pNormal->x, pNormal->y, pNormal->z);
			ver.TexCoords = QVector2D(pTexCoord->x, pTexCoord->y);
			ver.Tangent = QVector3D(pTangent->x, pTangent->y, pTangent->z);
			vertices.push_back(ver);
		}
		//load indices
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
			const auto & face = mesh->mFaces[j];
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
		//load texture
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			QVector<Texture> diffuseMaps = this->loadMaterialTexture(material,
				aiTextureType_DIFFUSE,
				"texture_diffuse");
			for (const auto & d : diffuseMaps) {
				textures.append(d);
			}
			QVector<Texture> specularMaps = this->loadMaterialTexture(material,
				aiTextureType_SPECULAR,
				"texture_specular");
			for (const auto & s : specularMaps) {
				textures.append(s);
			}
		}
		m_meshes.push_back(Mesh(vertices, indices, textures));
	}
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
		for (const auto & j : m_textures) {
			if (j.path == str) {
				textures.push_back(j);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			QElapsedTimer time;
			time.start();
			texture.id = TextureFromFile(str.C_Str(), this->m_scenePath);
			qWarning() << "load texture cost " << time.elapsed() << str.C_Str();
			texture.type = typeName;
			textures.push_back(texture);
		}
	}
	return textures;
}

GLint Model::TextureFromFile(const char * path, QString directory)
{
	QString name = path;
	name = directory + "/" + name;
	GLuint textureID;
	glGenTextures(1, &textureID);
	QElapsedTimer time;
	time.start();
	QImage image(name);
	qWarning() << "QImage load image" << path << " cost:" << time.elapsed();
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
		0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}
