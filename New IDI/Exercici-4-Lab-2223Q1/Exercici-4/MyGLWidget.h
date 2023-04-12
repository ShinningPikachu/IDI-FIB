// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void viewTransform();
    virtual void iniCamera();
    virtual void initializeGL();
    virtual void carregaShaders();
    virtual void modelTransformTG();
    virtual void modelTransformMorty();
    virtual void calculaCapsaModelMorty();
    virtual void modelTransformFantasma();
    virtual void paintGL();
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    float angleX;
    GLuint colFocusCanto, posFocusCanto1, posFocusCanto2, posFocusCanto3, posFocusCanto4, colFocusLlanterna, posFocusLlanterna;
    GLuint TGloc;
    glm::vec3 mortysize;
    glm::vec3 mortyPos = glm::vec3(5.0, 0.0, 5.0);
    float direcio = 90.0;
    float dist = 1.0;
    GLboolean llumM = true;
    GLboolean llumE = true;
    GLboolean mirant = false;
    glm::vec3 pcolFocusCanto;
    glm::vec3 pcolFocusLlanterna;

  private:
    void resetejar();
    int printOglError(const char file[], int line, const char func[]);
};
