#ifndef composerDlg_h__
#define composerDlg_h__

#include <QDialog>
#include <QModelIndex>

#include "mover.h"

class ComposerDlgPrivate;
class ComposerDlg : public QDialog
{
    Q_OBJECT
public:
    ComposerDlg( QList<PatternFormat::eTag> items, QList<PatternFormat::eTag> selectedItems, QWidget* parent = NULL);
    ~ComposerDlg();

    QList<PatternFormat::eTag> selectedItems() const;

protected slots:
    void copyFromCollection(QModelIndex);
    void removeLast();

protected:
    void build();

private:
    ComposerDlgPrivate* d;
};
#endif // composerDlg_h__

