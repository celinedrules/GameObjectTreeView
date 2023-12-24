#ifndef HIERARCHYBUTTONDELEGATE_H
#define HIERARCHYBUTTONDELEGATE_H

#include "gameobject.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QStyledItemDelegate>

class HierarchyButtonDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    HierarchyButtonDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        GameObject* gameObject = index.data(Qt::UserRole + 1).value<GameObject*>();
        if(gameObject){
            if (index.column() == 1) {
                QIcon icon = gameObject->icon();

                QSize iconSize(24, 24); // Adjust the width and height as needed

                QRect buttonRect = option.rect;
                buttonRect.setSize(iconSize);

                // Draw the transparent button
                painter->setPen(Qt::NoPen); // Set the pen to NoPen to hide the border
                painter->setBrush(QColor(45, 45, 45));
                painter->drawRect(buttonRect);

                // Draw the icon on the transparent button
                if(option.state & QStyle::State_MouseOver || !gameObject->visible()) {
                    icon.paint(painter, buttonRect, Qt::AlignCenter, QIcon::Normal, QIcon::On);
                }
            }
        }
        else {
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


#endif // HIERARCHYBUTTONDELEGATE_H
