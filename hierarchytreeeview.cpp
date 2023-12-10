#include "GameObject.h"
#include "hierarchytreeeview.h"

#include <QMenu>
#include <QModelIndex>

HierarchyTreeeView::HierarchyTreeeView(QWidget *parent) : QTreeView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QAbstractItemView::EditKeyPressed);
}

void HierarchyTreeeView::contextMenuEvent(QContextMenuEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        return;
    }

    GameObject* gameObject = index.data().value<GameObject*>();
    if (!gameObject) {
        return;
    }

    QMenu menu(this);
    QAction* action = menu.addAction("Action");
    connect(action, &QAction::triggered, this, [=] {
            // Handle the action
            qDebug() << "Action triggered on GameObject: " << gameObject->name();
        });
    menu.exec(event->globalPos());
}

void HierarchyTreeeView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F2 && currentIndex().isValid())
    {
        edit(currentIndex());
    }
    else
    {
        QTreeView::keyPressEvent(event);
    }
}

