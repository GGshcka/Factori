#ifndef FACTORI_GLDRAWABLEOBJECT_H
#define FACTORI_GLDRAWABLEOBJECT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QVector3D>

class GLDrawableObject : public QObject, protected QOpenGLFunctions {
Q_OBJECT

public:
    explicit GLDrawableObject(QOpenGLTexture *objTexture, QObject *parent = nullptr);

    void setPosition(float x, float y);
    void move(float dx, float dy);
    QVector2D getPosition() const;

    void setSize(float w, float h);
    void setRotation(float angle);
    void rotate(float angle);
    void setOpacity(float alpha);
    void setTint(QColor color);
    void setTexture(QOpenGLTexture *newTexture);
    void draw();

    void appendTextureBuffer(QOpenGLTexture *newTexture);
    void removeFromTextureBuffer(int id);
    QOpenGLTexture* getTextureFromBuffer(int id);

private:
    QOpenGLTexture *texture;
    QVector<QOpenGLTexture*> texturesBuffer;
    float x, y;
    float width, height;
    float rotation;
    float opacity;
    QColor tint;
};

#endif // FACTORI_GLDRAWABLEOBJECT_H
