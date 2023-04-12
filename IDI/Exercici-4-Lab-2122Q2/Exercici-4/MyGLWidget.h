// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void carregaShaders();
    virtual void initializeGL();
    virtual void modelTransformPatricio();
    virtual void paintGL();
    	//loc de posicion y color
    GLuint PosFocusCamaraLoc, PosFocusEscenaLoc, PosFocusPatricioLoc, ColorFocusCamaraLoc, ColorFocusEscenaLoc, ColorFocusPatricioLoc;
    	//valor inicial de color
    glm::vec3 ColorFocusCamara = glm::vec3(0.9, 0.0, 0.9);
    glm::vec3 ColorFocusEscena = glm::vec3(0.9, 0.9, 0.9);
    glm::vec3 ColorFocusPatricio = glm::vec3(0.9, 0.9, 0.2);
    	//rotacion
    float gir = 0;
    	//llum ences o apagat
    bool LlumFocus = true;
    bool LlumEscena = true;
    bool LlumPatricio = true;
  private:
    int printOglError(const char file[], int line, const char func[]);
};
