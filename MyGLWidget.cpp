// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent) : LL4GLWidget(parent) {

}

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

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
      anglecar1 += 2;
      anglecar2 += 3;
    break;
	}
  case Qt::Key_L: {
      if (ences) ences = false;
      else ences = true;
      glUniform1i(encesLoc, ences);
      if(!ences) {
        glClearColor(0.3,0.3,0.3,1);
      }
      else glClearColor(0.5,0.7,1,1);
    break;
	}
  case Qt::Key_S: {
      if(automatic) {
        timer.stop();
        automatic = false;
      }
      else {
        timer.start(50);
        automatic = true;
      }
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::initializeGL ()
{
  LL4GLWidget::initializeGL();
  colorLoc = glGetUniformLocation (program->programId(), "color");
  poscoche1Loc = glGetUniformLocation (program->programId(), "poscoche1");
  poscoche2Loc = glGetUniformLocation (program->programId(), "poscoche2");
  encesLoc = glGetUniformLocation (program->programId(), "boolean");
  glUniform1i(encesLoc, ences);
  connect(&timer, SIGNAL(timeout()), this, SLOT(animar()));  
}

void MyGLWidget::animar() {
  makeCurrent();
  anglecar1 += 2;
  anglecar2 += 3;
  update();
}

void MyGLWidget::iniEscena ()
{
  centreEsc = glm::vec3(0,2.5,0);
  radiEsc = 17.85;  
}

void MyGLWidget::iniCamera ()
{
  angleY = 0.0;

  viewTransform ();
}

void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.1,0.1,0.1);
  diff = glm::vec3(0.0,0.5,0.5);
  spec = glm::vec3(0.8,0.8,0.8);
  shin = 100;
}

void MyGLWidget::viewTransform ()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, glm::radians(float(32.0)), glm::vec3(1, 0, 0));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::paintGL () 
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::vec3 color = glm::vec3(1,1,1);
  glUniform3fv(colorLoc, 1, &color[0]);

  // Car
  glBindVertexArray (VAO_models[CAR]);
  color = glm::vec3(1,0,0);
  glUniform3fv(colorLoc, 1, &color[0]);
  modelTransformCar (6.5, anglecar1, 1);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);
  color = glm::vec3(0,1,0);
  glUniform3fv(colorLoc, 1, &color[0]);
  modelTransformCar (9, anglecar2, 2);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);
  
  color = glm::vec3(1,1,1);
  glUniform3fv(colorLoc, 1, &color[0]);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  // Road
  glBindVertexArray (VAO_models[ROAD]);
  modelTransformRoad (glm::vec3(5, 0.01, -5), 0.0f);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  modelTransformRoad (glm::vec3(-5, 0.01, -5), 90);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  modelTransformRoad (glm::vec3(-5, 0.01, 5), 180);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  modelTransformRoad (glm::vec3(5, 0.01, 5), -90);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  
  // Pipe
  glBindVertexArray (VAO_models[PIPE]);
  modelTransformPipe ();
  glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

  
  
  
  
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformTerra () {
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(2.5, 2.5, 2.5));
  TG = glm::translate(TG, glm::vec3(-5, 0, -5));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPipe ()
{
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(5,5,5));
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE]));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformRoad (glm::vec3 pos, float angle)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(5,5,5));
  TG = glm::scale(TG, glm::vec3(escalaModels[ROAD]));
  TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseModels[ROAD]);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCar (float radi, float angle, int coche)
{
  glm::mat4 TG(1.0f);
  TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(radi,0,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[CAR]));
  TG = glm::translate(TG, -centreBaseModels[CAR]);

  glm::vec4 posllum = (View * TG * glm::vec4(2.48,0.4,-3.2,1));

  if (coche == 1) {
    glUniform4fv(poscoche1Loc, 1, &posllum[0]);
  }
  else {
    glUniform4fv(poscoche2Loc, 1, &posllum[0]);
  }
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}