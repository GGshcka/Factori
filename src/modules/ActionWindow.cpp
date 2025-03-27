//
// Created by silly on 03.03.2025.
//

#include "ActionWindow.h"

ActionWindow::ActionWindow(QWidget *parent, Qt::WindowFlags flags) : QMdiSubWindow(parent, flags), dragging(false) {
    setWindowFlag(Qt::FramelessWindowHint, true);

    auto *mainWidget = new QWidget;

    bar = new QMenuBar(mainWidget);
    bar->setObjectName("ActionWindowMainBar");
    bar->setLayoutDirection(Qt::RightToLeft);
    bar->setFixedHeight(24);

    auto *actionClose = new QAction();
    actionClose->setIcon(QIcon(QPixmap(":/ui/sprCloseIcon")));
    connect(actionClose, &QAction::triggered, this, [this]() {
       this->close();
    });
    bar->addAction(actionClose);

    auto *layout = new QVBoxLayout(mainWidget);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(bar);

    mainLayout = new QVBoxLayout();
    layout->addLayout(mainLayout);

    mainWidget->setLayout(layout);
    setWidget(mainWidget);
}

void ActionWindow::mousePressEvent(QMouseEvent* event) {
    QPoint globalPos = event->globalPosition().toPoint();
    QPoint barLocalPos = bar->mapFromGlobal(globalPos);

    if (event->button() == Qt::LeftButton) {
        qDebug() << "leftMousePressed";
        dragging = true;
    }

    QMdiSubWindow::mousePressEvent(event);
}


void ActionWindow::mouseMoveEvent(QMouseEvent *event) {
    if (dragging) {
        move(event->globalPosition().toPoint() - dragPosition);  // Перемещаем окно
        event->accept();
        qDebug() << "moving";
    }
    QMdiSubWindow::mouseMoveEvent(event);
}

void ActionWindow::mouseReleaseEvent(QMouseEvent *event) {
    dragging = false;
    qDebug() << "release";
    QMdiSubWindow::mouseReleaseEvent(event);
}

void ActionWindow::closeEvent(QCloseEvent *closeEvent) {
    Q_EMIT closing();
    QMdiSubWindow::closeEvent(closeEvent);
}
