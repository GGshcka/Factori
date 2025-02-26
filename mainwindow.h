//
// Created by silly on 25.02.2025.
//

#ifndef FACTORI_MAINWINDOW_H
#define FACTORI_MAINWINDOW_H

#include <Qt>
#include <QtGlobal>
#include <QtWidgets>
#include "src/player.h"
#include "src/gamescene.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Factori");

        QMenuBar *menuBar = this->menuBar();

        systemMenuItem = menuBar->addMenu("System");

        auto *settingsMenuAction = new QAction("Settings", this);
        connect(settingsMenuAction, &QAction::triggered, this, [this]() {
            qDebug() << "!!!WIP!!!";
        });
        systemMenuItem->addAction(settingsMenuAction);

        auto *exitMenuAction = new QAction("Shutdown", this);
        connect(exitMenuAction, &QAction::triggered, this, &MainWindow::close);
        systemMenuItem->addAction(exitMenuAction);

        menuBar->addMenu(systemMenuItem);

        mdiArea = new QMdiArea(this);
        mdiArea->setGeometry(0, 0, this->width(), this->height());

        graphicsView = new QGraphicsView(mdiArea);

        mdiArea->setViewport(graphicsView);

        test = new QMdiSubWindow();
        test->setWindowTitle(QString("test"));
        test->setGeometry(width() - 200 - 20, 40, 200, 500);

        mdiArea->addSubWindow(test);

        /*scene = new QGraphicsScene();
        scene->setSceneRect(0, 0, this->width(), this->height());
        scene->setBackgroundBrush(QBrush(Qt::lightGray));

        plr = new Player(QPixmap(":/player/sprDown"));
        scene->addItem(plr);*/

        scene = new GameScene();

        graphicsView->setScene(scene);
        graphicsView->setRenderHint(QPainter::Antialiasing);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        moodMenuItem = menuBar->addMenu("Moods");

        auto *moodLoveMenuAction = new QAction("Love", this);
        connect(moodLoveMenuAction, &QAction::triggered, this, [this]() {
            scene->cuteCatGirl->showMood(1.0, "Love");
        });
        moodMenuItem->addAction(moodLoveMenuAction);

        auto *moodSadMenuAction = new QAction("Sad", this);
        connect(moodSadMenuAction, &QAction::triggered, this, [this]() {
            scene->cuteCatGirl->showMood(1.0, "Sad");
        });
        moodMenuItem->addAction(moodSadMenuAction);

        auto *moodAnnoyedMenuAction = new QAction("Annoyed", this);
        connect(moodAnnoyedMenuAction, &QAction::triggered, this, [this]() {
            scene->cuteCatGirl->showMood(1.0, "Annoyed");
        });
        moodMenuItem->addAction(moodAnnoyedMenuAction);

        menuBar->addMenu(moodMenuItem);

        setCentralWidget(mdiArea);
    }

protected:
    void keyPressEvent(QKeyEvent *event) override {
        switch (event->key()) {
            case Qt::Key_Plus:
            case Qt::Key_Equal:
                graphicsView->scale(1.2, 1.2);
                break;
            case Qt::Key_Minus:
                graphicsView->scale(0.8, 0.8);
                break;
            case 87:
            case 1062:
                scene->cuteCatGirl->setY(scene->cuteCatGirl->y()-(scene->cuteCatGirl->pixmap().height()/10.0));
                scene->cuteCatGirl->setPixmap(QPixmap(":/player/sprUp"));
                break;
            case 83:
            case 1067:
                scene->cuteCatGirl->setY(scene->cuteCatGirl->y()+(scene->cuteCatGirl->pixmap().height()/10.0));
                scene->cuteCatGirl->setPixmap(QPixmap(":/player/sprDown"));
                break;
            case 65:
            case 1060:
                scene->cuteCatGirl->setX(scene->cuteCatGirl->x()-(scene->cuteCatGirl->pixmap().width()/10.0));
                scene->cuteCatGirl->setPixmap(QPixmap(":/player/sprLeft"));
                break;
            case 68:
            case 1042:
                scene->cuteCatGirl->setX(scene->cuteCatGirl->x()+(scene->cuteCatGirl->pixmap().width()/10.0));
                scene->cuteCatGirl->setPixmap(QPixmap(":/player/sprRight"));
                break;
            case Qt::Key_F1:
                menuBar()->setActiveAction(menuBar()->actions()[0]);
                break;
            case Qt::Key_F2:
                menuBar()->setActiveAction(menuBar()->actions()[1]);
                break;
        }
        //qDebug() << "INFO: KeyPressed:" << char (event->key()) << "|" << event->key();
        graphicsView->centerOn(scene->cuteCatGirl);
        graphicsView->update();
        QWidget::keyPressEvent(event);
    }

    void resizeEvent(QResizeEvent *event) override {
        QMainWindow::resizeEvent(event);
        /*mdiArea->update();
        graphicsView->setGeometry(mdiArea->viewport()->rect());
        graphicsView->update();
        scene->update();*/
        mdiArea->setViewport(graphicsView);
        test->setGeometry(width() - 200 - 20, 40, 200, 500);
    }


private:
    QMdiArea *mdiArea;
    QMdiSubWindow *test;
    QMenu *systemMenuItem, *moodMenuItem;
    QGraphicsView *graphicsView;
    /*QGraphicsScene*/GameScene *scene;
    Player *plr;
};


#endif //FACTORI_MAINWINDOW_H
