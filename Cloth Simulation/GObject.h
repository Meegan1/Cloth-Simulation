#pragma once
#include <vector>
#include <qopenglfunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <glm/glm.hpp>
#include <QMatrix4x4>


struct Triangle
{
    unsigned int x = -1;
    unsigned int y = -1;
    unsigned int z = -1;
};

class CGObject
{
public:
    typedef std::shared_ptr<CGObject> CGObject_Ptr;
public:
    CGObject();

public:
    virtual int init(std::string file, float scaleCo = 1.0f);
    virtual int writeFile(std::string objFile);
    virtual int render();

    bool getShowWire() { return m_showWire; }
    bool getShowTexture() { return m_showTexture; }
    bool getShowColor() { return m_showColor; }

    void switchShowWire() { m_showWire = !m_showWire; }
    void switchShowTexture() { m_showTexture = !m_showTexture; }
    void switchShowColor() { m_showColor = !m_showColor; }

	void setFaceVertices(std::vector<glm::vec3> vertices, std::vector<Triangle> triangles);

    int scale(float scale = 1);

    int normalise();
protected:
    int loadObj(std::string file);
    int computeFaceNormals(bool recompute = false);
    int computeVertNormals();

    int initBuffer();


    //init colors
    int computeVertexColors();
protected:
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_vertNormals;
    std::vector<glm::vec3> m_vertTexCoords;
    std::vector<glm::vec3> m_colors;
    std::vector<Triangle> m_triangles;
    std::vector<glm::vec3> m_faceNormals;

    bool m_showWire = false;
    bool m_showTexture = false;
    bool m_showColor = true;
    bool m_showNormals = false;


    std::vector<float> m_buffer;

    int m_unitSize = 0;

    QMatrix4x4 m_transMat;

    std::string m_file;

    std::string m_name;
};


