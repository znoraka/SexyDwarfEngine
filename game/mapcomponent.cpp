#include "mapcomponent.h"

QString const MapComponent::name = "mapcomponent";
Pool<MapComponent *> *MapComponent::pool = new Pool<MapComponent*>([] () {return new MapComponent();});

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
    heightmap = QImage(mapFolder + "h.png");
    glTexture = new QOpenGLTexture(QImage(mapFolder + "t.png"));
    glTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    glTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    shader = new QOpenGLShaderProgram();
    shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assets/shaders/map_vertex.glsl");
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assets/shaders/map_fragment.glsl");
    qDebug() << "linked = " << shader->link();
    shader->bind();
    shader->setUniformValue("textureWidth", heightmap.width());
    shader->setUniformValue("textureHeight", heightmap.height());
    shader->release();

//    float stepX = 1.0 / (heightmap.width());
//    float stepY = 1.0 / (heightmap.height());

    float stepX = 1.0;
    float stepY = 1.0;

    float posX;
    float posY;

    auto color = [](QVector3D v) {
        if(v.z() < 0.08) {
            return QVector3D(v.z(), 0.4, 0);
        } else if(v.z() > 0.08 && v.z() < 0.15) {
            return QVector3D(0.54, 0.27 + v.z(), 0.07);
        } else {
            return QVector3D(0.9, 0.8, 0.9);
        }
    };

    for (int i = 0 ; i < heightmap.width() - 1; ++i) {
        for (int j = 0; j < heightmap.height() - 1; ++j) {
            posX = i * stepX; posY = (j + 1) * stepY;
            QVector3D v2(posX,  posY, getZ(posX, posY));
            verticesArray.push_back(v2);
            colorsArray.push_back(color(v2));

            posX = i * stepX; posY = j * stepY;
            QVector3D v1(posX,  posY, getZ(posX, posY));
            verticesArray.push_back(v1);
            colorsArray.push_back(color(v1));

            posX = (i + 1) * stepX; posY = j * stepY;
            QVector3D v3(posX,  posY, getZ(posX, posY));
            verticesArray.push_back(v3);
            colorsArray.push_back(color(v3));

            QVector3D n = QVector3D::normal(v3 - v1, v2 - v1);
            normalsArray.push_back(n);
            normalsArray.push_back(n);
            normalsArray.push_back(n);

            posX = i * stepX; posY = (j + 1) * stepY;
            QVector3D v4(posX, posY, getZ(posX, posY));
            verticesArray.push_back(v4);
            colorsArray.push_back(color(v4));

            posX = (i + 1) * stepX; posY = j * stepY;
            QVector3D v6(posX, posY, getZ(posX, posY));
            verticesArray.push_back(v6);
            colorsArray.push_back(color(v6));

            posX = (i + 1) * stepX; posY = (j + 1) * stepY;
            QVector3D v5(posX, posY, getZ(posX, posY));
            verticesArray.push_back(v5);
            colorsArray.push_back(color(v5));

            QVector3D n1 = QVector3D::normal(v6 - v4, v5 - v4);
            normalsArray.push_back(n1);
            normalsArray.push_back(n1);
            normalsArray.push_back(n1);
        }
    }

    m_vertexbuffer.create();
    m_vertexbuffer.bind();
    m_vertexbuffer.allocate(verticesArray.constData(), verticesArray.size() * sizeof(QVector3D));
    m_vertexbuffer.release();

    m_normalbuffer.create();
    m_normalbuffer.bind();
    m_normalbuffer.allocate(normalsArray.constData(), normalsArray.size() * sizeof(QVector3D));
    m_normalbuffer.release();

    m_colorbuffer.create();
    m_colorbuffer.bind();
    m_colorbuffer.allocate(colorsArray.constData(), colorsArray.size() * sizeof(QVector3D));
    m_colorbuffer.release();

    m_indexbuffer.create();
    m_indexbuffer.bind();
    m_indexbuffer.allocate(indexesArray.constData(), indexesArray.size() * sizeof(GLuint));
    m_indexbuffer.release();


    return this;
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

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    m_vertexbuffer.bind();
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    m_vertexbuffer.release();
    m_normalbuffer.bind();
    glNormalPointer(GL_FLOAT, 0, NULL);
    m_normalbuffer.release();
    m_colorbuffer.bind();
    glColorPointer(3, GL_FLOAT, 0, NULL);
    m_colorbuffer.release();

    glDrawArrays(GL_TRIANGLES, 0, verticesArray.size());

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();

    glTexture->release();
    shader->release();
}

float MapComponent::getZ(float i, float j)
{
    return qGray(this->heightmap.pixel(i, j)) * 0.8f;
}

