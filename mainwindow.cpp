#include "GameObject.h"
#include "TreeViewDelegate.h"
#include "buttonDelegate.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QIODevice>
#include <QFile>
#include <QTimer>
#include <QSignalMapper>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    treeViewDelegate = new TreeViewDelegate;
    btnDelegate = new ButtonDelegate(this);
    connect(btnDelegate, &ButtonDelegate::buttonClicked, this, &MainWindow::visibleClicked);

    //headers << "Name" << "Icon";

    // Load an application style
    QFile styleFile(":/resources/stylesheets/stylesheet.qss");
    styleFile.open(QFile::ReadOnly);

    // Apply the loaded stylesheet
    style = QString(styleFile.readAll());

    GameObject* object1 = new GameObject("Object1", 10, 12);
    GameObject* object2 = new GameObject("Object2", 0, 0, object1);

    gameObjects = QList<GameObject*>();
    gameObjects.append(object1);
    gameObjects.append(object2);

    //view = new QTreeView();
    view = new HierarchyTreeeView();

    model = new TreeModel();

    setupModel();

    view->setModel(model);

    connect(model, &QStandardItemModel::itemChanged, this, &MainWindow::onItemChanged);
    connect(view, &QTreeView::customContextMenuRequested, this, &MainWindow::showContextMenu);

    buttonAdd = new QPushButton("Add GameObject");
    QObject::connect(buttonAdd, &QPushButton::clicked, this, &MainWindow::onButtonAddClicked);

    buttonInfo = new QPushButton("Show GameObject Info");
    QObject::connect(buttonInfo, &QPushButton::clicked, this, &MainWindow::onButtonInfoClicked);

    //connect(view, &QTreeView::entered, btnDelegate, &ButtonDelegate::setHoveredIndex);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);
    layout->addWidget(buttonAdd);
    layout->addWidget(buttonInfo);

    ui->centralwidget->setLayout(layout);
    updateTreeView(gameObjects, model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTreeView(QList<GameObject*>& gameObjects, QStandardItemModel *model) {
    saveExpandedState();
    model->clear();
    setupModel();

    // Create a map to store QStandardItem pointers for each GameObject
    QHash<GameObject*, QStandardItem*> itemMap;

    for (GameObject* gameObject : gameObjects) {
        QStandardItem* nameItem = new QStandardItem(gameObject->name());
        nameItem->setData(QVariant::fromValue(gameObject)); // Add this line

        QStandardItem* iconItem = new QStandardItem();
        //iconItem->setData(QIcon(gameObject->icon()), Qt::DecorationRole);
        iconItem->setData(QVariant::fromValue(gameObject)); // Store the GameObject pointer in the icon item as well

        QList<QStandardItem*> rowItems;
        rowItems << nameItem << iconItem; // Changed the order here

        // Store the QStandardItem in the map for future reference
        itemMap[gameObject] = nameItem; // Changed to nameItem

        if (gameObject->parent()) {
            // Find the parent QStandardItem in the map
            QStandardItem* parentItem = itemMap.value(gameObject->parent(), nullptr);

            if (parentItem) {
                // Append the current item as a child of the parent item
                parentItem->appendRow(rowItems);
            }
        } else {
            // If there is no parent, add the current item directly to the model
            model->appendRow(rowItems);
        }
    }
    restoreExpandedState();
}

void MainWindow::setupModel()
{
    model->clear();
    model->setColumnCount(2);
    //model->setHorizontalHeaderLabels(headers);

    view->header()->resizeSection(1, 10);
    view->header()->setSectionsMovable(true);
    view->header()->swapSections(0, 1);
    view->header()->setHidden(true);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setAlternatingRowColors(true);
    view->setStyleSheet(style);
    view->header()->setStretchLastSection(true);
    view->setItemDelegate(treeViewDelegate);
    view->setItemDelegateForColumn(1, btnDelegate);
    view->setMouseTracking(true);
    view->viewport()->installEventFilter(this);

}

void MainWindow::onButtonAddClicked()
{
    addEmptyGameObject();
}

void MainWindow::onButtonInfoClicked()
{
    QModelIndex index = view->currentIndex();
    if (index.isValid()) {
        GameObject* gameObject = model->itemFromIndex(index)->data().value<GameObject*>();
        if (gameObject) {
            QMessageBox::information(nullptr, "GameObject Info",
                                     QString("Name: %1\n x: %2\ny: %3").arg(gameObject->name()).arg(gameObject->x()).arg(gameObject->y()));
        }
    }
}

void MainWindow::visibleClicked(QModelIndex index)
{
    if (index.isValid()) {
        QModelIndex adjustedIndex;
        if (index.parent().isValid()) {
            // It's a child item, use the original column index
            adjustedIndex = model->index(index.row(), index.column(), index.parent());
        } else {
            // It's a top-level item, adjust the column index to 0
            adjustedIndex = model->index(index.row(), 0);
        }
        GameObject* gameObject = model->itemFromIndex(adjustedIndex)->data().value<GameObject*>();

        if (gameObject) {
            // Now you have the correct GameObject
            qDebug() << gameObject->name();
            gameObject->setVisible(!gameObject->visible());
            if(gameObject->visible()){
                gameObject->setIcon(QIcon(":/resources/icons/visible.png"));
            }
            else{
                gameObject->setIcon(QIcon(":/resources/icons/visible2.png"));
            }

            QTimer::singleShot(0, this, [=] {
                    updateTreeView(gameObjects, model);
                });
        }
    }
}

void MainWindow::onItemChanged(QStandardItem *item)
{
    if (item) {
        GameObject* gameObject = item->data().value<GameObject*>();
        if (gameObject) {
            gameObject->setName(item->text());
        }
    }
}

void MainWindow::removeSelectedRow(QString guid)
{
    model->removeRow(guid);
}

void MainWindow::saveExpandedState(const QModelIndex &parent)
{
    for(int i = 0; i < model->rowCount(parent); ++i) {
        QModelIndex idx = model->index(i, 0, parent);
        GameObject* gameObject = model->itemFromIndex(idx)->data().value<GameObject*>();
        // Check if the GameObject is in the gameObjects list and is not null
        if (gameObjects.contains(gameObject) && gameObject && view->isExpanded(idx)) {
            expandedItems.insert(gameObject->guid());
        }
        if (model->hasChildren(idx)) {
            saveExpandedState(idx);
        }
    }
}

void MainWindow::restoreExpandedState(const QModelIndex &parent)
{
    for(int i = 0; i < model->rowCount(parent); ++i) {
        QModelIndex idx = model->index(i, 0, parent);
        GameObject* gameObject = model->itemFromIndex(idx)->data().value<GameObject*>();
        if (gameObject && expandedItems.contains(gameObject->guid())) {
            view->setExpanded(idx, true);
        }
        if (model->hasChildren(idx)) {
            restoreExpandedState(idx);
        }
    }
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QModelIndex index = view->indexAt(pos);
    QMenu contextMenu;
    if (index.isValid()) {
        GameObject* gameObject = model->itemFromIndex(index)->data().value<GameObject*>();
        if (gameObject) {
            QAction *addEmptyAction = contextMenu.addAction("Create Empty");
            connect(addEmptyAction, &QAction::triggered, this, &MainWindow::addEmptyGameObject);
            QAction *deleteAction = contextMenu.addAction("Delete");
            //connect(deleteAction, &QAction::triggered, this, &MainWindow::RemoveGameObject);
            connect(deleteAction, &QAction::triggered, std::bind(&MainWindow::RemoveGameObject, this, gameObject->guid()));
        }
    } else {
        // This is an empty part of the tree view
        QAction *addEmptyAction = contextMenu.addAction("Create Empty");
        connect(addEmptyAction, &QAction::triggered, this, &MainWindow::addEmptyGameObject);
        QAction *action4 = contextMenu.addAction("Action 4");
        // Connect the actions to slots...
    }
    contextMenu.exec(view->mapToGlobal(pos));
}

void MainWindow::addEmptyGameObject()
{
    QString baseName = "GameObject";
    QString name = baseName;
    int i = 1;
    GameObject* parent = nullptr;
    QModelIndex index = view->currentIndex();
    if (index.isValid()) {
        parent = model->itemFromIndex(index)->data().value<GameObject*>();
        view->setExpanded(index, true);
        qDebug() << "Parent: " <<model->itemFromIndex(index)->data();
        // Check if the parent already contains a GameObject with the same name
        while (parent->findChild(name) != nullptr) {
            // If it does, append a number to the name and increment it
            name = baseName + " (" + QString::number(i++) + ")";
        }
    }
    GameObject* gameObject = new GameObject(name, 3, 99, parent);
    gameObjects.append(gameObject);
    qDebug() << gameObject->guid();

    updateTreeView(gameObjects, model);

    // Enter edit mode for the name of the new GameObject
    QModelIndex newIndex = model->indexFromItem(gameObject, QModelIndex());
    if (newIndex.isValid()) {
        view->edit(newIndex);
    }
}

void MainWindow::RemoveGameObject(const QString guid){
    for (int i = 0; i < gameObjects.size(); ++i) {
        if (gameObjects[i]->guid() == guid) {
            // Check if the GameObject is a parent
            if (!gameObjects[i]->children().isEmpty()) {
                // If it is a parent, delete all its children
                for (GameObject* child : gameObjects[i]->children()) {
                    gameObjects.removeOne(child);  // Remove the child from the gameObjects list
                    //delete child;
                }
                gameObjects[i]->children().clear();  // Clear the list of children
            }

            delete gameObjects[i];  // Delete the GameObject
            gameObjects.removeAt(i);  // Remove it from the list
            break;  // Exit the loop
        }
    }

    updateTreeView(gameObjects, model);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == view->viewport() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QModelIndex index = view->indexAt(mouseEvent->pos());
        if (!index.isValid()) {
            // The click was not on an item, clear the selection
            view->clearSelection();
        }
    }
    // Pass the event on to the parent class
    return QMainWindow::eventFilter(obj, event);
}


