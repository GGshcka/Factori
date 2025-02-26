//
// Created by silly on 25.02.2025.
//

#ifndef FACTORI_PLAYER_H
#define FACTORI_PLAYER_H

#include "GraphicalGameObject.h"

class Player : public GraphicalGameObject {
Q_OBJECT
public:
    explicit Player(QPixmap pixmap, QGraphicsItem *parent = nullptr)
    : GraphicalGameObject(pixmap, parent) {}
};

#endif //FACTORI_PLAYER_H
