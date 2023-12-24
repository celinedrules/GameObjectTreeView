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

    GameObject* object1 = new GameObject("Object1", 10, 12);
    GameObject* object2 = new GameObject("Object2", 0, 0, object1);
    GameObject* object3 = new GameObject("Object 3", 0, 0);

    gameObjects = QList<GameObject*>();
    gameObjects.append(object1);
    gameObjects.append(object2);
    gameObjects.append(object3);

    view = new HierarchyTreeView(gameObjects);

    buttonAdd = new QPushButton("Add GameObject");
    QObject::connect(buttonAdd, &QPushButton::clicked, this, &MainWindow::onButtonAddClicked);

    buttonInfo = new QPushButton("Show GameObject Info");
    QObject::connect(buttonInfo, &QPushButton::clicked, this, &MainWindow::onButtonInfoClicked);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);
    layout->addWidget(buttonAdd);
    layout->addWidget(buttonInfo);

    ui->centralwidget->setLayout(layout);
    view->updateTreeView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonAddClicked()
{
    view->addEmptyGameObject();
}

void MainWindow::onButtonInfoClicked()
{
    GameObject* gameObject = view->getCurrentGameObject();

    if (gameObject) {
        QMessageBox::information(nullptr, "GameObject Info",
            QString("Name: %1\n x: %2\ny: %3").arg(gameObject->name()).arg(gameObject->x()).arg(gameObject->y()));
        }
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


