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
#include <QOpenGLShader>
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

    bool collides(VolumeComponent *other);

    QRectF getBounds() const;
    QVector3D getSize() const;

    void setColor(QVector4D color);

    void deleteBuffersData();

    static const QString name;

private:
    QVector<float> m_vertices;
    QVector<float> m_normals;
    QVector<unsigned int> m_indices;

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

    QOpenGLShaderProgram *shader;

    QRectF bounds;
    QVector3D size;
    QVector4D color;
};

#endif // VOLUMECOMPONENT_H
