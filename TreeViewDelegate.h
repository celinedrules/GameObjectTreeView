#ifndef TREEVIEWDELEGATE_H
#define TREEVIEWDELEGATE_H

#include <QEvent>
#include <QMouseEvent>
#include <QStyledItemDelegate>


class TreeViewDelegate : public QStyledItemDelegate {
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
        editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        editor->setStyleSheet("background: #434343;");
        return editor;
    }
};


#endif // TREEVIEWDELEGATE_H
