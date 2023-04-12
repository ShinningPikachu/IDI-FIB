#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();


  private:

    int printOglError(const char file[], int line, const char func[]);

  protected:
    virtual void RickTransform();
    virtual void MortyTransform();
    virtual void MortyTransformNew();
    virtual void PortalTransform();
    virtual void identTransform();
    virtual void viewTransform();
    virtual void projectTransform();
    virtual void iniCamera();
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void paintGL();
    virtual void iniEscena();
    virtual void carregaShaders();

    float direccio = float(M_PI);
    float x_ant, y_ant, girPsi, girTheta;
    float direccioPortal = float(M_PI);
    float direccioMorty = float(0.0);

    float fov = 2.0 * asin(radiEscena / (2*radiEscena));
    float znear = radiEscena;
    float zfar  = 3*radiEscena;
    //float ra = 1.0;

    float fovNew = float(M_PI/2);
    float znearNew = 0.25;
    float zfarNew  = 18;
    float raNew = 1.0;

    glm::vec3 posMorty = glm::vec3(0, 0, 1);

    glm::vec3 posMortyNew;
    float direccioMortyNew;

    GLboolean colorchange = false;
    GLboolean detect = MortyPortalTest(direccio, posMorty);
    GLboolean tancat = true;
    GLboolean existe = false;

    GLboolean camera = false;

    GLuint MortyColorLoc;


};
