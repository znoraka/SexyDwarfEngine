#ifndef MAPCOMPONENT_H
#define MAPCOMPONENT_H


#include <QVector>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QFile>
#include <QImage>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "engine/components/component.h"

class MapComponent : public Component
{
public:
    MapComponent();
    static Pool<MapComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    MapComponent *init(QString heightmapPath, QString texturePath);
    virtual void update(float delta);

    static const QString name;

private:
    QVector<QVector3D> verticesArray;
    QVector<QVector3D> normalsArray;
    QVector<QVector3D> colorsArray;
    QVector<GLuint> indexesArray;

    QOpenGLBuffer m_vertexbuffer;
    QOpenGLBuffer m_normalbuffer;
    QOpenGLBuffer m_colorbuffer;
    QOpenGLBuffer m_indexbuffer;

    QImage heightmap;

    QOpenGLTexture *glTexture;
    QOpenGLShaderProgram *shader;


    float getZ(float i, float j);
};
#endif // MAPCOMPONENT_H
