#include "GameObject.h"
#include "hierarchytreeeview.h"

#include <QApplication>
#include <QDrag>
#include <QMenu>
#include <QMimeData>
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

void HierarchyTreeeView::startDrag(Qt::DropActions supportedActions)
{
    QStandardItemModel* standardModel = qobject_cast<QStandardItemModel*>(model());
    if (!standardModel)
        return;

    QModelIndexList indexes = selectedIndexes();
    if (indexes.count() <= 0)
        return;

    QMimeData* data = standardModel->mimeData(indexes);
    if (!data)
        return;

    QDrag* drag = new QDrag(this);
    drag->setMimeData(data);

    // Set the icon
    QPixmap pixmap(":/resources/icons/drag.png");
    QSize size(12, 12);  // Replace with your desired size
    pixmap = pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    drag->setPixmap(pixmap);

    // Start the drag operation
    drag->exec(supportedActions, Qt::CopyAction);
}
