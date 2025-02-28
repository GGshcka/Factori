#include "OpenGLScene.h"
#include "PerlinNoiseGenerator.h"
#include "GLMDI.h"

void OpenGLScene::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    seed = QDateTime::currentMSecsSinceEpoch() / 1000;

    player = new GLDrawableObject(new QOpenGLTexture(QImage(":/player/sprDown").mirrored()));
    player->appendTextureBuffer(new QOpenGLTexture(QImage(":/player/sprUp").mirrored()));
    player->appendTextureBuffer(new QOpenGLTexture(QImage(":/player/sprDown").mirrored()));
    player->appendTextureBuffer(new QOpenGLTexture(QImage(":/player/sprLeft").mirrored()));
    player->appendTextureBuffer(new QOpenGLTexture(QImage(":/player/sprRight").mirrored()));

    /*auto* generator = new PerlinNoiseGenerator(0, 0, 50, 50, *//*QDateTime::currentMSecsSinceEpoch() / 1000*//*69);
    QOpenGLTexture* texture = generator->generateTexture();
    mapObj = new GLDrawableObject(texture);

    mapObj->setSize(texture->width() / (32 * 2), texture->height() / (32 * 2));*/

    chunkLoader();
}   //new QOpenGLTexture(QImage(":/map/grass").mirrored())

void OpenGLScene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = float(width) / float(height);
    if (width >= height) {
        glOrtho(-zoomValue * aspect, zoomValue * aspect, -zoomValue, zoomValue, -1.0, 1.0);
    } else {
        glOrtho(-zoomValue, zoomValue, -zoomValue / aspect, zoomValue / aspect, -1.0, 1.0);
    }
    glMatrixMode(GL_MODELVIEW);

    glTranslatef(-player->getPosition().x(), -player->getPosition().y(), 0.0f);

    //mapObj->draw();

    for (GLDrawableObject* chunk : chunks) {
        chunk->draw();
    }

    player->draw();

    /*QVector2D pos = player->getPosition();
    QVector2D size = mapObj->getSize();
    QString debugText = QString(
            "Map INFO | W: %1 | H: %2"
            "\nPlayer POS | X: %3 | Y: %4").arg(size.x()).arg(size.y()).arg(pos.x()).arg(pos.y());*/
}

void OpenGLScene::resizeGL(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

void OpenGLScene::zoom(double value) {
    zoomValue += value;
    zoomValue = std::clamp(zoomValue, 0.5, 10.0);
    update();
}

void OpenGLScene::keyPressEvent(QKeyEvent *event) {
    keysPressed.insert(event->key());
}

void OpenGLScene::keyReleaseEvent(QKeyEvent *event) {
    keysPressed.remove(event->key());
}

void OpenGLScene::updateMovement() {
    if (keyboardGrabber()) {
        float speed = 0.05f; // Скорость движения

        float dx = 0.0f;
        float dy = 0.0f;
        QOpenGLTexture *texturePath;

        if (keysPressed.contains(Qt::Key_W) || keysPressed.contains(1062)) {
            dy += speed;
            texturePath = player->getTextureFromBuffer(0);
        }
        if (keysPressed.contains(Qt::Key_S) || keysPressed.contains(1067)) {
            dy -= speed;
            texturePath = player->getTextureFromBuffer(1);
        }
        if (keysPressed.contains(Qt::Key_A) || keysPressed.contains(1060)) {
            dx -= speed;
            texturePath = player->getTextureFromBuffer(2);
        }
        if (keysPressed.contains(Qt::Key_D) || keysPressed.contains(1042)) {
            dx += speed;
            texturePath = player->getTextureFromBuffer(3);
        }

        if (keysPressed.contains(Qt::Key_Plus) || keysPressed.contains(Qt::Key_Equal)) {
            zoom(-0.1);
        }
        if (keysPressed.contains(Qt::Key_Minus)) {
            zoom(0.1);
        }

        // Обработка диагонального движения (нормализация скорости)
        if (dx != 0.0f && dy != 0.0f) {
            float normFactor = 1.0f / sqrt(2.0f);
            dx *= normFactor;
            dy *= normFactor;
        }

        if (dx != 0.0f || dy != 0.0f) {
            player->move(dx, dy);

            if (texturePath->isCreated()) {
                player->setTexture(texturePath);
            }

            if (currentChunk != QVector2D(std::trunc(player->getPosition().x()), std::trunc(player->getPosition().y()))) {
                chunkLoader();
            }

            update();
        }
    } else { keysPressed.clear(); }
}

void OpenGLScene::chunkLoader() {
    QVector2D plPos = player->getPosition();

    currentChunk.setX(std::trunc(plPos.x()));
    currentChunk.setY(std::trunc(plPos.y()));

    int zoomMultiplier = static_cast<int>(std::trunc(zoomValue));

    // Границы области рендеринга чанков
    int minX = currentChunk.x() - (4 * zoomMultiplier);
    int maxX = currentChunk.x() + (4 * zoomMultiplier);
    int minY = currentChunk.y() - (4 * zoomMultiplier);
    int maxY = currentChunk.y() + (4 * zoomMultiplier);

    // Создаем временный контейнер для новых чанков
    QHash<QPair<int, int>, GLDrawableObject*> updatedChunks;

    // Проверяем существующие чанки и оставляем только нужные
    for (auto it = chunks.begin(); it != chunks.end();) {
        GLDrawableObject* chunk = *it;
        QPair<int, int> chunkCoords(chunk->getPosition().x(), chunk->getPosition().y());

        if (chunkCoords.first >= minX && chunkCoords.first <= maxX &&
            chunkCoords.second >= minY && chunkCoords.second <= maxY) {
            // Если чанк остается в зоне рендеринга, сохраняем его
            updatedChunks.insert(chunkCoords, chunk);
            ++it;
        } else {
            // Если чанк вышел за границы, удаляем его
            delete chunk;
            it = chunks.erase(it);
        }
    }

    // Создаем недостающие чанки
    for (int i = -4 * zoomMultiplier; i <= 4 * zoomMultiplier; i++) {
        for (int j = -4 * zoomMultiplier; j <= 4 * zoomMultiplier; j++) {
            int chunkX = currentChunk.x() + i;
            int chunkY = currentChunk.y() + j;
            QPair<int, int> chunkCoords(chunkX, chunkY);

            if (!updatedChunks.contains(chunkCoords)) {
                auto* generator = new PerlinNoiseGenerator(chunkX, chunkY, chunkSize, chunkSize, seed);
                QOpenGLTexture* texture = generator->generateTexture();
                auto* chunk = new GLDrawableObject(texture);

                if (i == 0 && j == 0) {
                    chunk->setPosition(chunkX, chunkY);
                } else {
                    chunk->setPosition(chunkX + (chunkSize/2 * i) - i, chunkY + (chunkSize/2 * j) - j);
                }

                chunk->setSize(texture->width() / (32 * 2), texture->height() / (32 * 2));
                updatedChunks.insert(chunkCoords, chunk);

                delete generator;
            }
        }
    }

    // Обновляем основной контейнер чанков
    chunks = updatedChunks.values();
}

