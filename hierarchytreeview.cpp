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
    // Set the context menu policy to custom, allowing for a custom context menu to be used
    setContextMenuPolicy(Qt::CustomContextMenu);
    // Set the edit triggers to only allow editing when the Edit key is pressed
    setEditTriggers(QAbstractItemView::EditKeyPressed);

    // Initialize the delegates for the tree view and button
    treeViewDelegate = new HierarchyTreeViewDelegate;
    btnDelegate = new HierarchyButtonDelegate(this);

    // Connect the buttonClicked signal from the button delegate to the visibleClicked slot in this class
    connect(btnDelegate, &HierarchyButtonDelegate::buttonClicked, this, &HierarchyTreeView::visibleClicked);

    // Open the stylesheet file and read its contents
    QFile styleFile(":/resources/stylesheets/stylesheet.qss");
    styleFile.open(QFile::ReadOnly);

    // Apply the loaded stylesheet
    style = QString(styleFile.readAll());

    // Initialize the model for the tree view with the provided list of GameObjects
    _model = new HierarchyTreeModel(gameObjects, this);

    // Set the model for this tree view
    this->setModel(_model);

    // Initializie the tree view
    initialize();

    // Connect the customContextMenuRequested signal from this tree view to the showContextMenu slot in this class
    connect(this, &QTreeView::customContextMenuRequested, this, &HierarchyTreeView::showContextMenu);
    // Connect the itemChanged signal from the model to the onItemChanged slot in this class
    connect(_model, &QStandardItemModel::itemChanged, this, &HierarchyTreeView::onItemChanged);
    // Connect the gameObjectMoved signal from the model to a lambda function that calls updateTreeView
    connect(_model, &HierarchyTreeModel::gameObjectMoved, this, [=] {
        updateTreeView();
    });
}

void HierarchyTreeView::paintEvent(QPaintEvent *event)
{
    // Initialize the QPainter object
    QPainter painter(viewport());

    // Get the number of rows in the model
    int rowCount = model()->rowCount();

    // Loop through each row in the model
    for(int row = 0; row < rowCount; ++ row) {
        // Get the model index for the current row
        QModelIndex index = model()->index(row, 0);
        // Get the visual rectangle for the current index
        QRect rect = visualRect(index);
        // Adjust the x position and width of the rectangle
        rect.setX(24);
        rect.setWidth(columnWidth(0) + 24);

        // Determine the background color based on the row number
        QColor backgroundColor = (row % 2 == 0) ? QColor(56, 56, 56) : QColor(52, 52, 52);
        // Fill the rectangle with the background color
        painter.fillRect(rect, backgroundColor);
    }

    // Get the full rectangle of the viewport
    QRect fullRect = viewport()->rect();
    // Create a rectangle for the second column
    QRect secondColumnRect(0, 0, 24, fullRect.height());

    // Set the colors for each column
    QColor secondColumnColor(45, 45, 45);

    // Paint the background of each column
    painter.fillRect(secondColumnRect, secondColumnColor);

    // Call the base class paintEvent
    QTreeView::paintEvent(event);
}

void HierarchyTreeView::updateTreeView()
{
    // Save the expanded state of the tree view
    saveExpandedState();
    // Clear the model
    _model->clear();
    // Initialize the tree view
    initialize();

    // Create a map to store QStandardItem pointers for each GameObject
    QHash<GameObject*, QStandardItem*> itemMap;

    // First, add all root GameObjects to the itemMap and the model
    for (GameObject* gameObject : _gameObjects) {
        // If the GameObject does not have a parent, it is a root GameObject
        if (!gameObject->parent()) {
            // Create a new QStandardItem for the GameObject's name and associate it with the GameObject
            QStandardItem* nameItem = new QStandardItem(gameObject->name());
            nameItem->setData(QVariant::fromValue(gameObject));

            // Create a new QStandardItem for the GameObject's name and associate it with the GameObject
            QStandardItem* iconItem = new QStandardItem();
            iconItem->setData(QVariant::fromValue(gameObject));

            // Add the name and icon items to a list
            QList<QStandardItem*> rowItems;
            rowItems << nameItem << iconItem;

            // Add the GameObject to the itemMap
            itemMap[gameObject] = nameItem;

            // Add the row items to the model
            _model->appendRow(rowItems);
        }
    }

    // Then, add all GameObjects that do have a parent
    for (GameObject* gameObject : _gameObjects) {
        // If the GameObject has a parent, it is not a root GameObject
        if (gameObject->parent()) {
            // Create a new QStandardItem for the GameObject's name and associate it with the GameObject
            QStandardItem* nameItem = new QStandardItem(gameObject->name());
            nameItem->setData(QVariant::fromValue(gameObject));

            // Create a new QStandardItem for the GameObject's icon and associate it with the GameObject
            QStandardItem* iconItem = new QStandardItem();
            iconItem->setData(QVariant::fromValue(gameObject));

            // Add the name and icon items to a list
            QList<QStandardItem*> rowItems;
            rowItems << nameItem << iconItem;

            // Add the GameObject to the itemMap
            itemMap[gameObject] = nameItem;

            // Get the QStandardItem for the GameObject's parent
            QStandardItem* parentItem = itemMap.value(gameObject->parent(), nullptr);

            // If the parent item exists, append the row items to it
            if (parentItem) {
                parentItem->appendRow(rowItems);
            }
        }
    }

    // Restore the expanded state of the tree view
    restoreExpandedState();
}

void HierarchyTreeView::removeSelectedRow(QString guid)
{
    // Call the removeRow function on the model with the provided GUID
    _model->removeRow(guid);
}

void HierarchyTreeView::visibleClicked(QModelIndex index)
{
    // Check if the index is valid
    if (index.isValid()) {
        QModelIndex adjustedIndex;

        // Check if the index has a parent (i.e., it's a child item)
        if (index.parent().isValid()) {
            // It's a child item, use the original column index
            adjustedIndex = _model->index(index.row(), index.column(), index.parent());
        } else {
            // It's a top-level item, adjust the column index to 0
            adjustedIndex = _model->index(index.row(), 0);
        }

        // Get the GameObject associated with the adjusted index
        GameObject* gameObject = _model->itemFromIndex(adjustedIndex)->data().value<GameObject*>();

        // Check if the GameObject is valid
        if (gameObject) {
            // Toggle the visibility of the GameObject and update its icon
            gameObject->setVisible(!gameObject->visible());

            if(gameObject->visible()){
                gameObject->setVisibleIcon(QIcon(":/resources/icons/visible.png"));
            }
            else{
                gameObject->setVisibleIcon(QIcon(":/resources/icons/visible2.png"));
            }

            // Update the tree view after a delay
            QTimer::singleShot(0, this, [=] {
                    updateTreeView();
                });
        }
    }
}

void HierarchyTreeView::onItemChanged(QStandardItem *item)
{
    // Check if the item is valid
    if (item) {
        // Get the GameObject associated with the item
        GameObject* gameObject = item->data().value<GameObject*>();

        // Check if the GameObject is valid
        if (gameObject) {
            // Update the name of the GameObject
            gameObject->setName(item->text());
        }
    }
}

void HierarchyTreeView::contextMenuEvent(QContextMenuEvent* event)
{
    // Get the index at the position of the context menu event
    QModelIndex index = indexAt(event->pos());

    // Check if the index is valid
    if (!index.isValid()) {
        return;
    }

    // Get the GameObject associated with the index
    GameObject* gameObject = index.data().value<GameObject*>();

    // Check if the GameObject is valid
    if (!gameObject) {
        return;
    }

    // Create a context menu and add an action to it
    QMenu menu(this);
    QAction* action = menu.addAction("Action");

    // Connect the triggered signal of the action to a slot that handles the action
    connect(action, &QAction::triggered, this, [=] {
            // Handle the action
            qDebug() << "Action triggered on GameObject: " << gameObject->name();
        });

    // Show the context menu at the position of the event
    menu.exec(event->globalPos());
}

void HierarchyTreeView::keyPressEvent(QKeyEvent *event)
{
    // Check if the F2 key was pressed and if the current index is valid
    if (event->key() == Qt::Key_F2 && currentIndex().isValid())
    {
        // If so, start editing the current index
        edit(currentIndex());
    }
    else
    {
        // Otherwise, call the base class keyPressEvent
        QTreeView::keyPressEvent(event);
    }
}

void HierarchyTreeView::startDrag(Qt::DropActions supportedActions)
{
    // Cast the model to a QStandardItemModel
    QStandardItemModel* standardModel = qobject_cast<QStandardItemModel*>(model());

    // Check if the cast was successful
    if (!standardModel)
        return;

    // Get the list of selected indexes
    QModelIndexList indexes = selectedIndexes();

    // Check if there are any selected indexes
    if (indexes.count() <= 0)
        return;

    // Get the MIME data for the selected indexes
    QMimeData* data = standardModel->mimeData(indexes);

    // Check if the MIME data is valid
    if (!data)
        return;

    // Create a new QDrag object
    QDrag* drag = new QDrag(this);
    // Set the MIME data for the drag operation
    drag->setMimeData(data);

    // Set the icon for the drag operation
    QPixmap pixmap(":/resources/icons/drag.png");
    QSize size(12, 12);
    pixmap = pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    drag->setPixmap(pixmap);

    // Start the drag operation
    drag->exec(supportedActions, Qt::CopyAction);
}

void HierarchyTreeView::initialize()
{
    // Clear the model and set the column count to 2
    _model->clear();
    _model->setColumnCount(2);

    // Configure the header of the tree view
    this->header()->resizeSection(1, 10);
    this->header()->setSectionsMovable(true);
    this->header()->swapSections(0, 1);
    this->header()->setHidden(true);

    // Set the selection behavior to select rows
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Apply the stylesheet to the tree view
    this->setStyleSheet(style);
    // Stretch the last section of the header
    this->header()->setStretchLastSection(true);

    // Set the item delegate for the tree view and the button delegate for column 1
    this->setItemDelegate(treeViewDelegate);
    this->setItemDelegateForColumn(1, btnDelegate);

    // Enable mouse tracking, drag and drop, and internal move
    this->setMouseTracking(true);
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setDragDropMode(QAbstractItemView::InternalMove);

    // Install an event filter on the viewport
    this->viewport()->installEventFilter(this->parent());
}

void HierarchyTreeView::saveExpandedState(const QModelIndex &parent)
{
    // Loop through each row in the model under the provided parent
    for(int i = 0; i < _model->rowCount(parent); ++i) {
        // Get the index for the current row
        QModelIndex idx = _model->index(i, 0, parent);
        // Get the GameObject associated with the index
        GameObject* gameObject = _model->itemFromIndex(idx)->data().value<GameObject*>();

        // Check if the GameObject is in the gameObjects list, is not null, and is expanded
        if (_gameObjects.contains(gameObject) && gameObject && this->isExpanded(idx)) {
            // If so, add the GameObject's GUID to the expandedItems set
            expandedItems.insert(gameObject->guid());
        }

        // If the index has children, recursively call this function on it
        if (_model->hasChildren(idx)) {
            saveExpandedState(idx);
        }
    }
}

void HierarchyTreeView::restoreExpandedState(const QModelIndex &parent)
{
    // Loop through each row in the model under the provided parent
    for(int i = 0; i < _model->rowCount(parent); ++i) {
        // Get the index for the current row
        QModelIndex idx = _model->index(i, 0, parent);
        // Get the GameObject associated with the index
        GameObject* gameObject = _model->itemFromIndex(idx)->data().value<GameObject*>();

        // Check if the GameObject is in the expandedItems set
        if (gameObject && expandedItems.contains(gameObject->guid())) {
            // If so, expand the index in the tree view
            this->setExpanded(idx, true);
        }

        // If the index has children, recursively call this function on it
        if (_model->hasChildren(idx)) {
            restoreExpandedState(idx);
        }
    }
}

void HierarchyTreeView::addEmptyGameObject()
{
    // Set the base name for the new GameObject
    QString baseName = "GameObject";
    QString name = baseName;
    int i = 1;

    // Initialize the parent of the new GameObject to null
    GameObject* parent = nullptr;
    // Get the current index in the tree view
    QModelIndex index = this->currentIndex();

    // Check if the index is valid
    if (index.isValid()) {
        // If so, get the GameObject associated with the index and set it as the parent
        parent = _model->itemFromIndex(index)->data().value<GameObject*>();
        // Expand the index in the tree view
        this->setExpanded(index, true);

        // Check if the parent already contains a GameObject with the same name
        while (parent->findChild(name) != nullptr) {
            // If it does, append a number to the name and increment it
            name = baseName + " (" + QString::number(i++) + ")";
        }
    }

    // Create a new GameObject with the determined name and parent, and add it to the gameObjects list
    GameObject* gameObject = new GameObject(name, 3, 99, parent);
    _gameObjects.append(gameObject);

    // Update the tree view
    updateTreeView();

    // Enter edit mode for the name of the new GameObject
    QModelIndex newIndex = _model->indexFromItem(gameObject, QModelIndex());
    if (newIndex.isValid()) {
        this->edit(newIndex);
    }
}

GameObject* HierarchyTreeView::getCurrentGameObject()
{
    // Get the current index in the tree view
    QModelIndex index = this->currentIndex();

    // Check if the index is valid
     if (index.isValid()) {
        // If so, return the GameObject associated with the index
        return _model->itemFromIndex(index)->data().value<GameObject*>();
    }

     return nullptr;
}

void HierarchyTreeView::RemoveGameObject(const QString guid)
{
    // Loop through the gameObjects list
    for (int i = 0; i < _gameObjects.size(); ++i) {
        // Check if the current GameObject's GUID matches the provided GUID
        if (_gameObjects[i]->guid() == guid) {
            // If so, check if the GameObject is a parent
            if (!_gameObjects[i]->children().isEmpty()) {
                // If it is a parent, delete all its children
                for (GameObject* child : _gameObjects[i]->children()) {
                    // Remove the child from the gameObjects list
                    _gameObjects.removeOne(child);
                }

                  // Clear the list of children
                _gameObjects[i]->children().clear();
            }

            // Delete the GameObject and remove it from the list
            delete _gameObjects[i];
            _gameObjects.removeAt(i);

            // Exit the loop
            break;
        }
    }

    // Update the tree view
    updateTreeView();
}

void HierarchyTreeView::showContextMenu(const QPoint &pos)
{
    // Get the index at the position where the context menu was requested
    QModelIndex index = this->indexAt(pos);
    // Create a new QMenu for the context menu
    QMenu contextMenu;

    // Check if the index is valid
    if (index.isValid()) {
        // If so, get the GameObject associated with the index
        GameObject* gameObject = _model->itemFromIndex(index)->data().value<GameObject*>();

        // Check if the GameObject is valid
        if (gameObject) {
            // If so, add a "Create Empty" action to the context menu
            QAction *addEmptyAction = contextMenu.addAction("Create Empty");
            // Connect the triggered signal of the action to the addEmptyGameObject slot
            connect(addEmptyAction, &QAction::triggered, this, &HierarchyTreeView::addEmptyGameObject);

            // Add a "Delete" action to the context menu
            QAction *deleteAction = contextMenu.addAction("Delete");
            // Connect the triggered signal of the action to the RemoveGameObject slot, passing the GUID of the GameObject
            connect(deleteAction, &QAction::triggered, std::bind(&HierarchyTreeView::RemoveGameObject, this, gameObject->guid()));
        }
    } else {
        // If the index is not valid, this is an empty part of the tree view
        // Add a "Create Empty" action to the context menu
        QAction *addEmptyAction = contextMenu.addAction("Create Empty");
        // Connect the triggered signal of the action to the addEmptyGameObject slot
        connect(addEmptyAction, &QAction::triggered, this, &HierarchyTreeView::addEmptyGameObject);
    }

    // Show the context menu at the global position of the context menu event
    contextMenu.exec(this->mapToGlobal(pos));
}
