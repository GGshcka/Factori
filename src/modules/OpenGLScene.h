#ifndef FACTORI_OPENGLSCENE_H
#define FACTORI_OPENGLSCENE_H

#include <Qt>
#include <QtWidgets>
#include <QtGlobal>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "GLDrawableObject.h"

class OpenGLScene : public QOpenGLWidget, protected QOpenGLFunctions {
Q_OBJECT

public:
    OpenGLScene(QWidget *parent = nullptr) : QOpenGLWidget(parent), zoomValue(3.0) {
        grabKeyboard();
        movementTimer = new QTimer(this);
        connect(movementTimer, &QTimer::timeout, this, &OpenGLScene::updateMovement);
        movementTimer->start(1000/60);
        setMouseTracking(true);
    }
    ~OpenGLScene();

    GLDrawableObject *player;
    static bool editMode;

    void zoom(double value);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QVector<GLDrawableObject*> chunks;
    QVector<QPair<int, int>>* placedObjects;
    double zoomValue;
    int width, height;
    QSet<int> keysPressed;
    QTimer *movementTimer;
    int seed;
    int chunkSize = 2;
    QVector2D currentChunk;
    const float cellSize = 1.0f;
    QPointF mousePos, mouseWorldPos, mouseGridPos;

    void updateMovement();
    void chunkLoader();
    void drawGrid();
    void drawPlacedObjects();
};


#endif //FACTORI_OPENGLSCENE_H
