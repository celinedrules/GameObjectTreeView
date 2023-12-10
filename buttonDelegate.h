#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include "GameObject.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QStyledItemDelegate>

class ButtonDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    ButtonDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (index.column() == 1 && option.state & QStyle::State_MouseOver) { // Removed the column index check
            // Draw the button
            GameObject* gameObject = index.data(Qt::UserRole + 1).value<GameObject*>();
            if (gameObject) {
                QIcon icon = gameObject->icon();

                QSize iconSize(24, 24); // Adjust the width and height as needed

                QRect buttonRect = option.rect;
                buttonRect.setSize(iconSize);

                // Draw the transparent button
                painter->setPen(Qt::NoPen); // Set the pen to NoPen to hide the border
                painter->setBrush(Qt::transparent);
                painter->drawRect(buttonRect);

                // Draw the icon on the transparent button
                icon.paint(painter, buttonRect, Qt::AlignCenter, QIcon::Normal, QIcon::On);
            }
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }


    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override {
        if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                // Perform your action here
                //qDebug() << "Button in row " << index.row() << " clicked.";
                emit buttonClicked(index);
            }
        }
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

signals:
    void buttonClicked(const QModelIndex &index) const;
};


#endif // BUTTONDELEGATE_H
