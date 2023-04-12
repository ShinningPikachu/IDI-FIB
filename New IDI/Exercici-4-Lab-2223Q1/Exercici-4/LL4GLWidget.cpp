#include "LL4GLWidget.h"

#include <iostream>

LL4GLWidget::LL4GLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  DoingInteractive = NONE;
}

LL4GLWidget::~LL4GLWidget ()
{
  if (program != NULL)
    delete program;
}

void LL4GLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  carregaShaders();

  creaBuffersMorty();
  creaBuffersFantasma();
  creaBuffersTerraIParets();

  iniEscena();
  iniCamera();
}

void LL4GLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.1,0.1,0.1);
  diff = glm::vec3(0.4,0.4,0.4);
  spec = glm::vec3(0.8,0.8,0.8);
  shin = 100;
}

void LL4GLWidget::iniEscena ()
{
  centreEsc = glm::vec3(5,3,5);
  radiEsc = 8;
}

void LL4GLWidget::iniCamera ()
{
  angleY = 0.0;
  viewTransform ();
}

void LL4GLWidget::paintGL ()
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
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma ();
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);


  glBindVertexArray(0);
}

void LL4GLWidget::resizeGL (int w, int h)
{
#ifdef __APPLE__
  // Aquest codi és necessari únicament per a MACs amb pantalla retina.
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
  ra = float(ample)/float(alt);
  projectTransform();
}

void LL4GLWidget::modelTransformMorty ()
{
  TG_morty = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG_morty = glm::rotate(TG_morty, float(glm::radians(90.0)),glm::vec3(0.f, 1.f, 0.f));
  TG_morty = glm::scale(TG_morty, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG_morty = glm::translate(TG_morty, -centreBaseMorty);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG_morty[0][0]);
}

void LL4GLWidget::modelTransformFantasma ()
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(1,0.5,5));
  TG = glm::rotate(TG, float(glm::radians(90.0)),glm::vec3(0.f, 1.f, 0.f));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::translate(TG, -centreBaseFantasma);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void LL4GLWidget::modelTransformTerra ()
{
  glm::mat4 TG = glm::mat4(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void LL4GLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(float(M_PI/3.0), ra, radiEsc, 3.0f*radiEsc);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}
void LL4GLWidget::viewTransform ()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void LL4GLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    default: event->ignore(); break;
  }
  update();
}

void LL4GLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void LL4GLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void LL4GLWidget::calculaCapsaModelMorty ()
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
}

void LL4GLWidget::creaBuffersMorty ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  morty.load("./models/Morty.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModelMorty ();

  // Creació del Vertex Array Object del Morty
  glGenVertexArrays(1, &VAO_Morty);
  glBindVertexArray(VAO_Morty);

  // Creació dels buffers del model fantasma
  GLuint VBO_Morty[6];
  // Buffer de posicions
  glGenBuffers(6, VBO_Morty);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*morty.faces().size()*3*3, morty.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*morty.faces().size()*3*3, morty.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*morty.faces().size()*3*3, morty.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*morty.faces().size()*3*3, morty.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*morty.faces().size()*3*3, morty.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*morty.faces().size()*3, morty.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void LL4GLWidget::calculaCapsaModelFantasma ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = fantasma.vertices()[0];
  miny = maxy = fantasma.vertices()[1];
  minz = maxz = fantasma.vertices()[2];
  for (unsigned int i = 3; i < fantasma.vertices().size(); i+=3)
  {
    if (fantasma.vertices()[i+0] < minx)
      minx = fantasma.vertices()[i+0];
    if (fantasma.vertices()[i+0] > maxx)
      maxx = fantasma.vertices()[i+0];
    if (fantasma.vertices()[i+1] < miny)
      miny = fantasma.vertices()[i+1];
    if (fantasma.vertices()[i+1] > maxy)
      maxy = fantasma.vertices()[i+1];
    if (fantasma.vertices()[i+2] < minz)
      minz = fantasma.vertices()[i+2];
    if (fantasma.vertices()[i+2] > maxz)
      maxz = fantasma.vertices()[i+2];
  }
  escalaFantasma = 1.5/(maxy-miny);

  centreBaseFantasma[0] = (minx+maxx)/2.0;
  centreBaseFantasma[1] = miny;
  centreBaseFantasma[2] = (minz+maxz)/2.0;
}

void LL4GLWidget::creaBuffersFantasma ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  fantasma.load("./models/Fantasma.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModelFantasma ();

  // Creació del Vertex Array Object del Fantasma
  glGenVertexArrays(1, &VAO_Fantasma);
  glBindVertexArray(VAO_Fantasma);

  // Creació dels buffers del model fantasma
  GLuint VBO_Fantasma[6];
  // Buffer de posicions
  glGenBuffers(6, VBO_Fantasma);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*fantasma.faces().size()*3*3, fantasma.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*fantasma.faces().size()*3*3, fantasma.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*fantasma.faces().size()*3*3, fantasma.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*fantasma.faces().size()*3*3, fantasma.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*fantasma.faces().size()*3*3, fantasma.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*fantasma.faces().size()*3, fantasma.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}


void LL4GLWidget::creaBuffersTerraIParets ()
{
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[30] = {
	glm::vec3(0.0, 0.0, 10.0),
	glm::vec3(10.0, 0.0, 10.0),
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(10.0, 0.0, 10.0),
	glm::vec3(10.0, 0.0, 0.0),

	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(10.0, 0.0, 0.0),
	glm::vec3(0.0, 6.0, 0.0),
	glm::vec3(0.0, 6.0, 0.0),
	glm::vec3(10.0, 0.0, 0.0),
	glm::vec3(10.0, 6.0, 0.0),

	glm::vec3(0.0, 0.0, 10.0),
	glm::vec3(0.0, 6.0, 10.0),
	glm::vec3(10.0, 0.0, 10.0),
	glm::vec3(0.0, 6.0, 10.0),
	glm::vec3(10.0, 6.0, 10.0),
	glm::vec3(10.0, 0.0, 10.0),

	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.0, 6.0, 0.0),
	glm::vec3(0.0, 0.0, 10.0),
	glm::vec3(0.0, 0.0, 10.0),
	glm::vec3(0.0, 6.0, 0.0),
	glm::vec3(0.0, 6.0, 10.0),

	glm::vec3(10.0, 6.0, 0.0),
	glm::vec3(10.0, 0.0, 0.0),
	glm::vec3(10.0, 0.0, 10.0),
	glm::vec3(10.0, 6.0, 10.0),
	glm::vec3(10.0, 6.0, 0.0),
	glm::vec3(10.0, 0.0, 10.0)

  };

  // VBO amb la normal de cada vèrtex
  glm::vec3 norm1 (0,1,0);
  glm::vec3 norm2 (0,0,1);
  glm::vec3 norm3 (0,0,-1);
  glm::vec3 norm4 (1,0,0);
  glm::vec3 norm5 (-1,0,0);
  glm::vec3 normterra[30] = {
	norm1, norm1, norm1, norm1, norm1, norm1, // la normal (0,1,0) per al terra
	norm2, norm2, norm2, norm2, norm2, norm2,  // la normal (0,0,1) per una paret
	norm3, norm3, norm3, norm3, norm3, norm3,  // la normal (0,0,-1) per la paret oposada
	norm4, norm4, norm4, norm4, norm4, norm4,  // la normal (1,0,0) per una paret
	norm5, norm5, norm5, norm5, norm5, norm5  // la normal (-1,0,0) per la paret oposada

  };


  iniMaterialTerra();

  // Fem que el material del terra afecti a tots els vèrtexs per igual
  glm::vec3 matambterra[30] = {
	amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffterra[30] = {
	diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspecterra[30] = {
	spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec
  };
  float matshinterra[30] = {
	shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[6];
  glGenBuffers(6, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambterra), matambterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffterra), matdiffterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspecterra), matspecterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshinterra), matshinterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void LL4GLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicLlumShader.frag");
  vs.compileSourceFile("shaders/basicLlumShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "Proj");
  viewLoc = glGetUniformLocation (program->programId(), "View");
}
