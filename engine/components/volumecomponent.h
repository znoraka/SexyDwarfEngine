#ifndef VOLUMECOMPONENT_H
#define VOLUMECOMPONENT_H

#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QFile>
#include <QFileInfo>
#include <QMatrix4x4>
#include <QTemporaryFile>
#include <QSharedPointer>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//#include <assimp/Importer.hpp>

#include "engine/components/component.h"

struct MaterialInfo
{
    QString Name;
    QVector3D Ambient;
    QVector3D Diffuse;
    QVector3D Specular;
    float Shininess;
};

struct LightInfo
{
    QVector4D Position;
    QVector3D Intensity;
};

struct Mesh
{
    QString name;
    unsigned int indexCount;
    unsigned int indexOffset;
    QSharedPointer<MaterialInfo> material;
};

struct Node
{
    QString name;
    QMatrix4x4 transformation;
    QVector<QSharedPointer<Mesh> > meshes;
    QVector<Node> nodes;
};

class VolumeComponent : public Component
{
public:
    VolumeComponent();
    static Pool<VolumeComponent *> *pool;
    const QString componentName() const override;
    const QString systemName() const override;
    virtual System *instantiateSystem();

    void release() override;

    VolumeComponent *init(QString filePath);
    virtual void update(float delta);
    VolumeComponent *clone();

    bool clicked(Qt::MouseButton button, int x, int y);

    QRectF getBounds() const;
    QVector3D getSize() const;

    static const QString name;

private:
//    QSharedPointer<MaterialInfo> processMaterial(aiMaterial *material);
//    QSharedPointer<Mesh> processMesh(aiMesh *mesh);
//    void processNode(const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode);

//    void transformToUnitCoordinates();
//    void findObjectDimensions(Node *node, QMatrix4x4 transformation, QVector3D &minDimension, QVector3D &maxDimension);

    QVector<float> m_vertices;
    QVector<float> m_normals;
    QVector<unsigned int> m_indices;

//    QVector<QSharedPointer<MaterialInfo> > m_materials;
//    QVector<QSharedPointer<Mesh> > m_meshes;
//    QSharedPointer<Node> m_rootNode;

    QVector<QVector3D> verticesArray;
    QVector<QVector3D> normalsArray;
    QVector<QVector3D> colorsArray;
    QVector<GLuint> indexesArray;

    QOpenGLVertexArrayObject vao;

    int posAttr, colAttr, normalAttr;

    QOpenGLBuffer m_vertexbuffer;
    QOpenGLBuffer m_normalbuffer;
    QOpenGLBuffer m_colorbuffer;
    QOpenGLBuffer m_indexbuffer;

    QRectF bounds;
    QVector3D size;
};

#endif // VOLUMECOMPONENT_H
