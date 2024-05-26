// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();
    
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);

    virtual void initializeGL ( );
    virtual void paintGL ( );
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void iniMaterialTerra ();
    virtual void viewTransform ();


    virtual void modelTransformTerra ();
    virtual void modelTransformRoad (glm::vec3 pos, float angle);
    virtual void modelTransformPipe ();
    virtual void modelTransformCar (float radi, float angle, int coche);

  public slots:
    void animar();
    
  private:
    int printOglError(const char file[], int line, const char func[]);
    float anglecar1 = 0;
    float anglecar2 = 0;
    GLuint colorLoc, poscoche1Loc, poscoche2Loc, encesLoc;
    QTimer timer;
    bool automatic = false;
    bool ences = true;
};
