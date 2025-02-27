#ifndef FACTORI_OPENGLSCENE_H
#define FACTORI_OPENGLSCENE_H

#include <Qt>
#include <QtWidgets>
#include <QtGlobal>
#include <QOpenGLWidget>
#include "GLDrawableObject.h"

class OpenGLScene : public QOpenGLWidget, protected QOpenGLFunctions {
Q_OBJECT

public:
    OpenGLScene(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}
    GLDrawableObject *player;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    //void keyPressEvent(QKeyEvent *event) override;

private:
    GLDrawableObject *obj;
    bool initialized = false;
};


#endif //FACTORI_OPENGLSCENE_H
