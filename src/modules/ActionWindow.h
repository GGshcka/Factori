//
// Created by silly on 03.03.2025.
//

#ifndef FACTORI_ACTIONWINDOW_H
#define FACTORI_ACTIONWINDOW_H

#include <Qt>
#include <QtWidgets>
#include <QtGlobal>

//TODO Своя рамка окна!
class ActionWindow : public QMdiSubWindow {
Q_OBJECT
public:
    ActionWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    QVBoxLayout *mainLayout;

public: Q_SIGNALS:
    void closing();

protected:
    void closeEvent(QCloseEvent *closeEvent) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool dragging;
    QPoint dragPosition;
    QMenuBar *bar;
};


#endif //FACTORI_ACTIONWINDOW_H
