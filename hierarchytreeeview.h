#ifndef HIERARCHYTREEEVIEW_H
#define HIERARCHYTREEEVIEW_H

#include <QContextMenuEvent>
#include <QTreeView>



class HierarchyTreeeView : public QTreeView
{
    Q_OBJECT
public:
    explicit HierarchyTreeeView(QWidget* parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // HIERARCHYTREEEVIEW_H
