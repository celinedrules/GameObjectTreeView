#ifndef HIERARCHYTREEMODEL_H
#define HIERARCHYTREEMODEL_H

#include "gameobject.h"

#include <QIODevice>
#include <QMimeData>
#include <QStandardItemModel>


/**
 * @class HierarchyTreeModel
 * @brief A custom model for displaying GHameObjects in a tree hierarchy
 *
 * This class inherits from QStandardItemModel and provides custom behavior for handling GameObjects
 * It emits a signal when a GameObject is moved within the hierarchy
 */
class HierarchyTreeModel : public QStandardItemModel {
    Q_OBJECT
public:
    /**
     * @brief Constructs a HierarchyTreeModel with a list of GameObjects
     *
     * @param gameObjects A reference to a list of GameObjects
     * @param parent The parent QObject
     */
    HierarchyTreeModel(QList<GameObject*>& gameObjects, QObject *parent = nullptr);

    /**
     * @brief Returns the item flags for the given model index
     *
     * @param index The model index
     * @return The item flags for the model index
     */
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /**
     * @brief Appends a row to the model with a single item
     *
     * @param item The item to append
     */
    void appendRow(QStandardItem* item);

    /**
     * @brief Appends a row to the model with a list of items
     *
     * @param items The list of items to append
     */
    void appendRow(QList<QStandardItem*> items);

    /**
     * @brief Removes a row from the model
     *
     * @param guid The GUID of the row to remove
     */
    void removeRow(QString guid);

    /**
     * @brief Returns the model index for a given GameObject
     *
     * @param gameObject The GameObject for the row to be removed
     * @param parent The parent model index
     * @return The model index for the GameObject
     */
    QModelIndex indexFromItem(const GameObject* gameObject, const QModelIndex& parent) const;

    /**
     * @brief Returns the MIME types supported by this model
     *
     * @return A list of MIME types
     */
    QStringList mimeTypes() const override;

    /**
     * @brief Returns the MIME data for a list of model indexes
     *
     * @param indexes The list of model indexes
     * @return The MIME data for the model indexes
     */
    QMimeData* mimeData(const QModelIndexList &indexes) const override;

    /**
     * @brief Handles the dropping of MIME data onto the model
     *
     * @param data The MIME data
     * @param action The drop action
     * @param row The row where the data was dropped
     * @param column The column where the data was dropped
     * @param parent The parent model index
     * @return True if the drop was handled otherwise false
     */
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

signals:
    /**
     * @brief Signal that is emitted when a GameObject is moved within the hierarchy
     */
    void gameObjectMoved();

private:
    /**
     * @brief A reference to a list of GameObjects
     */
    QList<GameObject*>& gameObjects;

    /**
     * @brief Finds an item in the model that corresponds to a GameObject.
     *
     * @param gameObject The GameObject
     * @param parent The parent item
     * @return The item that corresponds to the GameObject
     */
    QStandardItem* findItem(GameObject* gameObject, QStandardItem* parent = nullptr);
};
#endif // HIERARCHYTREEMODEL_H
