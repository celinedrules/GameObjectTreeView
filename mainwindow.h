#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "hierarchytreeview.h"

#include <gameobject.h>
#include <QMainWindow>
#include <QModelIndex>
#include <QPushButton>
#include <QStandardItem>
#include <QTreeView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStandardItem *findItemInModel(const GameObject *gameObject, QStandardItem *parentItem) const;
public slots:
    void onButtonAddClicked();
    void onButtonInfoClicked();

private:
    Ui::MainWindow *ui;
    QList<GameObject*> gameObjects;
    HierarchyTreeView *view;
    QPushButton *buttonAdd;
    QPushButton *buttonInfo;
    QHash<GameObject*, QStandardItem*> map;
    bool eventFilter(QObject* obj, QEvent* event);
};

#endif // MAINWINDOW_H
