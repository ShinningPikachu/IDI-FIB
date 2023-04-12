// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"
class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  private:
    int printOglError(const char file[], int line, const char func[]);
  protected:
    virtual void carregaShaders();
    virtual void projectTransform();
    virtual void initializeGL ();
    virtual void viewTransform();
    virtual void creaBuffers();
    virtual void paintGL();
    virtual void creaBuffers_Terra();
    virtual void resizeGL();
    virtual void iniCamara();
    virtual void iniCaja();
    virtual void Calcula();
    GLuint viewLoc;
    GLuint projLoc;
    GLuint VAO_H;
    GLuint VAO_T;
    
    float FOV, ra, znear, zfar;
    glm::vec3 OBS, VRP, up;
    glm::vec3 minim, maxim;
    Model m;
};
