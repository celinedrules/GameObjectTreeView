#ifndef HIERARCHYTREEEVIEW_H
#define HIERARCHYTREEEVIEW_H

#include <QContextMenuEvent>
#include <QTreeView>



class HierarchyTreeeView : public QTreeView
{
    Q_OBJECT
public:
    explicit HierarchyTreeeView(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent *event) override;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void startDrag(Qt::DropActions supportedActions) override;

private:
    QPoint dragStartPosition;
};

#endif // HIERARCHYTREEEVIEW_H
