#include "hierarchybuttondelegate.h"

HierarchyButtonDelegate::HierarchyButtonDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void HierarchyButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
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

bool HierarchyButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
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
