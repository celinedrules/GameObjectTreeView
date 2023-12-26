#include <QStandardItem>
#include <QUuid>

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

/**
 * @class GameObject
 * @brief Base class for all entities on the scene
 */
class GameObject
{
public:
    /**
     * @brief GameObject Creates a new GameObject
     */

    GameObject();
    /**
     * @brief GameObject Creates a new GameObject
     * @param name The name of the GameObject
     * @param x The x-pos of the GameObject in the scene
     * @param y The y-pos of the GameObject in the scene
     * @param parent The GameObjects's parent
     */
    GameObject(const QString &name, int x = 0, int y = 0, GameObject* parent = nullptr);

    ~GameObject();

    /**
     * @brief guid Gets the unique id of the GameObject as a string
     * @return The unique id of the GameObject a s a string
     */
    QString guid() const;

    /**
     * @brief name Gets the name of the GameObject
     * @return The name of the GameObject
     */
    QString name() const;

    /**
     * @brief x Gets the x-pos of the GameObject
     * @return The x-pos of the GameObject
     */
    int x() const;

    /**
     * @brief y Gets the y-pos of the GameObject
     * @return The y-pos of the GameObject
     */
    int y() const;

    /**
     * @brief visible Gets the visibility of the GameObject in the scene
     * @return A bool if the GameObject is visible in the scene
     */
    bool visible();

    /**
     * @brief parent The GameObjects parent
     * @return A GameObject representing this GameObject's parent
     */
    GameObject* parent() const;

    /**
     * @brief getVisibleIcon Gets the icon that represents the GameObject's visibility
     * @return The visibility icon
     */
    QIcon getVisibleIcon();

    /**
     * @brief setParent Sets the GameObject's parent
     * @param parent The GameObject to set as the parent
     */
    void setParent(GameObject* parent);

    /**
     * @brief setName Sets the GameObject's name
     * @param name The name of the GameObject
     */
    void setName(QString name);

    /**
     * @brief setVisible Sets the visibility of the GameObject in the scene
     * @param visible If the GameObject is visible or not in the scene
     */
    void setVisible(bool visible);

    /**
     * @brief setVisibleIcon Sets the icon representing the GameObject's visibility
     * @param icon The icon used to represent the GameObject's visibility
     */
    void setVisibleIcon(const QIcon &icon);

    /**
     * @brief addChild Adds a child GameObject
     * @param child The GameObject to set as a child
     */
    void addChild(GameObject* child);

    /**
     * @brief findChild Finds a child GameObject by name
     * @param name The name of the child GameObject to find
     * @return The child GameObject
     */
    GameObject* findChild(const QString& name) const;

    /**
     * @brief children Gets all child GameObjects
     * @return A list of all child GameObjects
     */
    QList<GameObject*> children() const;

private:
    QString guid_;                  // Unique id of GameObject represented as a string
    QString name_;                  // The name of the GameObject
    int x_;                         // The x-pos of the GameObject
    int y_;                         // The y-pos of the GameObject
    bool visible_;                  // The visibility of the GameObject
    GameObject* parent_;            // The parent GameObject
    QIcon visibileIcon_;            // The icon representing the GameObject's visibility
    QList<GameObject*> children_;   // List of all of the GameObjects children
};

#endif // GAMEOBJECT_H
