#include "mapcomponent.h"

QString const MapComponent::name = "mapcomponent";
Pool<MapComponent *> *MapComponent::pool = new Pool<MapComponent*>([] () {return new MapComponent();});

uint qHash(double data)
{
    union U {
        quint64 n;
        double f;
    };
    U u;
    u.f = data;
    return u.f;
}

inline uint qHash(const QVector3D &v, uint seed)
{
    return qHash(v.x()) ^ ROTL10(qHash(v.y())) ^ ROTL20(qHash(v.z()));
}

MapComponent::MapComponent()
{
}

const QString MapComponent::componentName() const
{
    return MapComponent::name;
}

void MapComponent::release()
{
    MapComponent::pool->release(this);
}

MapComponent *MapComponent::init(QString mapFolder)
{
    heightmap = QImage(mapFolder + "h.png").mirrored();
    glTexture = new QOpenGLTexture(QImage(mapFolder + "t.png").mirrored());
    glTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    glTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    shader = new QOpenGLShaderProgram();
    shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assets/shaders/map_vertex.glsl");
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assets/shaders/map_fragment.glsl");
    qDebug() << "linked = " << shader->link();
    shader->bind();
    shader->setUniformValue("textureWidth", (float) heightmap.width());
    shader->setUniformValue("textureHeight", (float) heightmap.height());
    shader->release();

    auto color = [](QVector3D v) {
        if(v.z() < 0.08) {
            return QVector3D(v.z(), 0.4, 0);
        } else if(v.z() > 0.08 && v.z() < 0.15) {
            return QVector3D(0.54, 0.27 + v.z(), 0.07);
        } else {
            return QVector3D(0.9, 0.8, 0.9);
        }
    };

    step(0, 0, heightmap.width(), heightmap.height(), 0.0);

    del_point2d_t	points_[verticesSet.size()];
    QList<QVector3D> l = verticesSet.toList();
    for (int i = 0; i < verticesSet.size(); ++i) {
        del_point2d_t p;
        p.x = l[i].x(); p.y = l[i].y();
        points_[i] = p;
    }
    delaunay2d_t* res = delaunay2d_from(points_, verticesSet.size());
    tri_delaunay2d_t*	tdel	= tri_delaunay2d_from(res);

    for (int i = 0; i < tdel->num_triangles; ++i) {
        for (int j = 0; j < 3; ++j) {
            indexesArray.push_back(tdel->tris[i * 3 + j]);
        }
    }
    delaunay2d_release(res);

    for (int i = 0; i < indexesArray.size(); i += 3) {
        QVector3D v1 = l.at(indexesArray[i]);
        QVector3D v2 = l.at(indexesArray[i + 1]);
        QVector3D v3 = l.at(indexesArray[i + 2]);
        QVector3D n = QVector3D::normal(v2 - v1, v3 - v1);

        verticesArray.push_back(v1);
        colorsArray.push_back(color(v1));
        normalsArray.push_back(n);

        verticesArray.push_back(v2);
        colorsArray.push_back(color(v2));
        normalsArray.push_back(n);

        verticesArray.push_back(v3);
        colorsArray.push_back(color(v3));
        normalsArray.push_back(n);

    }

    shader->bind();

    posAttr = shader->attributeLocation("position");
    colAttr = shader->attributeLocation("color");
    normalAttr = shader->attributeLocation("normal");

    v_vao.create();
    v_vao.bind();
    m_vertexbuffer.create();
    m_vertexbuffer.bind();
    m_vertexbuffer.allocate(verticesArray.constData(), verticesArray.size() * sizeof(QVector3D));
    shader->enableAttributeArray(posAttr);
    shader->setAttributeBuffer(posAttr, GL_FLOAT, 0, 3, 0);
    m_vertexbuffer.release();

    m_normalbuffer.create();
    m_normalbuffer.bind();
    m_normalbuffer.allocate(normalsArray.constData(), normalsArray.size() * sizeof(QVector3D));
    shader->enableAttributeArray(normalAttr);
    shader->setAttributeBuffer(normalAttr, GL_FLOAT, 0, 3, 0);
    m_normalbuffer.release();

    m_colorbuffer.create();
    m_colorbuffer.bind();
    m_colorbuffer.allocate(colorsArray.constData(), colorsArray.size() * sizeof(QVector3D));
    shader->enableAttributeArray(colAttr);
    shader->setAttributeBuffer(colAttr, GL_FLOAT, 0, 3, 0);
    m_colorbuffer.release();

    m_indexbuffer.create();
    m_indexbuffer.bind();
    m_indexbuffer.allocate(indexesArray.constData(), indexesArray.size() * sizeof(GLuint));
    m_indexbuffer.release();

    v_vao.release();
    shader->release();

    return this;
}

void MapComponent::step(int startx, int starty, int width, int height, float threshold)
{
    float posX, posY;

    if(startx + width >= heightmap.width()) width--;
    if(starty + height >= heightmap.height()) height--;

    auto areaAvg = [=]() {
        double sum = 0;
        for (int i = startx; i < width + startx; ++i) {
            for (int j = starty; j < height + starty; ++j) {
                sum += qGray(heightmap.pixel(i , j));
            }
        }
        return sum /= (width * height);
    };

    auto areaStdDev = [=] (double avg) {
        double dev = 0;

        for (int i = startx; i < width + startx; ++i) {
            for (int j = starty; j < height + starty; ++j) {
                dev += pow(qGray(heightmap.pixel(i , j)) - avg, 2);
            }
        }

        return sqrt(dev / (width * height));
    };

    double v = areaStdDev(areaAvg());

    if(v < threshold || width == 1) {
        posX = startx;
        posY = starty;
        QVector3D v1(posX,  posY, getZ(posX, posY));
        verticesSet.insert(v1);

        posX = startx;
        posY = (starty + height);
        QVector3D v2(posX,  posY, getZ(posX, posY));
        verticesSet.insert(v2);

        posX = (startx + width);
        posY = starty;
        QVector3D v3(posX,  posY, getZ(posX, posY));
        verticesSet.insert(v3);

    } else {
        step(startx, starty, width * 0.5, height * 0.5, threshold);
        step(startx + width * 0.5, starty, width * 0.5, height * 0.5, threshold);
        step(startx, starty + height * 0.5, width * 0.5, height * 0.5, threshold);
        step(startx + width * 0.5, starty + height * 0.5, width * 0.5, height * 0.5, threshold);
    }
}

void MapComponent::update(float delta)
{
    shader->bind();
    shader->setUniformValue("textureWidth", (float) heightmap.width());
    shader->setUniformValue("textureHeight", (float) heightmap.height());

    glTexture->bind();

    glPushMatrix();

    glRotatef(getEntity()->getRotation().x(), 1, 0, 0);
    glRotatef(getEntity()->getRotation().y(), 0, 1, 0);
    glRotatef(getEntity()->getRotation().z(), 0, 0, 1);
    glTranslatef(getEntity()->getPosition().x(), getEntity()->getPosition().y(), getEntity()->getPosition().z());
    glScalef(getEntity()->getScale().x(), getEntity()->getScale().y(), getEntity()->getScale().z());

    v_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, normalsArray.size());
    v_vao.release();

    glTexture->release();
    shader->release();

    glPopMatrix();
}

float MapComponent::getZ(float i, float j)
{
    return qGray(this->heightmap.pixel(i, j)) * 0.8f;
}

