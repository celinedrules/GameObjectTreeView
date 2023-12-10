#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "TreeModel.h"
#include "TreeViewDelegate.h"
#include "buttonDelegate.h"
#include "hierarchytreeeview.h"

#include <GameObject.h>
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
    void updateTreeView(QList<GameObject*>& gameObjects, QStandardItemModel *model);
    void setupModel();
    QStandardItem *findItemInModel(const GameObject *gameObject, QStandardItem *parentItem) const;
public slots:
    void onButtonAddClicked();
    void onButtonInfoClicked();
    void visibleClicked(QModelIndex index);
    void onItemChanged(QStandardItem *item);
    void removeSelectedRow(QString guid);

private:
    Ui::MainWindow *ui;
    QList<GameObject*> gameObjects;
    //QTreeView *view;
    HierarchyTreeeView *view;
    TreeModel *model;
    QPushButton *buttonAdd;
    QPushButton *buttonInfo;
    QHash<GameObject*, QStandardItem*> map;
    ButtonDelegate *btnDelegate;
    TreeViewDelegate *treeViewDelegate;
    //QStringList headers;
    QString style;
    QSet<QString> expandedItems;
    void saveExpandedState(const QModelIndex &parent = QModelIndex());
    void restoreExpandedState(const QModelIndex &parent = QModelIndex());
    void showContextMenu(const QPoint &pos);
    void addEmptyGameObject();
    void RemoveGameObject(const QString guid);
    bool eventFilter(QObject* obj, QEvent* event);
};

#endif // MAINWINDOW_H
