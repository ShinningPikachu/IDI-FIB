#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void initializeGL();
//    glm::vec3 posCub = glm::vec3(5.0,0.0,0.0);
    float AnglePatricio = 0;
    bool estatCubPat = false;
    glm::vec3 colFocus = glm::vec3 {1, 1, 0};

  private:
    int printOglError(const char file[], int line, const char func[]);
};
