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
#include <QOpenGLVertexArrayObject>
#include <QSet>
#include <QtCore/qmath.h>

#include "engine/components/component.h"
#include "engine/tools/delaunay.h"

class MapComponent : public Component
{
#define ROTL10(x) (((x) << 10) | (((x) >> 22) & 0x000000ff))
#define ROTL20(x) (((x) << 20) | (((x) >> 12) & 0x0000ffff))
public:
    MapComponent();
    static Pool<MapComponent *> *pool;
    const QString componentName() const override;

    void release() override;

    MapComponent *init(QString mapFolder);
    virtual void update(float delta);
    MapComponent *clone();

    static const QString name;
    float getZ(float i, float j);

private:
    QSet<QVector3D> verticesSet;

    QVector<QVector3D> verticesArray;
    QVector<QVector3D> normalsArray;
    QVector<QVector3D> colorsArray;
    QVector<GLuint> indexesArray;

    QOpenGLVertexArrayObject vao;

    QOpenGLBuffer m_vertexbuffer;
    QOpenGLBuffer m_normalbuffer;
    QOpenGLBuffer m_colorbuffer;
    QOpenGLBuffer m_indexbuffer;

    QImage heightmap;

    QOpenGLTexture *glTexture;
    QOpenGLShaderProgram *shader;

    int posAttr, colAttr, normalAttr;

    void step(int startx, int starty, int width, int height, float threshold);
    void computeColorWithLights(QVector3D color, QVector3D normal, QVector3D vertex);
};
#endif // MAPCOMPONENT_H
