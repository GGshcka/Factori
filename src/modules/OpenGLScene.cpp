#include "OpenGLScene.h"
#include "PerlinNoiseGenerator.h"
#include "GLMDI.h"
#include "DebugInfoHandler.h"

bool OpenGLScene::editMode = false;

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

    placedObjects = new QVector<QPair<int, int>>();

    chunkLoader();
}

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

    for (GLDrawableObject* chunk : chunks) {
        chunk->draw();
    }

    drawPlacedObjects();
    if (editMode) drawGrid();
    player->draw();
}

void OpenGLScene::resizeGL(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

void OpenGLScene::zoom(double value) {
    zoomValue += value;
    zoomValue = std::clamp(zoomValue, 1.0, 15.0);
    update();
    chunkLoader();
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
    int minX = currentChunk.x() - (3 * zoomMultiplier) - 1;
    int maxX = currentChunk.x() + (3 * zoomMultiplier) + 1;
    int minY = currentChunk.y() - (2 * zoomMultiplier) - 1;
    int maxY = currentChunk.y() + (2 * zoomMultiplier) + 1;

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
    for (int i = -3 * zoomMultiplier; i <= 3 * zoomMultiplier; i++) {
        for (int j = -2 * zoomMultiplier; j <= 2 * zoomMultiplier; j++) {
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

void OpenGLScene::drawPlacedObjects() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    for (const auto &pos : *placedObjects) {
        auto *obj = new GLDrawableObject(new QOpenGLTexture(QImage(":/obj/sprTest").mirrored()));
        obj->setPosition(pos.first, pos.second);
        obj->draw();
    }

    glEnd();
}

OpenGLScene::~OpenGLScene() {
    chunks.clear();
    delete player;
}

//TODO Первый объект белый
void OpenGLScene::mousePressEvent(QMouseEvent *event) {
    if (editMode) {
        auto find = std::find(placedObjects->begin(), placedObjects->end(), qMakePair(mouseGridPos.x(), mouseGridPos.y()));
        if (find == placedObjects->end()) {
            placedObjects->append(qMakePair(mouseGridPos.x(), mouseGridPos.y()));
            qDebug() << "Appended:" << mouseGridPos.x() << "|" << mouseGridPos.y();
        }

        update();
    }
}

void OpenGLScene::drawGrid() {
    glColor3f(1.0f, 1.0f, 1.0f); // Серый цвет сетки
    glLineWidth(1.0f);
    glBegin(GL_LINES);

    float startX = std::round(player->getPosition().x()) - width / 2 - cellSize / 2;
    float endX = startX + width;
    float startY = std::round(player->getPosition().y()) - height / 2 - cellSize / 2;
    float endY = startY + height;

    // Вертикальные линии
    for (float x = startX; x <= endX; x += cellSize) {
        glVertex2f(x, startY);
        glVertex2f(x, endY);
    }

    // Горизонтальные линии
    for (float y = startY; y <= endY; y += cellSize) {
        glVertex2f(startX, y);
        glVertex2f(endX, y);
    }

    glEnd();

    glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Красный цвет квадрата
    glBegin(GL_QUADS);

    float aspect = float(width) / float(height);
    mouseWorldPos.setX(((2.0f * mousePos.x()) / width - 1.0f) * zoomValue * aspect + player->getPosition().x());
    mouseWorldPos.setY((1.0f - 2.0f * mousePos.y() / height) * zoomValue + player->getPosition().y());

    // Привязка к сетке
    mouseGridPos.setX(std::round(mouseWorldPos.x() / cellSize) * cellSize);
    mouseGridPos.setY(std::round(mouseWorldPos.y() / cellSize) * cellSize);

    /*qDebug() << "Mouse:" << mousePos.x() << mousePos.y();
    qDebug() << "World:" << mouseWorldPos.x() << mouseWorldPos.y();
    qDebug() << "Grid:" << mouseGridPos.x() << mouseGridPos.y();*/

    QPointF corrected(mouseGridPos.x() - cellSize/2, mouseGridPos.y() - cellSize/2);

    glVertex2f(corrected.x(), corrected.y());
    glVertex2f(corrected.x() + cellSize, corrected.y());
    glVertex2f(corrected.x() + cellSize, corrected.y() + cellSize);
    glVertex2f(corrected.x(), corrected.y() + cellSize);

    glEnd();
}

void OpenGLScene::mouseMoveEvent(QMouseEvent *event) {
    mousePos = event->pos();
    update();
}


