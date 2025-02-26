//
// Created by silly on 08.01.2025.
//

#ifndef FACTORI_GRAPHICALGAMEOBJECT_H
#define FACTORI_GRAPHICALGAMEOBJECT_H

#include <Qt>
#include <QtWidgets>

class GraphicalGameObject : public QObject, public QGraphicsPixmapItem {
Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal rotation READ getRotation WRITE setRotation)
    Q_PROPERTY(qreal scale READ getScale WRITE setScale)
    Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity NOTIFY opacityChanged)

public:
    explicit GraphicalGameObject(const QPixmap& pixmap, QGraphicsItem *parent = nullptr) : QGraphicsPixmapItem(pixmap, parent), m_startPos(0, 0), m_rotation(0.0), m_scale(1.0) {}

    QPointF startPos() const { return m_startPos; }
    void setStartPos(QPointF newStartPos) {
        m_startPos = newStartPos;
        setPos(m_startPos);
    }
    void setStartPos(float x, float y) {
        m_startPos = QPointF(x, y);
        setPos(m_startPos);
    }

    void setPos(const QPointF &newPos) {
        QGraphicsPixmapItem::setPos(newPos);
        Q_EMIT posChanged();
    }
    void setPos(const float x, const float y) {
        QGraphicsPixmapItem::setPos(x, y);
        Q_EMIT posChanged();
    }
    void setX(const float x) {
        QGraphicsPixmapItem::setX(x);
        Q_EMIT posChanged();
    }
    void setY(const float y) {
        QGraphicsPixmapItem::setY(y);
        Q_EMIT posChanged();
    }

    void setPixmap(const QPixmap &pixmap) {
        QGraphicsPixmapItem::setPixmap(pixmap);
        setTransformOriginPoint(boundingRect().center());
    }

    qreal getRotation() const { return m_rotation; }
    void setRotation(qreal angle) {
        m_rotation = angle;
        QTransform transform;
        transform.translate(boundingRect().center().x(), boundingRect().center().y());
        transform.rotate(m_rotation);
        transform.translate(-boundingRect().center().x(), -boundingRect().center().y());
        setTransform(transform);
    }

    qreal getScale() const { return m_scale; }
    void setScale(qreal scalePoint) {
        m_scale = scalePoint;
        QTransform transform;
        transform.translate(boundingRect().center().x(), boundingRect().center().y());
        transform.scale(m_scale, m_scale);
        transform.translate(-boundingRect().center().x(), -boundingRect().center().y());
        setTransform(transform);
    }

    qreal getOpacity() const { return QGraphicsPixmapItem::opacity(); }
    void setOpacity(qreal value) {
        if (qFuzzyCompare(getOpacity(), value)) return;
        QGraphicsPixmapItem::setOpacity(value);
        Q_EMIT opacityChanged();
    }

    void showMood(double multiplier, const QString& moodString) {
        auto *moodLabel = new QLabel();
        moodLabel->setAttribute(Qt::WA_TranslucentBackground); // Устанавливаем прозрачный фон
        moodLabel->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint); // Убираем рамки окна

        auto *movie = new QMovie(":/moods/spr" + moodString);

        if (moodString == "Annoyed") movie->setSpeed(100 / multiplier);
        else if (moodString == "Sad" || moodString == "Love") movie->setSpeed(31 / multiplier);

        moodLabel->setMovie(movie);
        movie->start();

        QGraphicsProxyWidget *proxy = scene()->addWidget(moodLabel);

        proxy->setPos(pos().x(), pos().y() - pixmap().height());

        connect(this, &GraphicalGameObject::posChanged, [this, proxy]() {
            proxy->setPos(pos().x(), pos().y() - pixmap().height());
        });

        auto *animation = new QPropertyAnimation(proxy, "opacity", this);
        animation->setDuration(2000 / multiplier); // Время анимации
        animation->setKeyValueAt(0.0, 0.0);
        animation->setKeyValueAt(0.2, 1.0);
        animation->setKeyValueAt(0.7, 1.0);
        animation->setKeyValueAt(1.0, 0.0);
        animation->setEasingCurve(QEasingCurve::Linear);

        connect(animation, &QPropertyAnimation::finished, this, [this, movie, moodLabel, proxy]() {
            scene()->removeItem(proxy);
            moodLabel->close();
            movie->stop();
        });

        animation->start();
    }



Q_SIGNALS:
        void opacityChanged();
        void posChanged();

private:
    QPointF m_startPos;
    qreal m_rotation;
    qreal m_scale;
};

#endif // FACTORI_GRAPHICALGAMEOBJECT_H
