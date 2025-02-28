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
    OpenGLScene(QWidget *parent = nullptr) : QOpenGLWidget(parent), zoomValue(3.0) {
        grabKeyboard();
        movementTimer = new QTimer(this);
        connect(movementTimer, &QTimer::timeout, this, &OpenGLScene::updateMovement);
        movementTimer->start(1000/60);
    }
    GLDrawableObject *player;

    void zoom(double value);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QVector<GLDrawableObject*> chunks;
    double zoomValue;
    int width, height;
    QSet<int> keysPressed;
    QTimer *movementTimer;
    int seed;
    int chunkSize = 2;
    QVector2D currentChunk;
    QVector2D lastChunk;

    void updateMovement();
    void chunkLoader();
};


#endif //FACTORI_OPENGLSCENE_H
