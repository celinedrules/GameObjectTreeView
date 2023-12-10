#ifndef TREEITEM_H
#define TREEITEM_H

#include "GameObject.h"


class TreeItem : public QStandardItem {
public:
    explicit TreeItem(GameObject* data) : QStandardItem(), data_(data)
    {
        setText(data->name());
    }

    GameObject* gameObject() const { return data_; }

private:
    GameObject* data_;
};


#endif // TREEITEM_H
