#ifndef HIERARCHYBUTTONDELEGATE_H
#define HIERARCHYBUTTONDELEGATE_H

#include "gameobject.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QStyledItemDelegate>

class HierarchyButtonDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    HierarchyButtonDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void buttonClicked(const QModelIndex &index) const;
};


#endif // HIERARCHYBUTTONDELEGATE_H
