//
// Created by silly on 25.02.2025.
//

#ifndef FACTORI_GAMESCENE_H
#define FACTORI_GAMESCENE_H

#include <Qt>
#include <QtWidgets>
#include "player.h"

class GameScene : public QGraphicsScene {
public:
    explicit GameScene() {
        setSceneRect(-50000, -50000, 100000, 100000);
        setBackgroundBrush(QImage(":/map/grass"));

        cuteCatGirl = new Player(QPixmap(":/player/sprDown"));

        addItem(cuteCatGirl);
    }

    Player *cuteCatGirl;
};


#endif //FACTORI_GAMESCENE_H
