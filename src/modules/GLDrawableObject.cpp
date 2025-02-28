#include "GLDrawableObject.h"

GLDrawableObject::GLDrawableObject(QOpenGLTexture *objTexture, QObject *parent)
        : QObject(parent), texture(objTexture), x(0), y(0), width(1), height(1), rotation(0), opacity(1.0), tint(qRgb(255,255,255)) {
    initializeOpenGLFunctions();
}

void GLDrawableObject::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void GLDrawableObject::move(float dx, float dy) {
    x += dx;
    y += dy;
}

QVector2D GLDrawableObject::getPosition() const {
    return {x, y};
}

void GLDrawableObject::setSize(float w, float h) {
    width = w;
    height = h;
}
QVector2D GLDrawableObject::getSize() const {
    return {width, height};
}

void GLDrawableObject::setRotation(float angle) {
    rotation = angle;
}
void GLDrawableObject::rotate(float angle) {
    rotation += angle;
}

void GLDrawableObject::setOpacity(float alpha) {
    opacity = alpha;
}

void GLDrawableObject::setTint(QColor color) {
    tint = color;
}

void GLDrawableObject::setTexture(QOpenGLTexture *newTexture) {
    texture = newTexture;
}

void GLDrawableObject::appendTextureBuffer(QOpenGLTexture *newTexture) {
    texturesBuffer.append(newTexture);
}

void GLDrawableObject::removeFromTextureBuffer(int id) {
    if (texturesBuffer.length() - 1 >= id) {
        texturesBuffer.remove(id);
    }
}

QOpenGLTexture* GLDrawableObject::getTextureFromBuffer(int id) {
    if (texturesBuffer[id] != nullptr && texturesBuffer.length() - 1 >= id) {
        return texturesBuffer[id];
    }
    return nullptr;
}

void GLDrawableObject::draw() {
    if (!texture || !QOpenGLContext::currentContext()) {
        qDebug() << "Ошибка: текстура не загружена или нет OpenGL-контекста!";
        return;
    }

    texture->bind();

    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(rotation, 0, 0, 1);
    glScalef(width, height, 1);

    glColor4f(tint.redF(), tint.greenF(), tint.blueF(), opacity);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-0.5, -0.5);
    glTexCoord2f(1, 0); glVertex2f(0.5, -0.5);
    glTexCoord2f(1, 1); glVertex2f(0.5, 0.5);
    glTexCoord2f(0, 1); glVertex2f(-0.5, 0.5);
    glEnd();

    glPopMatrix();
    texture->release();
}
