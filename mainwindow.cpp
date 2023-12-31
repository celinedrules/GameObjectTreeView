#include "gameobject.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QSignalMapper>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create some GameObjects
    GameObject* object1 = new GameObject("Object1", 10, 12);
    GameObject* object2 = new GameObject("Object2", 0, 0, object1);
    GameObject* object3 = new GameObject("Object 3", 0, 0);

    // Add the GameObjects to the gameObjects list
    gameObjects = QList<GameObject*>();
    gameObjects.append(object1);
    gameObjects.append(object2);
    gameObjects.append(object3);

    // Create the HierarchyTreeView with the gameObjects list
    view = new HierarchyTreeView(gameObjects);

    // Create the Add GameObject button and connect its clicked signal to the onButtonAddClicked slot
    buttonAdd = new QPushButton("Add GameObject");
    QObject::connect(buttonAdd, &QPushButton::clicked, this, &MainWindow::onButtonAddClicked);

    // Create the Show GameObject Info button and connect its clicked signal to the onButtonInfoClicked slot
    buttonInfo = new QPushButton("Show GameObject Info");
    QObject::connect(buttonInfo, &QPushButton::clicked, this, &MainWindow::onButtonInfoClicked);

    // Create a QVBoxLayout and add the HierarchyTreeView and buttons to it
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);
    layout->addWidget(buttonAdd);
    layout->addWidget(buttonInfo);

    // Set the layout of the central widget to the QVBoxLayout
    ui->centralwidget->setLayout(layout);

    // Update the HierarchyTreeView
    view->updateTreeView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonAddClicked()
{
    // Add an empty GameObject to the HierarchyTreeView
    view->addEmptyGameObject();
}

void MainWindow::onButtonInfoClicked()
{
    // Get the currently selected GameObject in the HierarchyTreeView
    GameObject* gameObject = view->getCurrentGameObject();

    // If a GameObject is selected, show a message box with its info
    if (gameObject) {
        QMessageBox::information(nullptr, "GameObject Info",
            QString("Name: %1\n x: %2\ny: %3").arg(gameObject->name()).arg(gameObject->x()).arg(gameObject->y()));
        }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // Check if the event was a mouse button press on the viewport of the HierarchyTreeView
    if (obj == view->viewport() && event->type() == QEvent::MouseButtonPress) {
        // If so, cast the event to a QMouseEvent
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        // Get the index at the position of the mouse event
        QModelIndex index = view->indexAt(mouseEvent->pos());

        // If the index is not valid (i.e., the click was not on an item), clear the selection
        if (!index.isValid()) {
            view->clearSelection();
        }
    }

    // Pass the event on to the parent class
    return QMainWindow::eventFilter(obj, event);
}
