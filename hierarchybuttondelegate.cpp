#include "hierarchybuttondelegate.h"

HierarchyButtonDelegate::HierarchyButtonDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void HierarchyButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // Get the GameObject from the model index
    GameObject* gameObject = index.data(Qt::UserRole + 1).value<GameObject*>();
    if(gameObject){
        // Check if the column of the index is 1
        if (index.column() == 1) {
            // Get the icon representing the visibility  of the GameObject
            QIcon icon = gameObject->getVisibleIcon();
            // Set the size of the icon
            QSize iconSize(24, 24);

            // Define the rectangle for the button
            QRect buttonRect = option.rect;
            // Set the size of the button to the size of the icon
            buttonRect.setSize(iconSize);

            // Draw the transparent button
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(45, 45, 45));
            painter->drawRect(buttonRect);

            // If the mouse is over the item or the GameObject is not visible, draw the icon
            if(option.state & QStyle::State_MouseOver || !gameObject->visible()) {
                icon.paint(painter, buttonRect, Qt::AlignCenter, QIcon::Normal, QIcon::On);
            }
        }
    }
    else {
        // If the GameObject does not exist, call the parent class;s paint function
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool HierarchyButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    // Check if the event is a mouse button release event
    if (event->type() == QEvent::MouseButtonRelease) {
        // Cast the event to a QMouseEvent
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        // Check if the left mouse button was released
        if (mouseEvent->button() == Qt::LeftButton) {
            // If tghe left mouse button was released, emit hte button clicked signal with the index of the button
            emit buttonClicked(index);
        }
    }

    // If the event was not handled, call the parent class's editorEvent function
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
