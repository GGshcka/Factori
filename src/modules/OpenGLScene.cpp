#include "OpenGLScene.h"

void OpenGLScene::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    player = new GLDrawableObject(new QOpenGLTexture(QImage(":/player/sprDown").mirrored()));
    player->appendTextureBuffer(new QOpenGLTexture(QImage(":/player/sprUp").mirrored()));
    player->appendTextureBuffer(new QOpenGLTexture(QImage(":/player/sprDown").mirrored()));
    player->appendTextureBuffer(new QOpenGLTexture(QImage(":/player/sprLeft").mirrored()));
    player->appendTextureBuffer(new QOpenGLTexture(QImage(":/player/sprRight").mirrored()));

    obj = new GLDrawableObject(new QOpenGLTexture(QImage(":/map/grass").mirrored()));
}

void OpenGLScene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(-player->getPosition().x(), -player->getPosition().y(), 0.0f);

    obj->draw();
    player->draw();
}

void OpenGLScene::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = float(w) / float(h);
    if (w >= h) {
        glOrtho(-2.0 * aspect, 2.0 * aspect, -2.0, 2.0, -1.0, 1.0);
    } else {
        glOrtho(-2.0, 2.0, -2.0 / aspect, 2.0 / aspect, -1.0, 1.0);
    }
    glMatrixMode(GL_MODELVIEW);
}