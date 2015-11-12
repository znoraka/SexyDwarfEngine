#include "volumecomponent.h"

QString const VolumeComponent::name = "volumecomponent";
Pool<VolumeComponent *> *VolumeComponent::pool = new Pool<VolumeComponent*>([] () {return new VolumeComponent();});

VolumeComponent::VolumeComponent()
{
}

const QString VolumeComponent::componentName() const
{
    return VolumeComponent::name;
}

const QString VolumeComponent::systemName() const
{
    //    return VolumeSystem::name;
}

System *VolumeComponent::instantiateSystem()
{
    //    return new VolumeSystem();
}

void VolumeComponent::release()
{
    VolumeComponent::pool->release(this);
}

VolumeComponent *VolumeComponent::init(QString filePath)
{
    verticesArray.clear();
    normalsArray.clear();
    colorsArray.clear();
    indexesArray.clear();

    QStringList list;
    qDebug() << "loading " << filePath;
    QFile file( filePath );
    if(file.size() < 10) {
        qDebug() << "file not found : " << filePath;
        return nullptr;
    }
    if ( file.open(QIODevice::ReadOnly) )
    {
        QTextStream stream( &file );
        list = stream.readAll().split("\n");
    }

    if(!list.at(0).contains("ply")) {
        qDebug() << "bad file";
        return nullptr;
    }
    int vertexCount = 0, faceCount = 0;
    int cpt = 0;

    while(vertexCount == 0) {
        if(list.at(cpt).startsWith("element vertex")) {
            vertexCount = list.at(cpt).split(" ").last().toInt();
        }
        cpt++;
    }

    while(faceCount == 0) {
        if(list.at(cpt).startsWith("element face")) {
            faceCount = list.at(cpt).split(" ").last().toInt();
        }
        cpt++;
    }

    while(list.at(cpt++).compare("end_header") != 0);

//    qDebug() << "vertex count = " << vertexCount;
//    qDebug() << "cpt = " << cpt;
    for (int i = cpt; i < vertexCount + cpt; ++i) {
        QStringList l = list.at(i).split(" ");
        QVector3D p, n, c;
        p.setX(l.at(0).toFloat());
        p.setY(l.at(1).toFloat());
        p.setZ(l.at(2).toFloat());
        verticesArray.push_back(p);
        if(l.size() > 5) {
            n.setX(l.at(3).toFloat());
            n.setY(l.at(4).toFloat());
            n.setZ(l.at(5).toFloat());
        } else {
            n.setX(0);
            n.setY(0);
            n.setZ(0);
        }
        normalsArray.push_back(n);
        if(l.size() > 8) {
            c.setX(l.at(6).toFloat() / 255.0f);
            c.setY(l.at(7).toFloat() / 255.0f);
            c.setZ(l.at(8).toFloat() / 255.0f);
        } else {
            c.setX(0);
            c.setY(0);
            c.setZ(0);
        }
        colorsArray.push_back(c);
    }

    for (int i = cpt + vertexCount; i < cpt + vertexCount + faceCount; ++i) {
        QStringList faces = list.at(i).split(" ");
        if(faces.at(0).toInt() == 3) {
            indexesArray.push_back(faces.at(1).toUInt());
            indexesArray.push_back(faces.at(2).toUInt());
            indexesArray.push_back(faces.at(3).toUInt());
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

void VolumeComponent::update(float delta)
{

    glPushMatrix();

    glTranslatef(getEntity()->getPosition().x(), getEntity()->getPosition().y(), getEntity()->getPosition().z());
    glRotatef(getEntity()->getRotation().x(), 1, 0, 0);
    glRotatef(getEntity()->getRotation().y(), 0, 1, 0);
    glRotatef(getEntity()->getRotation().z(), 0, 0, 1);
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

//    m_indexbuffer.bind();
//    glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, NULL);
    glDrawElements(GL_TRIANGLES, indexesArray.size(), GL_UNSIGNED_INT, indexesArray.constData());
//    m_indexbuffer.release();

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}

VolumeComponent *VolumeComponent::clone()
{
    VolumeComponent *v = VolumeComponent::pool->obtain();
    v->verticesArray = verticesArray;
    v->normalsArray = normalsArray;
    v->colorsArray = colorsArray;
    v->indexesArray = indexesArray;

    v->m_vertexbuffer = m_vertexbuffer;
    v->m_normalbuffer = m_normalbuffer;
    v->m_colorbuffer = m_colorbuffer;
    v->m_indexbuffer = m_indexbuffer;

    return v;
}
