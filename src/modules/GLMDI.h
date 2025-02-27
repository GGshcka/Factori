#ifndef FACTORI_GLMDI_H
#define FACTORI_GLMDI_H

#include <Qt>
#include <QtWidgets>
#include <QtGlobal>
#include "OpenGLScene.h"

class GLMDI : public QWidget {
    OpenGLScene *glScene;
    QMdiArea *mdiArea;
    QMdiSubWindow *test;
    QLabel *debugInfo;

public:
    GLMDI(QWidget *parent = nullptr) : QWidget(parent) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        glScene = new OpenGLScene(this);

        mdiArea = new QMdiArea(this);
        mdiArea->setBackground(Qt::transparent);
        mdiArea->setGeometry(0, 0, this->width(), this->height());

        test = new QMdiSubWindow();
        test->setWindowTitle(QString("test"));
        test->setGeometry(mdiArea->width() - 200 - 20, 0, 200, mdiArea->height());
        //test->setWidget(new OpenGLScene());

        mdiArea->addSubWindow(test);

        debugInfo = new QLabel("", this);
        debugInfo->setStyleSheet("background-color: white;");

        movementTimer = new QTimer(this);
        connect(movementTimer, &QTimer::timeout, this, &GLMDI::updateMovement);
        movementTimer->start(1000/60);

    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        QWidget::resizeEvent(event);
        glScene->setFixedSize(this->width(), this->height());
        glScene->update();
        mdiArea->setFixedSize(this->width(), this->height());
        mdiArea->update();
        test->setGeometry(mdiArea->width() - 200, 0, 200, mdiArea->height());
    }

    void keyPressEvent(QKeyEvent *event) override {
        keysPressed.insert(event->key());
    }

    void keyReleaseEvent(QKeyEvent *event) override {
        keysPressed.remove(event->key());
    }


private:
    QSet<int> keysPressed;  // Храним нажатые клавиши
    QTimer *movementTimer;  // Таймер для обработки движения

    void updateDebugInfo() {
        QVector2D pos = glScene->player->getPosition();
        QString debugText = QString("Player POS | X: %1 | Y: %2").arg(pos.x()).arg(pos.y());

        debugInfo->setText(debugText);
        debugInfo->adjustSize();
    }

    void updateMovement() {
        float speed = 0.05f; // Скорость движения

        float dx = 0.0f;
        float dy = 0.0f;
        QOpenGLTexture *texturePath;

        if (keysPressed.contains(Qt::Key_W) || keysPressed.contains(1062)) {
            dy += speed;
            texturePath = glScene->player->getTextureFromBuffer(0);
        }
        if (keysPressed.contains(Qt::Key_S) || keysPressed.contains(1067)) {
            dy -= speed;
            texturePath = glScene->player->getTextureFromBuffer(1);
        }
        if (keysPressed.contains(Qt::Key_A) || keysPressed.contains(1060)) {
            dx -= speed;
            texturePath = glScene->player->getTextureFromBuffer(2);
        }
        if (keysPressed.contains(Qt::Key_D) || keysPressed.contains(1042)) {
            dx += speed;
            texturePath = glScene->player->getTextureFromBuffer(3);
        }

        // Обработка диагонального движения (нормализация скорости)
        if (dx != 0.0f && dy != 0.0f) {
            float normFactor = 1.0f / sqrt(2.0f);
            dx *= normFactor;
            dy *= normFactor;
        }

        if (dx != 0.0f || dy != 0.0f) {
            glScene->player->move(dx, dy);

            // Меняем текстуру только если движемся
            if (texturePath->isCreated()) {
                glScene->player->setTexture(texturePath);
            }

            glScene->update();
            updateDebugInfo();
        }
    }
};

#endif //FACTORI_GLMDI_H
