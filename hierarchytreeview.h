#ifndef HIERARCHYTREEVIEW_H
#define HIERARCHYTREEVIEW_H

#include "gameobject.h"
#include "hierarchytreemodel.h"
#include "hierarchytreeviewdelegate.h"
#include "hierarchybuttondelegate.h"
#include <QContextMenuEvent>
#include <QTreeView>


/**
 * @class HierarchyTreeView
 * @brief A custom QTreeView for displaying GameObjects in a hierarchy
 *
 * This class inherits from QTreeViewand provides a custom view for diplsaying GameObject's in a hiereachy style view
 */
class HierarchyTreeView : public QTreeView
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a HierarchyTreeView with a list of GameObjects
     *
     * @param gameObjects A reference to a list of GameObjects
     * @param parent The parent QWidget
     */
    explicit HierarchyTreeView(QList<GameObject*> &gameObjects, QWidget* parent = nullptr);

    /**
     * @brief Handles the painting of the view
     *
     * @param event The paint event
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Updates the tree view
     */
    void updateTreeView();

    /**
     * @brief Adds an empty GameObject to the tree view
     */
    void addEmptyGameObject();

    /**
     * @brief Returns the currently selected GameObject
     *
     * @return The currently selected GameObject
     */
    GameObject* getCurrentGameObject();

    HierarchyTreeModel *_model; // The model for the tree view
    HierarchyButtonDelegate *btnDelegate; // The delegate for handling button clicks
    HierarchyTreeViewDelegate *treeViewDelegate; // The delegate for handling the display of items

public slots:
    /**
     * @brief Removes the selected row from the tree view
     *
     * @param guid The GUID of the row to remove
     */
    void removeSelectedRow(QString guid);

    /**
     * @brief Handles the clicking of the visibility button
     *
     * @param index The model index of the clicked item
     */
    void visibleClicked(QModelIndex index);

    /**
     * @brief Handles changes to items in the tree view
     *
     * @param item The item that changed
     */
    void onItemChanged(QStandardItem *item);

protected:
    /**
     * @brief Handles context menu events.
     *
     * @param event The context menu event
     */
    void contextMenuEvent(QContextMenuEvent *event) override;

    /**
     * @brief Handles key press events
     *
     * @param event The key event
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief Starts a drag operation
     *
     * @param supportedActions The supported drag actions
     */
    void startDrag(Qt::DropActions supportedActions) override;

private:
    /**
     * @brief Initializes the the tree view
     */
    void initialize();

    /**
     * @brief Saves the expanded state of the tree view
     *
     * @param parent The parent model index
     */
    void saveExpandedState(const QModelIndex &parent = QModelIndex());

    /**
     * @brief Restores the expanded state of the tree view
     *
     * @param parent The parent model index
     */
    void restoreExpandedState(const QModelIndex &parent = QModelIndex());

    /**
     * @brief Removes a GameObject from the tree view
     *
     * @param guid The GUID of the GameObject to remove
     */
    void RemoveGameObject(const QString guid);

    /**
     * @brief Shows a context menu at the specified position
     *
     * @param pos The position to show the context menu
     */
    void showContextMenu(const QPoint &pos);

    QSet<QString> expandedItems; // A list of items in the tree view that are expanded
    QPoint dragStartPosition; // The start position of a drag operation
    QList<GameObject*> _gameObjects; // The list of GameObjects
    QString style; // The style of the tree view
};

#endif // HIERARCHYTREEVIEW_H
