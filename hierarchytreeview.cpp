#include "gameobject.h"
#include "hierarchytreeview.h"

#include <QApplication>
#include <QDrag>
#include <QMenu>
#include <QMimeData>
#include <QModelIndex>
#include <QPainter>
#include <QHeaderView>
#include <QFile>
#include <QTimer>

HierarchyTreeView::HierarchyTreeView(QList<GameObject*> &gameObjects, QWidget *parent) : QTreeView(parent), _gameObjects(gameObjects)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QAbstractItemView::EditKeyPressed);

    treeViewDelegate = new HierarchyTreeViewDelegate;
    btnDelegate = new HierarchyButtonDelegate(this);

    connect(btnDelegate, &HierarchyButtonDelegate::buttonClicked, this, &HierarchyTreeView::visibleClicked);

    QFile styleFile(":/resources/stylesheets/stylesheet.qss");
    styleFile.open(QFile::ReadOnly);

    // Apply the loaded stylesheet
    style = QString(styleFile.readAll());

    _model = new HierarchyTreeModel(gameObjects, this);

    this->setModel(_model);

    setupModel();

    connect(this, &QTreeView::customContextMenuRequested, this, &HierarchyTreeView::showContextMenu);
    connect(_model, &QStandardItemModel::itemChanged, this, &HierarchyTreeView::onItemChanged);
    connect(_model, &HierarchyTreeModel::gameObjectMoved, this, [=] {
        updateTreeView();
    });
}

void HierarchyTreeView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());

    int rowCount = model()->rowCount();

    for(int row = 0; row < rowCount; ++ row) {
        QModelIndex index = model()->index(row, 0);
        QRect rect = visualRect(index);
        rect.setX(24);
        rect.setWidth(columnWidth(0) + 24);

        QColor backgroundColor = (row % 2 == 0) ? QColor(56, 56, 56) : QColor(52, 52, 52);
        painter.fillRect(rect, backgroundColor);
    }

    QRect fullRect = viewport()->rect();
    QRect secondColumnRect(0, 0, 24, fullRect.height());


    // // Set the colors for each column
    QColor secondColumnColor(45, 45, 45);  // Green for the second column

    // // Paint the background of each column
    painter.fillRect(secondColumnRect, secondColumnColor);

    QTreeView::paintEvent(event);  // Call the base class paintEvent
}

void HierarchyTreeView::updateTreeView()
{
    saveExpandedState();
    _model->clear();
    setupModel();

    // Create a map to store QStandardItem pointers for each GameObject
    QHash<GameObject*, QStandardItem*> itemMap;

    // First, add all root GameObjects to the itemMap and the model
    for (GameObject* gameObject : _gameObjects) {
        if (!gameObject->parent()) {
            QStandardItem* nameItem = new QStandardItem(gameObject->name());
            nameItem->setData(QVariant::fromValue(gameObject));

            QStandardItem* iconItem = new QStandardItem();
            iconItem->setData(QVariant::fromValue(gameObject));

            QList<QStandardItem*> rowItems;
            rowItems << nameItem << iconItem;

            itemMap[gameObject] = nameItem;
            _model->appendRow(rowItems);
        }
    }

    // Then, add all GameObjects that do have a parent
    for (GameObject* gameObject : _gameObjects) {
        if (gameObject->parent()) {
            QStandardItem* nameItem = new QStandardItem(gameObject->name());
            nameItem->setData(QVariant::fromValue(gameObject));

            QStandardItem* iconItem = new QStandardItem();
            iconItem->setData(QVariant::fromValue(gameObject));

            QList<QStandardItem*> rowItems;
            rowItems << nameItem << iconItem;

            itemMap[gameObject] = nameItem;

            QStandardItem* parentItem = itemMap.value(gameObject->parent(), nullptr);
            if (parentItem) {
                parentItem->appendRow(rowItems);
            }
        }
    }

    restoreExpandedState();
}

void HierarchyTreeView::removeSelectedRow(QString guid)
{
    _model->removeRow(guid);
}

void HierarchyTreeView::visibleClicked(QModelIndex index)
{
    if (index.isValid()) {
        QModelIndex adjustedIndex;
        if (index.parent().isValid()) {
            // It's a child item, use the original column index
            adjustedIndex = _model->index(index.row(), index.column(), index.parent());
        } else {
            // It's a top-level item, adjust the column index to 0
            adjustedIndex = _model->index(index.row(), 0);
        }
        GameObject* gameObject = _model->itemFromIndex(adjustedIndex)->data().value<GameObject*>();

        if (gameObject) {
            // Now you have the correct GameObject
            qDebug() << gameObject->name();
            gameObject->setVisible(!gameObject->visible());
            if(gameObject->visible()){
                gameObject->setVisibleIcon(QIcon(":/resources/icons/visible.png"));
            }
            else{
                gameObject->setVisibleIcon(QIcon(":/resources/icons/visible2.png"));
            }

            QTimer::singleShot(0, this, [=] {
                    updateTreeView();
                });
        }
    }
}

void HierarchyTreeView::onItemChanged(QStandardItem *item)
{
    if (item) {
        GameObject* gameObject = item->data().value<GameObject*>();
        if (gameObject) {
            gameObject->setName(item->text());
        }
    }
}

void HierarchyTreeView::contextMenuEvent(QContextMenuEvent* event)
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

void HierarchyTreeView::keyPressEvent(QKeyEvent *event)
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

void HierarchyTreeView::startDrag(Qt::DropActions supportedActions)
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

void HierarchyTreeView::setupModel()
{
    _model->clear();
    _model->setColumnCount(2);

    this->header()->resizeSection(1, 10);
    this->header()->setSectionsMovable(true);
    this->header()->swapSections(0, 1);
    this->header()->setHidden(true);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setStyleSheet(style);
    this->header()->setStretchLastSection(true);
    this->setItemDelegate(treeViewDelegate);
    this->setItemDelegateForColumn(1, btnDelegate);
    this->setMouseTracking(true);
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setDragDropMode(QAbstractItemView::InternalMove);
    this->viewport()->installEventFilter(this->parent());
}

void HierarchyTreeView::saveExpandedState(const QModelIndex &parent)
{
    for(int i = 0; i < _model->rowCount(parent); ++i) {
        QModelIndex idx = _model->index(i, 0, parent);
        GameObject* gameObject = _model->itemFromIndex(idx)->data().value<GameObject*>();
        // Check if the GameObject is in the gameObjects list and is not null
        if (_gameObjects.contains(gameObject) && gameObject && this->isExpanded(idx)) {
            expandedItems.insert(gameObject->guid());
        }
        if (_model->hasChildren(idx)) {
            saveExpandedState(idx);
        }
    }
}

void HierarchyTreeView::restoreExpandedState(const QModelIndex &parent)
{
    for(int i = 0; i < _model->rowCount(parent); ++i) {
        QModelIndex idx = _model->index(i, 0, parent);
        GameObject* gameObject = _model->itemFromIndex(idx)->data().value<GameObject*>();
        if (gameObject && expandedItems.contains(gameObject->guid())) {
            this->setExpanded(idx, true);
        }
        if (_model->hasChildren(idx)) {
            restoreExpandedState(idx);
        }
    }
}

void HierarchyTreeView::addEmptyGameObject()
{
    QString baseName = "GameObject";
    QString name = baseName;
    int i = 1;
    GameObject* parent = nullptr;
    QModelIndex index = this->currentIndex();
    if (index.isValid()) {
        parent = _model->itemFromIndex(index)->data().value<GameObject*>();
        this->setExpanded(index, true);
        qDebug() << "Parent: " <<_model->itemFromIndex(index)->data();
        // Check if the parent already contains a GameObject with the same name
        while (parent->findChild(name) != nullptr) {
            // If it does, append a number to the name and increment it
            name = baseName + " (" + QString::number(i++) + ")";
        }
    }
    GameObject* gameObject = new GameObject(name, 3, 99, parent);
    _gameObjects.append(gameObject);
    qDebug() << gameObject->guid();

    updateTreeView();

    // Enter edit mode for the name of the new GameObject
    QModelIndex newIndex = _model->indexFromItem(gameObject, QModelIndex());
    if (newIndex.isValid()) {
        this->edit(newIndex);
    }
}

GameObject* HierarchyTreeView::getCurrentGameObject()
{
    QModelIndex index = this->currentIndex();
     if (index.isValid()) {
        return _model->itemFromIndex(index)->data().value<GameObject*>();
    }
}

void HierarchyTreeView::RemoveGameObject(const QString guid)
{
    for (int i = 0; i < _gameObjects.size(); ++i) {
        if (_gameObjects[i]->guid() == guid) {
            // Check if the GameObject is a parent
            if (!_gameObjects[i]->children().isEmpty()) {
                // If it is a parent, delete all its children
                for (GameObject* child : _gameObjects[i]->children()) {
                    _gameObjects.removeOne(child);  // Remove the child from the gameObjects list
                    //delete child;
                }
                _gameObjects[i]->children().clear();  // Clear the list of children
            }

            delete _gameObjects[i];  // Delete the GameObject
            _gameObjects.removeAt(i);  // Remove it from the list
            break;  // Exit the loop
        }
    }

    updateTreeView();
}

void HierarchyTreeView::showContextMenu(const QPoint &pos)
{
    QModelIndex index = this->indexAt(pos);
    QMenu contextMenu;
    if (index.isValid()) {
        GameObject* gameObject = _model->itemFromIndex(index)->data().value<GameObject*>();
        if (gameObject) {
            QAction *addEmptyAction = contextMenu.addAction("Create Empty");
            connect(addEmptyAction, &QAction::triggered, this, &HierarchyTreeView::addEmptyGameObject);
            QAction *deleteAction = contextMenu.addAction("Delete");
            //connect(deleteAction, &QAction::triggered, this, &MainWindow::RemoveGameObject);
            connect(deleteAction, &QAction::triggered, std::bind(&HierarchyTreeView::RemoveGameObject, this, gameObject->guid()));
        }
    } else {
        // This is an empty part of the tree view
        QAction *addEmptyAction = contextMenu.addAction("Create Empty");
        connect(addEmptyAction, &QAction::triggered, this, &HierarchyTreeView::addEmptyGameObject);
        QAction *action4 = contextMenu.addAction("Action 4");
        // Connect the actions to slots...
    }
    contextMenu.exec(this->mapToGlobal(pos));
}
