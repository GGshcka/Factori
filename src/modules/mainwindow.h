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

        mainView = new GLMDI();

        QMenuBar *menuBar = this->menuBar();

        setCentralWidget(mainView);

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

        gameMenuItem = menuBar->addMenu("Game");

        auto *editorMenuAction = new QAction("Editor", this);
        connect(settingsMenuAction, &QAction::triggered, this, [this]() {
            mainView->codeEditor->show();
        });
        gameMenuItem->addAction(editorMenuAction);

        buildMenuAction = new QAction("Build", this); //!TODO! Починить проблему с чекбоксом и подокнами
        buildMenuAction->setCheckable(true);
        buildMenuAction->setChecked(false);
        connect(buildMenuAction, &QAction::triggered, this, [this]() {
            if (buildMenuAction->isChecked()) {
                OpenGLScene::editMode = buildMenuAction->isChecked();
                mainView->execBuildSelector();
            } else mainView->buildSelector->close();
        });
        gameMenuItem->addAction(buildMenuAction);

        menuBar->addMenu(gameMenuItem);

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
    QMenu *systemMenuItem, *moodMenuItem, *gameMenuItem;
    GLMDI *mainView;
    QAction *buildMenuAction;
};


#endif //FACTORI_MAINWINDOW_H
