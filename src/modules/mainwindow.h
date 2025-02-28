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

        mainView = new GLMDI();

        setCentralWidget(mainView);

        QFile sfile(":/style");
        if(!sfile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Cannot open stylesheet file.";
            return;
        }
        QString stylesheet = QString::fromUtf8(sfile.readAll());
        this->setStyleSheet(stylesheet);
    }

protected:
    void keyPressEvent(QKeyEvent *event) override {
        switch (event->key()) {
            case Qt::Key_F1:
                menuBar()->setActiveAction(menuBar()->actions()[0]);
                break;
        }
        QWidget::keyPressEvent(event);
    }

    void resizeEvent(QResizeEvent *event) override {
        QMainWindow::resizeEvent(event);
        mainView->setFixedSize(this->width(), this->height() - menuBar()->height());
        mainView->update();
    }


private:
    QMdiArea *mdiArea;
    QMdiSubWindow *test;
    QMenu *systemMenuItem, *moodMenuItem;
    QGraphicsView *graphicsView;
    GLMDI *mainView;
};


#endif //FACTORI_MAINWINDOW_H
