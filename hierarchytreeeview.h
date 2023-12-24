#ifndef HIERARCHYTREEEVIEW_H
#define HIERARCHYTREEEVIEW_H

#include "GameObject.h"
#include "TreeModel.h"
#include "TreeViewDelegate.h"
#include "buttonDelegate.h"
#include <QContextMenuEvent>
#include <QTreeView>



class HierarchyTreeeView : public QTreeView
{
    Q_OBJECT
public:
    explicit HierarchyTreeeView(QList<GameObject*> &gameObjects, QWidget* parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void updateTreeView();
    void addEmptyGameObject();
    GameObject* getCurrentGameObject();
    TreeModel *_model;
    ButtonDelegate *btnDelegate;
    TreeViewDelegate *treeViewDelegate;

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

#endif // HIERARCHYTREEEVIEW_H
