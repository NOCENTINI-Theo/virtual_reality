#ifndef VRWIDGET_H
#define VRWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <QMouseEvent>

class VRWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    VRWidget(QWidget *parent = nullptr);
    ~VRWidget();


private :
    QOpenGLBuffer vbo;
    QOpenGLShaderProgram program;
    QOpenGLVertexArrayObject vao;
    float angleY=0;
    float angleX=0;
    QTimer* timer = new QTimer();
    QPointF oldpos;
    bool animationActivated=false;
    bool discoActivated=false;
    int fov = 45;
    int opacity = 100;
    QVector3D couleur1;
    QVector3D couleur2;
    QVector3D couleur3;
    QVector3D couleur4;
    QVector3D couleur5;
    QVector3D couleur6;
    int animationSpeed=1;



private slots:
    void update();

public slots:
    void startAnimation();
    void changeFov(int val);
    void changeOpacity(int newOpacity);
    void startDisco();
    void changeAnimationSpeed(int newAnimationSpeed);


protected :
    void initializeBuffer();
    void initializeShader();
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    // QOpenGLWidget interface
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

};
#endif // VRWIDGET_H
