/*
 * Copyright (C) 2014 Michael Knopke
 *
 * See the COPYRIGHT file for terms of use.
 */

#include "composerDlg.h"

#include <QtGui>

class ComposerDlgPrivate
{
public:
    QListWidget* collection;
    QList<PatternFormat::eTag> selection;
    QLineEdit* pattern;
    QLineEdit* example;
    QLabel* description;
};

//--------------------------------------------------------------------------------------

ComposerDlg::ComposerDlg( QList<PatternFormat::eTag> items, QList<PatternFormat::eTag> selectedItems, 
   const QString& labelText, QWidget* parent /*= NULL*/ ): QDialog(parent), d (new ComposerDlgPrivate)
{
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
    this->setMinimumWidth(800);

    d->description = new QLabel;
    d->description->setText(labelText);
    d->collection = new QListWidget;
    d->collection->setSelectionMode(QAbstractItemView::SingleSelection);

    d->selection = selectedItems;

    d->pattern = new QLineEdit;
    d->pattern->setReadOnly(true);
    d->example = new QLineEdit;
    d->example->setReadOnly(true);

    // fill collection
    foreach(PatternFormat::eTag item, items) {
        QListWidgetItem * it = new QListWidgetItem;
        it->setText(PatternFormat::tagToString(item) + " - " + PatternFormat::tagToDescription(item));
        it->setData(Qt::UserRole, item);
        d->collection->addItem(it);
    }

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    connect(d->collection, SIGNAL(clicked(QModelIndex)), this, SLOT(copyFromCollection(QModelIndex)));

    QPushButton* minusButton = new QPushButton(tr("<-"));
    connect(minusButton,SIGNAL(clicked()), this, SLOT(removeLast()));

    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(d->pattern);
    hbox->addWidget(minusButton);

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(d->description);
    vbox->addWidget(d->collection);
    vbox->addLayout(hbox);
    vbox->addWidget(d->example);
    vbox->addWidget(buttonBox);

    build();

    this->setLayout(vbox);
}

//--------------------------------------------------------------------------------------

ComposerDlg::~ComposerDlg()
{
    delete d;
    d = NULL;
}

//--------------------------------------------------------------------------------------

void ComposerDlg::copyFromCollection( QModelIndex index)
{
    PatternFormat::eTag tag = static_cast<PatternFormat::eTag>(d->collection->item(index.row())->data(Qt::UserRole).toInt());
    if ( (tag == PatternFormat::NewSubDir) && (d->selection.last() ==  PatternFormat::NewSubDir)) {
         return;
    }
    d->selection << tag;
    build();
}

//--------------------------------------------------------------------------------------

QList<PatternFormat::eTag> ComposerDlg::selectedItems() const
{
    return d->selection;
}

//--------------------------------------------------------------------------------------

void ComposerDlg::build()
{
    QString textPattern;
    QString textExample;
    for (int i=0; i < d->selection.count(); i++) {
        PatternFormat::eTag tag = d->selection.at(i);
        textPattern += PatternFormat::tagToString(tag);
        textExample += PatternFormat::tagToExample(tag);
    }

    d->pattern->setText(textPattern);
    d->example->setText(textExample);
}

//--------------------------------------------------------------------------------------

void ComposerDlg::removeLast()
{
    if (d->selection.isEmpty()) {
        return;
    }
    d->selection.takeLast();
    build();
}

//--------------------------------------------------------------------------------------

