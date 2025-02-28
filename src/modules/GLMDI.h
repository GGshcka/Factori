#ifndef FACTORI_GLMDI_H
#define FACTORI_GLMDI_H

#include <Qt>
#include <QtWidgets>
#include <QtGlobal>
#include "OpenGLScene.h"
#include "CodeHighlighter.h"

class GLMDI : public QWidget {
    OpenGLScene *glScene;
    QMdiArea *mdiArea;
    QMdiSubWindow *codeEditor;
    QTextEdit *edit;

public:
    GLMDI(QWidget *parent = nullptr) : QWidget(parent) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        glScene = new OpenGLScene(this);

        mdiArea = new QMdiArea(this);
        mdiArea->setBackground(Qt::transparent);
        mdiArea->setGeometry(0, 0, this->width(), this->height());
        mdiArea->setFocusPolicy(Qt::NoFocus);
        mdiArea->setTabsClosable(false);

        edit = new QTextEdit();
        codeEditor = new QMdiSubWindow();
        codeEditor->setAttribute(Qt::WA_Hover, true);
        codeEditor->installEventFilter(this);
        codeEditor->setWindowTitle(QString("Edit logic: none"));
        codeEditor->setWidget(edit);
        new CodeHighlighter(edit->document());
        edit->setTabStopDistance(12);

        //mdiArea->addSubWindow(codeEditor);
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        QWidget::resizeEvent(event);
        glScene->setFixedSize(this->width(), this->height());
        glScene->update();
        mdiArea->setFixedSize(this->width(), this->height());
        mdiArea->update();
        codeEditor->setGeometry(mdiArea->width() - 400 - 50, mdiArea->height()/2/2, 400, mdiArea->height()/2);
    }

    bool eventFilter(QObject *watched, QEvent *event) override {
        if (event->type() == QEvent::HoverEnter) {
            glScene->releaseKeyboard();
        } else if (event->type() == QEvent::HoverLeave) {
            glScene->grabKeyboard();
        }
        return QWidget::eventFilter(watched, event);
    }
};

#endif //FACTORI_GLMDI_H
