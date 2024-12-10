#include "vrwidget.h"
#include <QVector3D>
#include <QMessageBox>
#include <iostream>

VRWidget::VRWidget(QWidget *parent)
    : QOpenGLWidget(parent), QOpenGLFunctions()
{}

VRWidget::~VRWidget() {}

void VRWidget::initializeBuffer()
{
    //Définition des 4 sommets
    QVector3D A(0, 0, 1);
    QVector3D B(+1, 0, 1);
    QVector3D C(+1, 1, 1);
    QVector3D D(0, 1, 1);
    QVector3D E(0, 0, 0);
    QVector3D F(+1, 0, 0);
    QVector3D G(+1, 1, 0);
    QVector3D H(0, 1, 0);


    //Définition de 4 couleurs
    QVector3D rouge(1, 0, 0);
    QVector3D noir(0, 0, 0);
    QVector3D bleu(0, 0, 1);
    QVector3D blanc(1, 1, 1);
    QVector3D vert(0, 1, 0);
    QVector3D magenta(1, 0, 1);
    QVector3D cyan(0, 1, 1);
    QVector3D jaune(1, 1, 0);
    couleur1 = QVector3D(1.0f, 0.0f, 0.0f);
    couleur2 = QVector3D(0.0f, 0.0f, 1.0f);
    couleur3 = QVector3D(1.0f, 0.0f, 1.0f);
    couleur4 = QVector3D(0.0f, 1.0f, 1.0f);
    couleur5 = QVector3D(0.0f, 1.0f, 0.0f);
    couleur6 = QVector3D(1.0f, 1.0f, 0.0f);

    //On prépare le tableau des données
    QVector3D vertexData[] = {
        A, B, C, D,
        B,F,G,C,
        D,C,G,H,
        D,A,E,H,
        A,E,F,B,
        E,H,G,F,
        couleur1,couleur1,couleur1,couleur1,
        couleur2,couleur2,couleur2,couleur2,
        couleur3,couleur3,couleur3,couleur3,
        couleur4,couleur4,couleur4,couleur4,
        couleur5,couleur5,couleur5,couleur5,
        couleur6,couleur6,couleur6,couleur6,

    };

    //Initialisation du Vertex Buffer Object
    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    //Création du VBO
    vbo.create();
    //Lien du VBO avec le contexte de rendu OpenGL
    vbo.bind();
    //Allocation des données dans le VBO
    vbo.allocate(vertexData, sizeof (vertexData));
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    //Libération du VBO
    vbo.release();
}

void VRWidget::initializeShader()
{
    bool resultat;
    program.create();
    program.bind();

    //Vertex Shader
    resultat = program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/VS_simple.vsh");
    if (!resultat)     {
        QMessageBox msg;
        msg.setWindowTitle("Vertex shader");
        msg.setText(program.log());
        msg.exec();
        exit(EXIT_FAILURE);
    }

    //Fragment Shader
    resultat = program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/FS_simple.fsh");
    if (!resultat)     {
        QMessageBox msg;
        msg.setWindowTitle("Fragment shader");
        msg.setText(program.log());
        msg.exec();
        exit(EXIT_FAILURE);
    }

    //Link
    resultat = program.link();
    if (!resultat)     {
        QMessageBox msg;
        msg.setWindowTitle("Link du shader program");
        msg.setText(program.log());
        msg.exec();
        exit(EXIT_FAILURE);
    }

    vao.create();
    vao.bind();
    vbo.bind();

    program.setAttributeBuffer("rv_Position", GL_FLOAT, 0, 3);
    program.enableAttributeArray("rv_Position");

    program.setAttributeBuffer("rv_Color", GL_FLOAT, sizeof(QVector3D) * 24, 3);
    program.enableAttributeArray("rv_Color");


    //Libération
    vao.release();
    program.release();
}


void VRWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.566f, 0.867f, 1.0f);

    initializeBuffer();
    initializeShader();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}

void VRWidget::resizeGL(int w, int h)
{
    //transformation de viewport
    glViewport(0, 0, w, h);
}

void VRWidget::paintGL()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 model, proj, view, matrix;
    float wd= width();
    float ht = height();
    //Définition de la matrice de projetcion
    proj.perspective(fov, wd/ht, 0.1f, 100.0f);

    //Définiion de la matrice de vue
    view  = QMatrix4x4();

    //Définition de la matrice de placement
    model.translate(0, 0, -3);//T2
    model.rotate(angleX ,1,0,0); //R2
    model.rotate(angleY,0,1,0); //R1
    model.translate(-0.5, -0.5, -0.5); //T1




    //Produit des trois matrices (dans ce sens!)
    matrix = proj * view * model;

    program.bind();
    program.setUniformValue("u_ModelViewProjectionMatrix", matrix);
    float opac = opacity;
    std::cout<<opac/100<<std::endl;
    program.setUniformValue("u_Opacity",opac/100);
    vao.bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

    vao.release();

    program.release();

}

void VRWidget::update()
{
    if(animationActivated){
        angleY=angleY + animationSpeed;
    }
    if(discoActivated){
        angleY = angleY+90;
        //opacity =(25 *sin(angleY))+75;
    }

    QOpenGLWidget::update();
}

void VRWidget::mousePressEvent(QMouseEvent* event)
{
    oldpos = event->position();
}

void VRWidget::mouseMoveEvent(QMouseEvent* event)
{
    float dx = (event->position().x() - oldpos.x())/width();
    float dy = (event->position().y() - oldpos.y())/height();
    angleX = angleX + dy*180;
    angleY = angleY + dx*180;
    oldpos = event->position();
    QOpenGLWidget::update();
}

void VRWidget::startAnimation()
{
    if(!animationActivated){
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(30);
        animationActivated = true;
    }
    else{
        timer->stop();
        timer = new QTimer();
        animationActivated = false;
    }
}

void VRWidget::changeFov(int val)
{
    fov = val;
    QOpenGLWidget::update();
}

void VRWidget::changeOpacity(int newOpacity)
{
    opacity = newOpacity;
    QOpenGLWidget::update();

}

void VRWidget::startDisco()
{
    if(!discoActivated){
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(30);
        discoActivated = true;
    }
    else{
        timer->stop();
        timer = new QTimer();
        discoActivated = false;
    }

}

void VRWidget::changeAnimationSpeed(int newAnimationSpeed)
{
    animationSpeed = newAnimationSpeed;
}
