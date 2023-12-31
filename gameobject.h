#include <QStandardItem>
#include <QUuid>

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

/**
 * @class GameObject
 * @brief Represents a game object in a hierarchy
 *
 * This class represents a game object with a unique identifier (GUID), name, position (x, y), visibility status, parent, visibility icon, and a list of child game objects.
 */
class GameObject
{
public:
    /**
     * @brief Default constructor
     */
    GameObject();

    /**
     * @brief Constructs a GameObject with the given name, position, and parent
     *
     * @param name The name of the GameObject
     * @param x The x-coordinate of the GameObject's position
     * @param y The y-coordinate of the GameObject's position
     * @param parent The parent GameObject
     */
    GameObject(const QString &name, int x = 0, int y = 0, GameObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~GameObject();

    /**
     * @brief Returns the GUID of the GameObject
     *
     * @return The GUID of the GameObject
     */
    QString guid() const;

    /**
     * @brief Returns the name of the GameObject
     *
     * @return The name of the GameObject
     */
    QString name() const;

    /**
     * @brief Returns the x-coordinate of the GameObject's position
     *
     * @return The x-coordinate of the GameObject's position
     */
    int x() const;

    /**
     * @brief Returns the y-coordinate of the GameObject's position
     *
     * @return The y-coordinate of the GameObject's position
     */
    int y() const;

    /**
     * @brief Returns the visibility status of the GameObject
     *
     * @return The visibility status of the GameObject
     */
    bool visible();

    /**
     * @brief Returns the parent GameObject
     *
     * @return The parent GameObject
     */
    GameObject* parent() const;

    /**
     * @brief Returns the visibility icon of the GameObject
     *
     * @return The visibility icon of the GameObject
     */
    QIcon getVisibleIcon();

    /**
     * @brief Sets the parent GameObject
     *
     * @param parent The new parent GameObject
     */
    void setParent(GameObject* parent);

    /**
     * @brief Sets the name of the GameObject
     *
     * @param name The new name of the GameObject
     */
    void setName(QString name);

    /**
     * @brief Sets the visibility status of the GameObject
     *
     * @param visible The new visibility status of the GameObject
     */
    void setVisible(bool visible);

    /**
     * @brief Sets the visibility icon of the GameObject
     *
     * @param icon The new visibility icon of the GameObject
     */
    void setVisibleIcon(const QIcon &icon);

    /**
     * @brief Adds a child GameObject
     *
     * @param child The new child GameObject
     */
    void addChild(GameObject* child);

    /**
     * @brief Finds a child GameObject by name
     *
     * @param name The name of the child GameObject to find
     * @return The found child GameObject, or nullptr if no child GameObject with the given name exists
     */
    GameObject* findChild(const QString& name) const;

    /**
     * @brief Returns the list of child GameObjects
     *
     * @return The list of child GameObjects
     */
    QList<GameObject*> children() const;

private:
    // The GUID of the GameObject
    QString guid_;
    // The name of the GameObject
    QString name_;
    // The x-coordinate of the GameObject's position
    int x_;
    // The y-coordinate of the GameObject's position
    int y_;
    // The visibility status of the GameObject
    bool visible_;
    // The parent GameObject
    GameObject* parent_;
    // The visibility icon of the GameObject
    QIcon visibileIcon_;
    // The list of child GameObjects
    QList<GameObject*> children_;
};

#endif // GAMEOBJECT_H
