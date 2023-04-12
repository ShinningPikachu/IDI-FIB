// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <QtMath>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[])
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502:
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}
void MyGLWidget::iniCamera(){
  angleY = 0.0;
  angleX = 0.0;
  viewTransform ();
}
void MyGLWidget::viewTransform (){
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::carregaShaders(){
  LL4GLWidget::carregaShaders();
  colFocusCanto = glGetUniformLocation(program->programId(), "colFocusCanto");
  posFocusCanto1 = glGetUniformLocation(program->programId(), "posFocusCanto1");
  posFocusCanto2 = glGetUniformLocation(program->programId(), "posFocusCanto2");
  posFocusCanto3 = glGetUniformLocation(program->programId(), "posFocusCanto3");
  posFocusCanto4 = glGetUniformLocation(program->programId(), "posFocusCanto4");

  colFocusLlanterna = glGetUniformLocation(program->programId(), "colFocusLlanterna");
  posFocusLlanterna = glGetUniformLocation(program->programId(), "posFocusLlanterna");

  TGloc = glGetUniformLocation(program->programId(), "TGpos");

}
void MyGLWidget::initializeGL(){
  initializeOpenGLFunctions();

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  carregaShaders();

  creaBuffersMorty();
  creaBuffersFantasma();
  creaBuffersTerraIParets();

  pcolFocusCanto = glm::vec3(0.7, 0.0, 0.7);
  glm::vec3 pposFocusCanto1 = glm::vec3(0.0, 6.0, 0.0);
  glm::vec3 pposFocusCanto2 = glm::vec3(10.0, 6.0, 0.0);
  glm::vec3 pposFocusCanto3 = glm::vec3(0.0, 6.0, 10.0);
  glm::vec3 pposFocusCanto4 = glm::vec3(10.0, 6.0, 10.0);

  glUniform3fv(colFocusCanto, 1, &pcolFocusCanto[0]);
  glUniform3fv(posFocusCanto1, 1, &pposFocusCanto1[0]);
  glUniform3fv(posFocusCanto2, 1, &pposFocusCanto2[0]);
  glUniform3fv(posFocusCanto3, 1, &pposFocusCanto3[0]);
  glUniform3fv(posFocusCanto4, 1, &pposFocusCanto4[0]);

  pcolFocusLlanterna = glm::vec3(0.9, 0.9, 0.0);
  glm::vec3 pposFocusLlanterna = centreBaseMorty;

  glUniform3fv(colFocusLlanterna, 1, &pcolFocusLlanterna[0]);
  glUniform3fv(posFocusLlanterna, 1, &pposFocusLlanterna[0]);

  modelTransformTG();

  iniEscena();
  iniCamera();
}
void MyGLWidget::paintGL ()
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, , alt);

  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 30);

  // MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty ();
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);

  // FANTASMA
  if (!mirant and !llumE){
    glBindVertexArray (VAO_Fantasma);
    modelTransformFantasma ();
    glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);
  }

  glBindVertexArray(0);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (e->y() - yClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::modelTransformTG(){
  glm::mat4 TGp;
  TGp = glm::translate(glm::mat4(1.f), mortyPos + glm::vec3(0.6*mortysize[0], -0.9*mortysize[1], -0.07*mortysize[2]));
  TGp = glm::rotate(TGp, float(glm::radians(direcio)),glm::vec3(0.f, 1.f, 0.f));
  TGp = glm::scale(TGp, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TGp = glm::translate(TGp, -centreBaseMorty);

  glUniformMatrix4fv(TGloc, 1, GL_FALSE, &TGp[0][0]);
}

void MyGLWidget::calculaCapsaModelMorty ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = morty.vertices()[0];
  miny = maxy = morty.vertices()[1];
  minz = maxz = morty.vertices()[2];
  for (unsigned int i = 3; i < morty.vertices().size(); i+=3)
  {
    if (morty.vertices()[i+0] < minx)
      minx = morty.vertices()[i+0];
    if (morty.vertices()[i+0] > maxx)
      maxx = morty.vertices()[i+0];
    if (morty.vertices()[i+1] < miny)
      miny = morty.vertices()[i+1];
    if (morty.vertices()[i+1] > maxy)
      maxy = morty.vertices()[i+1];
    if (morty.vertices()[i+2] < minz)
      minz = morty.vertices()[i+2];
    if (morty.vertices()[i+2] > maxz)
      maxz = morty.vertices()[i+2];
  }
  escalaMorty = 2.5/(maxy-miny);

  centreBaseMorty[0] = (minx+maxx)/2.0;
  centreBaseMorty[1] = miny;
  centreBaseMorty[2] = (minz+maxz)/2.0;
  mortysize[0] = maxx - minx;
  mortysize[1] = maxy - miny;
  mortysize[2] = maxz - minz;
}
void MyGLWidget::modelTransformFantasma ()
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(dist,0.5,5));
  TG = glm::rotate(TG, float(glm::radians(90.0)),glm::vec3(0.f, 1.f, 0.f));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::translate(TG, -centreBaseFantasma);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformMorty ()
{
  TG_morty = glm::translate(glm::mat4(1.f), mortyPos);
  TG_morty = glm::rotate(TG_morty, float(glm::radians(direcio)),glm::vec3(0.f, 1.f, 0.f));
  TG_morty = glm::scale(TG_morty, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG_morty = glm::translate(TG_morty, -centreBaseMorty);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG_morty[0][0]);
}
void MyGLWidget::resetejar(){
  direcio = 90.0;
  llumE = true;
  llumM = true;
  pcolFocusLlanterna = glm::vec3(0.9, 0.9, 0.0);
  glUniform3fv(colFocusLlanterna, 1, &pcolFocusLlanterna[0]);
  pcolFocusCanto = glm::vec3(0.7, 0.0, 0.7);
  glUniform3fv(colFocusCanto, 1, &pcolFocusCanto[0]);
  mirant = false;
  dist = 1.0;
}
void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
      if(direcio == 360.0) direcio = 0.0;
      direcio = direcio + 45;
      if(direcio == 270 ) {
        mirant = true;
      }
      else mirant = false;
      if (!mirant){
        dist = dist + 0.5;
        if (mortyPos[0]-dist <= 0.5) resetejar();
      }
    break;
	}
  case Qt::Key_D: {

      if(direcio == 0.0) direcio = 360.0;

      direcio = direcio - 45;

      if(direcio == 270 ) mirant = true;
      else mirant = false;

      if (!mirant){
        dist = dist + 0.5;
        if (mortyPos[0]-dist <= 0.5) resetejar();
      }
    break;
	}
  case Qt::Key_M: {
      llumM = !llumM;
      if (llumM) pcolFocusLlanterna = glm::vec3(0.9, 0.9, 0.0);
      else pcolFocusLlanterna = glm::vec3(0.0, 0.0, 0.0);
      glUniform3fv(colFocusLlanterna, 1, &pcolFocusLlanterna[0]);
    break;
	}
  case Qt::Key_E: {
      llumE = !llumE;
      if(llumE) pcolFocusCanto = glm::vec3(0.7, 0.0, 0.7);
      else pcolFocusCanto = glm::vec3(0.0, 0.0, 0.0);
      glUniform3fv(colFocusCanto, 1, &pcolFocusCanto[0]);
    break;
	}
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  modelTransformTG();
  update();
}
