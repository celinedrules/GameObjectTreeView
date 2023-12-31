#ifndef HIERARCHYTREEVIEWDELEGATE_H
#define HIERARCHYTREEVIEWDELEGATE_H

#include <QEvent>
#include <QMouseEvent>
#include <QStyledItemDelegate>

/**
 * @class HierarchyTreeViewDelegate
 * @brief Custom delegate for HierarchyTreeView
 *
 * This class inherits from QStyledItemDelegate and provides a custom delegate for HierarchyTreeView
 */
class HierarchyTreeViewDelegate : public QStyledItemDelegate {
public:
    /**
     * @brief Creates a custom editor widget
     *
     * This function is called when the user starts editing an item in the HierarchyTreeView. It overrides the createEditor function from QStyledItemDelegate to provide a custom editor widget
     *
     * @param parent The parent widget of the editor
     * @param option Contains the style options for the item
     * @param index The model index of the item
     * @return The created editor widget
     */
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};


#endif // HIERARCHYTREEVIEWDELEGATE_H
