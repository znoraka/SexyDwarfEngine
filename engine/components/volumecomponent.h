#ifndef VOLUMECOMPONENT_H
#define VOLUMECOMPONENT_H

#include <QVector>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QFile>

#include "engine/components/component.h"

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

    static const QString name;

private:
    QVector<QVector3D> verticesArray;
    QVector<QVector3D> normalsArray;
    QVector<QVector3D> colorsArray;
    QVector<GLuint> indexesArray;

    QOpenGLVertexArrayObject vao;

    QOpenGLBuffer m_vertexbuffer;
    QOpenGLBuffer m_normalbuffer;
    QOpenGLBuffer m_colorbuffer;
    QOpenGLBuffer m_indexbuffer;
};

#endif // VOLUMECOMPONENT_H
