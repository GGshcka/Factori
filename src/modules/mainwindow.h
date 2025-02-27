#ifndef FACTORI_MAINWINDOW_H
#define FACTORI_MAINWINDOW_H

#include <Qt>
#include <QtWidgets>
#include <QtGlobal>
#include "OpenGLScene.h"
#include "GLMDI.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
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

        /*mdiArea = new QMdiArea(this);
        mdiArea->setGeometry(0, 0, this->width(), this->height());

        glView = new OpenGLScene();
        glView->setAutoFillBackground(true);

        mdiArea->setViewport(glView);*/

        /*moodMenuItem = menuBar->addMenu("Moods");

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

        menuBar->addMenu(moodMenuItem);*/

        mainView = new GLMDI();

        setCentralWidget(mainView);
    }

protected:
    void keyPressEvent(QKeyEvent *event) override {
        switch (event->key()) {
            /*case Qt::Key_Plus:
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
                break;*/
            case Qt::Key_F1:
                menuBar()->setActiveAction(menuBar()->actions()[0]);
                break;
            /*case Qt::Key_F2:
                menuBar()->setActiveAction(menuBar()->actions()[1]);
                break;*/
        }
        QWidget::keyPressEvent(event);
    }

    void resizeEvent(QResizeEvent *event) override {
        QMainWindow::resizeEvent(event);
        mainView->setFixedSize(this->width(), this->height() - menuBar()->height());
        mainView->update();
//        test->setGeometry(width() - 200, 0, 200, mdiArea->height());
    }


private:
    QMdiArea *mdiArea;
    QMdiSubWindow *test;
    QMenu *systemMenuItem, *moodMenuItem;
    QGraphicsView *graphicsView;
    GLMDI *mainView;
    /*QGraphicsScene*//*GameScene *scene;
    Player *plr;*/
};


#endif //FACTORI_MAINWINDOW_H
