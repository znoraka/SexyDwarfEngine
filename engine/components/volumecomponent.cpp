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
//    this->vao.destroy();
    this->verticesArray.clear();
    this->normalsArray.clear();
    this->colorsArray.clear();
    this->indexesArray.clear();
//    this->m_vertexbuffer.destroy();
//    this->m_normalbuffer.destroy();
//    this->m_colorbuffer.destroy();
//    this->m_indexbuffer.destroy();

    VolumeComponent::pool->release(this);
}

VolumeComponent *VolumeComponent::init(QString filePath)
{
    QFile f(filePath);
    QString tempFilePath("./bnrpok");
    f.copy(tempFilePath);

    QFile tempFile(tempFilePath);
    tempFile.remove();

    this->vao.destroy();
    this->m_vertexbuffer.destroy();
    this->m_normalbuffer.destroy();
    this->m_colorbuffer.destroy();
    this->m_indexbuffer.destroy();

    m_vertices.clear();
    m_normals.clear();
    m_indices.clear();

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
            c.setX(1);
            c.setY(1);
            c.setZ(1);
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

    float minX = verticesArray[0].x(), maxX = verticesArray[0].x();
    float minY = verticesArray[0].y(), maxY = verticesArray[0].y();
    float minZ = verticesArray[0].z(), maxZ = verticesArray[0].z();

    for(auto i : verticesArray) {
        minX = qMin(i.x(), minX);
        maxX = qMax(i.x(), maxX);
        minY = qMin(i.y(), minY);
        maxY = qMax(i.y(), maxY);
        minZ = qMin(i.z(), minZ);
        maxZ = qMax(i.z(), maxZ);
    }

    this->bounds = QRectF(QPointF(minX, minY), QPointF(maxX, maxY));
    this->size = QVector3D(maxX - minX, maxY - minY, maxZ - minZ);

    return this;
}

void VolumeComponent::update(float delta)
{

    auto unRotate = []() {
        GLfloat m[16];
        glGetFloatv (GL_MODELVIEW_MATRIX, m);

        float d = sqrt(m[0] * m[0] + m[4] * m[4] + m[8] * m[8]);

        m[0] = d;
        m[1] = 0;
        m[2] = 0;
        m[4] = 0;
        m[5] = d;
        m[6] = 0;
        m[8] = 0;
        m[9] = 0;
        m[10] = d;

        glLoadMatrixf(m);
    };

    auto sendVertex = [](QVector3D p) {
        glVertex3f(p.x(), p.y(), p.z());
//        qDebug() << p;
    };

//    glPushMatrix();
////    unRotate();

////    glRotatef(90, 1, 0, 0);
//    glColor3f(1, 0, 1);
//    glBegin(GL_LINE_STRIP);


//    sendVertex(QVector3D(-size.x() * 0.5, 0, size.z() * 0.5));
//    sendVertex(QVector3D(size.x() * 0.5, 0, size.z() * 0.5));
//    sendVertex(QVector3D(size.x() * 0.5, size.y(), size.z() * 0.5));
//    sendVertex(QVector3D(-size.x() * 0.5, size.y(), size.z() * 0.5));

//    sendVertex(QVector3D(-size.x() * 0.5, 0, size.z() * 0.5));
//    sendVertex(QVector3D(-size.x() * 0.5, 0, -size.z() * 0.5));
//    sendVertex(QVector3D(-size.x() * 0.5, size.y(), -size.z() * 0.5));
//    sendVertex(QVector3D(-size.x() * 0.5, size.y(), size.z() * 0.5));

//    sendVertex(QVector3D(size.x() * 0.5, size.y(), size.z() * 0.5));
//    sendVertex(QVector3D(size.x() * 0.5, size.y(), -size.z() * 0.5));
//    sendVertex(QVector3D(-size.x() * 0.5, size.y(), -size.z() * 0.5));
//    sendVertex(QVector3D(-size.x() * 0.5, 0, -size.z() * 0.5));

//    sendVertex(QVector3D(size.x() * 0.5, 0, -size.z() * 0.5));
//    sendVertex(QVector3D(size.x() * 0.5, 0, size.z() * 0.5));
//    sendVertex(QVector3D(size.x() * 0.5, 0, -size.z() * 0.5));
//    sendVertex(QVector3D(size.x() * 0.5, size.y(), -size.z() * 0.5));

//    glEnd();

//    glPopMatrix();

    glEnable(GL_COLOR_MATERIAL);
    glColor4f(0, 1, 0, 0.5);
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

    glDrawElements(GL_TRIANGLES, indexesArray.size(), GL_UNSIGNED_INT, indexesArray.constData());

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

VolumeComponent *VolumeComponent::clone()
{
    VolumeComponent *v = VolumeComponent::pool->obtain();
    v->bounds = bounds;
    v->size = size;
    v->verticesArray = QVector<QVector3D>(verticesArray);
    v->normalsArray = QVector<QVector3D>(normalsArray);
    v->colorsArray = QVector<QVector3D>(colorsArray);
    v->indexesArray = QVector<GLuint>(indexesArray);

    v->m_vertexbuffer = m_vertexbuffer;
    v->m_normalbuffer = m_normalbuffer;
    v->m_colorbuffer = m_colorbuffer;
    v->m_indexbuffer = m_indexbuffer;
//    v->vao = vao;

    return v;
}

bool VolumeComponent::clicked(Qt::MouseButton button, int x, int y)
{
    QVector4D minsAndMaxs(99999, -99999, 99999, -99999);

    auto getRect = [=] (QVector4D minsAndMaxs, QVector3D v) {
        QPointF p = Camera::worldToScreen(v * getEntity()->getScale() + getEntity()->getPosition(), getEntity()->getModelViewMatrix(), getEntity()->getProjectionMatrix());
        if(p.x() < minsAndMaxs.x()) minsAndMaxs.setX(p.x());
        if(p.x() > minsAndMaxs.y()) minsAndMaxs.setY(p.x());
        if(p.y() < minsAndMaxs.z()) minsAndMaxs.setZ(p.y());
        if(p.y() > minsAndMaxs.w()) minsAndMaxs.setW(p.y());
        return minsAndMaxs;
    };

    minsAndMaxs = getRect(minsAndMaxs, QVector3D(-size.x() * 0.5, 0, size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(size.x() * 0.5, 0, size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(size.x() * 0.5, size.y(), size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(-size.x() * 0.5, size.y(), size.z() * 0.5));

    minsAndMaxs = getRect(minsAndMaxs, QVector3D(-size.x() * 0.5, 0, size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(-size.x() * 0.5, 0, -size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(-size.x() * 0.5, size.y(), -size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(-size.x() * 0.5, size.y(), size.z() * 0.5));

    minsAndMaxs = getRect(minsAndMaxs, QVector3D(size.x() * 0.5, size.y(), size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(size.x() * 0.5, size.y(), -size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(-size.x() * 0.5, size.y(), -size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(-size.x() * 0.5, 0, -size.z() * 0.5));

    minsAndMaxs = getRect(minsAndMaxs, QVector3D(size.x() * 0.5, 0, -size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(size.x() * 0.5, 0, size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(size.x() * 0.5, 0, -size.z() * 0.5));
    minsAndMaxs = getRect(minsAndMaxs, QVector3D(size.x() * 0.5, size.y(), -size.z() * 0.5));

    return x > minsAndMaxs.x() && x < minsAndMaxs.y() && y > minsAndMaxs.z() && y < minsAndMaxs.w();
}

bool VolumeComponent::collides(VolumeComponent *other)
{
    QVector3D thispos(getEntity()->getPosition().x() + getSize().x() * 0.5 * getEntity()->getScale().x(),
                      getEntity()->getPosition().y() + getSize().y() * 0.5 * getEntity()->getScale().y(),
                      getEntity()->getPosition().z() + getSize().z() * 0.5 * getEntity()->getScale().z());

    QVector3D otherpos(other->getEntity()->getPosition().x() + other->getSize().x() * 0.5 * other->getEntity()->getScale().x(),
                       other->getEntity()->getPosition().y() + other->getSize().y() * 0.5 * other->getEntity()->getScale().y(),
                       other->getEntity()->getPosition().z() + other->getSize().z() * 0.5 * other->getEntity()->getScale().z());

    return thispos.distanceToPoint(otherpos) < qMin(qMin(getSize().x() * 0.5 * getEntity()->getScale().x() +
                                                    other->getSize().x() * 0.5 * other->getEntity()->getScale().x(),
                                                    getSize().y() * 0.5 * getEntity()->getScale().y() +
                                                    other->getSize().y() * 0.5 * other->getEntity()->getScale().y()),
                                                    getSize().z() * 0.5 * getEntity()->getScale().z() +
                                                    other->getSize().z() * 0.5 * other->getEntity()->getScale().z());
}

QRectF VolumeComponent::getBounds() const
{
    return bounds;
}

QVector3D VolumeComponent::getSize() const
{
    return size;
}

void VolumeComponent::deleteBuffersData()
{
//    this->vao.destroy();
//    this->m_vertexbuffer.destroy();
//    this->m_normalbuffer.destroy();
//    this->m_colorbuffer.destroy();
//    this->m_indexbuffer.destroy();
}
