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

/**
 * @class MainWindow
 * @brief Main window of the application
 *
 * This class represents the main window of the application. It inherits from QMainWindow and contains various UI elements and game objects
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a MainWindow with the given parent
     *
     * @param parent The parent widget of the MainWindow
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for MainWindow
     */
    ~MainWindow();

    /**
     * @brief Finds an item in the model corresponding to a GameObject
     *
     * @param gameObject The GameObject to find
     * @param parentItem The parent item to start the search from
     * @return The found item, or nullptr if no item was found
     */
    QStandardItem *findItemInModel(const GameObject *gameObject, QStandardItem *parentItem) const;
public slots:
    /**
     * @brief Slot to handle the Add button being clicked
     */
    void onButtonAddClicked();

    /**
     * @brief Slot to handle the Info button being clicked
     */
    void onButtonInfoClicked();

private:
    /**
     * @brief Filters events for the MainWindow
     *
     * @param obj The object that the event was sent to
     * @param event The event that was sent
     * @return True if the event was filtered (i.e., it should be ignored), false otherwise
     */
    bool eventFilter(QObject* obj, QEvent* event);

    // The UI for the MainWindow
    Ui::MainWindow *ui;
    // The list of game objects
    QList<GameObject*> gameObjects;
    // The hierarchy tree view
    HierarchyTreeView *view;
    // // The Add button
    QPushButton *buttonAdd;
    // The Info button
    QPushButton *buttonInfo;
    // A map from GameObjects to items in the model
    QHash<GameObject*, QStandardItem*> map;
};

#endif // MAINWINDOW_H
