#ifndef FACTORI_GLMDI_H
#define FACTORI_GLMDI_H

#include <Qt>
#include <QtWidgets>
#include <QtGlobal>
#include "OpenGLScene.h"
#include "CodeHighlighter.h"
#include "ActionWindow.h"

class GLMDI : public QWidget {
public:
    GLMDI(QWidget *parent = nullptr) : QWidget(parent) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        mdiArea = new QMdiArea(this);
        mdiArea->setBackground(Qt::transparent);
        mdiArea->setFocusPolicy(Qt::NoFocus);
        mdiArea->setTabsClosable(false);
        mdiArea->setMouseTracking(true);

        glScene = new OpenGLScene();
        subWindow = mdiArea->addSubWindow(glScene);
        subWindow->setWindowFlag(Qt::FramelessWindowHint);
        subWindow->setGeometry(mdiArea->rect()); // Растягиваем на весь MdiArea
        mdiArea->setActiveSubWindow(subWindow);  // Делаем активным
        subWindow->show();


        /*edit = new QTextEdit();
        codeEditor = new ActionWindow();
        codeEditor->setAttribute(Qt::WA_Hover, true);
        codeEditor->setAttribute(Qt::WA_MouseTracking, true);
        codeEditor->installEventFilter(this);
        codeEditor->setWindowTitle(QString("Edit logic: none"));
        codeEditor->setWidget(edit);
        new CodeHighlighter(edit->document());
        edit->setTabStopDistance(12);

        mdiArea->addSubWindow(codeEditor);*/

        mdiArea->clearFocus();
    }

    void execBuildSelector() {
        buildSelector = new ActionWindow();
        buildSelector->setAttribute(Qt::WA_Hover, true);
        buildSelector->setAttribute(Qt::WA_MouseTracking, true);
        buildSelector->installEventFilter(this);
        buildSelector->setWindowTitle(QString("Selector"));
        buildSelector->setWindowFlag( Qt::WindowStaysOnTopHint);

        connect(buildSelector, &ActionWindow::closing, this, [this]() {
            mdiArea->removeSubWindow(buildSelector);
            OpenGLScene::editMode = false;
            glScene->update();
        });

        mdiArea->addSubWindow(buildSelector);

        glScene->update();
        buildSelector->show();

        buildSelector->setGeometry(mdiArea->width() - 400 - 50, mdiArea->height()/2/2, 400, mdiArea->height()/2);
    }

    QMdiArea *mdiArea;
    OpenGLScene *glScene;
    ActionWindow *codeEditor, *buildSelector;

protected:
    void resizeEvent(QResizeEvent *event) override {
        QWidget::resizeEvent(event);
        glScene->setFixedSize(this->width(), this->height());
        glScene->update();
        mdiArea->setFixedSize(this->width(), this->height());
        mdiArea->update();
        subWindow->setGeometry(mdiArea->rect());
        subWindow->update();
        //codeEditor->setGeometry(mdiArea->width() - 400 - 50, mdiArea->height()/2/2, 400, mdiArea->height()/2);
        if (buildSelector != nullptr) {
            buildSelector->setGeometry(mdiArea->width() - 400 - 50, mdiArea->height()/2/2, 400, mdiArea->height()/2);
        }
    }

    bool eventFilter(QObject *watched, QEvent *event) override {
        if (event->type() == QEvent::HoverEnter) {
            glScene->releaseKeyboard();
            //glScene->releaseMouse();
        } else if (event->type() == QEvent::HoverLeave) {
            glScene->grabKeyboard();
            //glScene->grabMouse();
        }
        return QWidget::eventFilter(watched, event);
    }

    QTextEdit *edit;
    QMdiSubWindow *subWindow;
};

#endif //FACTORI_GLMDI_H
