#ifndef HIERARCHYTREEVIEW_H
#define HIERARCHYTREEVIEW_H

#include "gameobject.h"
#include "hierarchytreemodel.h"
#include "hierarchytreeviewdelegate.h"
#include "hierarchybuttondelegate.h"
#include <QContextMenuEvent>
#include <QTreeView>



class HierarchyTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit HierarchyTreeView(QList<GameObject*> &gameObjects, QWidget* parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void updateTreeView();
    void addEmptyGameObject();
    GameObject* getCurrentGameObject();
    HierarchyTreeModel *_model;
    HierarchyButtonDelegate *btnDelegate;
    HierarchyTreeViewDelegate *treeViewDelegate;

public slots:
    void removeSelectedRow(QString guid);
    void visibleClicked(QModelIndex index);
    void onItemChanged(QStandardItem *item);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void startDrag(Qt::DropActions supportedActions) override;

private:
    void setupModel();
    void saveExpandedState(const QModelIndex &parent = QModelIndex());
    void restoreExpandedState(const QModelIndex &parent = QModelIndex());
    void RemoveGameObject(const QString guid);
    void showContextMenu(const QPoint &pos);

    QSet<QString> expandedItems;
    QPoint dragStartPosition;
    QList<GameObject*> _gameObjects;
    QString style;
};

#endif // HIERARCHYTREEVIEW_H
