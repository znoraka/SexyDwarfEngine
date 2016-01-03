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
//    this->verticesArray.clear();
//    this->normalsArray.clear();
//    this->colorsArray.clear();
//    this->indexesArray.clear();
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

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(tempFilePath.toStdString(),
                                             aiProcess_GenSmoothNormals |
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType
                                             );

    QFile tempFile(tempFilePath);
    tempFile.remove();

    if (!scene) {
        qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
        return this;
    } else {
        qDebug() << "asset loaded with assimp";
    }

    if (scene->HasMaterials()) {
        for (unsigned int ii = 0; ii < scene->mNumMaterials; ++ii)
        {
            QSharedPointer<MaterialInfo> mater = processMaterial(scene->mMaterials[ii]);
            m_materials.push_back(mater);
        }
    }

    if (scene->HasMeshes())  {
        for (unsigned int ii = 0; ii < scene->mNumMeshes; ++ii) {
            m_meshes.push_back(processMesh(scene->mMeshes[ii]));
        }
    } else {
        qDebug() << "Error: No meshes found";
        return false;
    }

    if (scene->mRootNode != NULL) {
        Node *rootNode = new Node;
        processNode(scene, scene->mRootNode, 0, *rootNode);
        m_rootNode.reset(rootNode);
    } else {
        qDebug() << "Error loading model";
        return false;
    }

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

    for(auto i : verticesArray) {
        minX = qMin(i.x(), minX);
        maxX = qMax(i.x(), maxX);
        minY = qMin(i.y(), minY);
        maxY = qMax(i.y(), maxY);
    }

    this->bounds = QRectF(QPointF(minX, minY), QPointF(maxX, maxY));

    return this;
}

void VolumeComponent::update(float delta)
{
//    glPushMatrix();

//    qDebug() << "position = " << getEntity()->getPosition();

//    glTranslatef(getEntity()->getPosition().x(), getEntity()->getPosition().y(), getEntity()->getPosition().z());
//    glRotatef(getEntity()->getRotation().x(), 1, 0, 0);
//    glRotatef(getEntity()->getRotation().y(), 0, 1, 0);
//    glRotatef(getEntity()->getRotation().z(), 0, 0, 1);
//    glScalef(getEntity()->getScale().x(), getEntity()->getScale().y(), getEntity()->getScale().z());

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

    //    m_indexbuffer.bind();
    //    glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, NULL);
    glDrawElements(GL_TRIANGLES, indexesArray.size(), GL_UNSIGNED_INT, indexesArray.constData());
    //    m_indexbuffer.release();

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

//    glPopMatrix();
}

VolumeComponent *VolumeComponent::clone()
{
    VolumeComponent *v = VolumeComponent::pool->obtain();
    v->bounds = bounds;
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

QRectF VolumeComponent::getBounds() const
{
    return bounds;
}

QSharedPointer<MaterialInfo> VolumeComponent::processMaterial(aiMaterial *material)
{
    QSharedPointer<MaterialInfo> mater(new MaterialInfo);

    aiString mname;
    material->Get(AI_MATKEY_NAME, mname);
    if (mname.length > 0)
        mater->Name = mname.C_Str();

    int shadingModel;
    material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);

    if (shadingModel != aiShadingMode_Phong && shadingModel != aiShadingMode_Gouraud) {
        qDebug() << "This mesh's shading model is not implemented in this loader, setting to default material";
        mater->Name = "DefaultMaterial";
    } else {
        aiColor3D dif(0.f,0.f,0.f);
        aiColor3D amb(0.f,0.f,0.f);
        aiColor3D spec(0.f,0.f,0.f);
        float shine = 0.0;

        material->Get(AI_MATKEY_COLOR_AMBIENT, amb);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, dif);
        material->Get(AI_MATKEY_COLOR_SPECULAR, spec);
        material->Get(AI_MATKEY_SHININESS, shine);

        mater->Ambient = QVector3D(amb.r, amb.g, amb.b);
        mater->Diffuse = QVector3D(dif.r, dif.g, dif.b);
        mater->Specular = QVector3D(spec.r, spec.g, spec.b);
        mater->Shininess = shine;

        mater->Ambient *= .2;
        if (mater->Shininess == 0.0)
            mater->Shininess = 30;
    }

    return mater;
}

QSharedPointer<Mesh> VolumeComponent::processMesh(aiMesh *mesh)
{
    QSharedPointer<Mesh> newMesh(new Mesh);
    newMesh->name = mesh->mName.length != 0 ? mesh->mName.C_Str() : "";
    newMesh->indexOffset = m_indices.size();
    unsigned int indexCountBefore = m_indices.size();
    int vertindexoffset = m_vertices.size()/3;

    // Get Vertices
    if (mesh->mNumVertices > 0) {
        for (uint ii = 0; ii < mesh->mNumVertices; ++ii) {
            aiVector3D &vec = mesh->mVertices[ii];

            m_vertices.push_back(vec.x);
            m_vertices.push_back(vec.y);
            m_vertices.push_back(vec.z);
        }
    }

    // Get Normals
    if (mesh->HasNormals()) {
        for (uint ii = 0; ii < mesh->mNumVertices; ++ii) {
            aiVector3D &vec = mesh->mNormals[ii];
            m_normals.push_back(vec.x);
            m_normals.push_back(vec.y);
            m_normals.push_back(vec.z);
        };
    }

    // Get mesh indexes
    for (uint t = 0; t < mesh->mNumFaces; ++t) {
        aiFace* face = &mesh->mFaces[t];
        if (face->mNumIndices != 3) {
            qDebug() << "Warning: Mesh face with not exactly 3 indices, ignoring this primitive.";
            continue;
        }

        m_indices.push_back(face->mIndices[0]+vertindexoffset);
        m_indices.push_back(face->mIndices[1]+vertindexoffset);
        m_indices.push_back(face->mIndices[2]+vertindexoffset);
    }

    newMesh->indexCount = m_indices.size() - indexCountBefore;
    newMesh->material = m_materials.at(mesh->mMaterialIndex);

    return newMesh;
}

void VolumeComponent::processNode(const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode)
{
    newNode.name = node->mName.length != 0 ? node->mName.C_Str() : "";

    newNode.transformation = QMatrix4x4(node->mTransformation[0]);

    newNode.meshes.resize(node->mNumMeshes);
    for (uint imesh = 0; imesh < node->mNumMeshes; ++imesh) {
        QSharedPointer<Mesh> mesh = m_meshes[node->mMeshes[imesh]];
        newNode.meshes[imesh] = mesh;
    }

    for (uint ich = 0; ich < node->mNumChildren; ++ich) {
        newNode.nodes.push_back(Node());
        processNode(scene, node->mChildren[ich], parentNode, newNode.nodes[ich]);
    }
}
