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
#include <QStack>
#include <QtCore/qmath.h>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "engine/components/component.h"
#include "engine/tools/delaunay.h"
#include "game/enemycomponent.h"

class PathFollowerComponent;
class EnemyComponent;

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

    float getWidth() const;
    float getHeight() const;

    QList<Entity *> *getEnemies() const;

    void nextWave();

    static const QString name;
    float getZ(float i, float j);
    QVector3D toMapCoordinate(QVector3D coords);

private:
    struct EnemyStruct {QString name;
                        float time;
                        int x, y;
                        QString toString() {
                            return QString() + "{" + "name:" + name + ", time:" + QString::number(time) + ", x:" + QString::number(x) + ", y:" + QString::number(y) + "}";
                        }
                        EnemyStruct(){}
                        EnemyStruct(QString name, float time, int x, int y) {
                            this->name = name;
                            this->time = time;
                            this->x = x;
                            this->y = y;
                        }};

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

    QJsonDocument wavesDocument;
    QHash<QString, EnemyComponent*> enemiesTemplate;
    VolumeComponent *volumeComponent;
    PathFollowerComponent *pathFollowerComponent;

    QVector<QStack<EnemyStruct> > waves;

    QList<Entity*> *enemies;

    float elapsed;
    int waveIndex;

    void step(int startx, int starty, int width, int height, float threshold);
    void computeColorWithLights(QVector3D color, QVector3D normal, QVector3D vertex);
    void parseJsonWaves();
};
#endif // MAPCOMPONENT_H
