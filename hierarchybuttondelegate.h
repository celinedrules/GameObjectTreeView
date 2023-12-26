#ifndef HIERARCHYBUTTONDELEGATE_H
#define HIERARCHYBUTTONDELEGATE_H

#include "gameobject.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QStyledItemDelegate>

/**
 * @class HierarchyButtonDelegate
 * @brief Custom button delegate for hierarchy tree view visibility
 */
class HierarchyButtonDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    /**
     * @brief HierarchyButtonDelegate creates a new instance of the visibility button delegate
     *        for the hierarchy tree view
     * @param parent The parent hierarechy tree view this delegate belongs too
     */
    HierarchyButtonDelegate(QObject *parent = nullptr);

    /**
     * @brief paint Renders the contents of the specified model index using the given painter and style option
     *
     * This function is called by Qt's view classes whenever an item needs to be drawn.
     *
     * @param painter The QPainter object that should be used to paint the item
     * @param option The QStyleOptionViewItem object that contains the options for the item to be painted
     * @param index The QModelIndex object that specifies the model index of the item to be painted
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    /**
     * @brief editorEvent Handles editor events for the item view in a class that inherits from QStyledItemDelegate
     *
     * This function is an override of the QStyledItemDelegate::editorEvent() function
     * It is used to handle custom editing behavior for the item view
     *
     * @param event The event that triggered the editor
     * @param model Pointer to the model that contains the data
     * @param option The style options for the item
     * @param index The model index of the item
     * @return Returns true if the event is handled; otherwise returns false
     */
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    /**
     * @brief buttonClicked Signal that is emitted when a button is clicked
     *
     * This signal is part of a class that inherits from QStyledItemDelegate
     * It is used to notify other parts of the program when a button has been clicked
     *
     * @param index The model index of the item associated with the button that was clicked
     */
    void buttonClicked(const QModelIndex &index);
};


#endif // HIERARCHYBUTTONDELEGATE_H
