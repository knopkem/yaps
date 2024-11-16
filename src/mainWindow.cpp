/*
 * Copyright (C) 2014 Michael Knopke
 *
 * See the COPYRIGHT file for terms of use.
 */

#include "mainWindow.h"
#include <version_config.h>
#include "composerDlg.h"

#include <QtGui>
#include <QtWidgets>


//--------------------------------------------------------------------------------------

class MainWindowPrivate {
public:
    QLineEdit*      sourceLineEdit;
    QPushButton*    sourceButton;
    QCheckBox*      traverseCheckBox;
    QLineEdit*      targetLineEdit;
    QPushButton*    targetButton;

    QLineEdit*      patternLineEdit;
    QLineEdit*      exampleLineEdit;
    QPushButton*    editFolderPb;

    QCheckBox*      fixDuplicatesCheckBox;
    QCheckBox*      copyDuplicatesCheckBox;
    QLineEdit*      patternFileLineEdit;
    QLineEdit*      exampleFileLineEdit;
    QPushButton*    editFilePb;

    QPushButton*    moveButton;
    QPushButton*    copyButton;

    Mover*          mover;
    PatternFormat   currentFormat;
    FileOptions     currentOptions;

    QList<PatternFormat::eTag> folderPatternList;
    QList<PatternFormat::eTag> filePatternList;

    QSettings*      settings;
};

//--------------------------------------------------------------------------------------

MainWindow::MainWindow() : d(new MainWindowPrivate)
{
    setWindowFlags( Qt::Dialog );
    this->setWindowTitle( tr("YAPS - ") + VERSION_STRING );
    this->setMinimumWidth(800);

    d->settings = new QSettings(QApplication::organizationName(), QApplication::applicationName());

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(createIntroGroup());
    mainLayout->addWidget(createSourceGroup());
    mainLayout->addWidget(createTargetGroup());
    mainLayout->addWidget(createFolderGroup());
    mainLayout->addWidget(createFileGroup());
    mainLayout->addWidget(createActionGroup());
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    // Create connections
    QObject::connect(d->sourceButton, SIGNAL(clicked()), this, SLOT(onSourceButtonClicked()));
    QObject::connect(d->targetButton, SIGNAL(clicked()), this, SLOT(onTargetButtonClicked()));
    QObject::connect(d->fixDuplicatesCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onDetermineState()));

    QObject::connect(d->editFolderPb, SIGNAL(clicked()), this, SLOT(editFolderPattern()));
    QObject::connect(d->editFilePb, SIGNAL(clicked()), this, SLOT(editFilePattern()));

    QObject::connect(d->moveButton, SIGNAL(clicked()), this, SLOT(doMove()));
    QObject::connect(d->copyButton, SIGNAL(clicked()), this, SLOT(doCopy()));

    d->sourceButton->setFocus();

    d->mover = new Mover(this);

    readSettings();
    evaluateFolderStructure();
    evaluateFilenameStructure();
    onDetermineState();
}

//--------------------------------------------------------------------------------------

MainWindow::~MainWindow( void )
{
    writeSettings();
    delete d->settings;
    delete d;
}

//--------------------------------------------------------------------------------------

QGroupBox* MainWindow::createIntroGroup()
{
    QGroupBox* introGroupBox = new QGroupBox(tr("Introduction"));
    QLabel* introText = new QLabel;
    introText->setText(tr("Welcome to YAPS - Yet Another Photo Sorter.\n"
    "\n"
    "Photos and videos can be easily sorted using the EXIF meta information (date, time, camera model etc.)\n"
    "You can customize the folder and file structure to your needs by editing the default patterns.\n"
    "Duplicates can be detected based on their file hashes (md5sum). Happy sorting!"
    ));
    QVBoxLayout* vboxIntro = new QVBoxLayout;
    vboxIntro->addWidget(introText);

    introGroupBox->setLayout(vboxIntro);
    return introGroupBox;
}

//--------------------------------------------------------------------------------------

QGroupBox* MainWindow::createSourceGroup()
{
    QGroupBox* sourceGroupBox = new QGroupBox(tr("Source Folder"));
    d->sourceLineEdit = new QLineEdit;
    d->sourceLineEdit->setPlaceholderText(tr("Directory where your original photos/movies are stored"));

    d->sourceButton = new QPushButton(tr("Choose source folder"));
    d->sourceButton->setMaximumWidth(200);

    d->traverseCheckBox = new QCheckBox(tr("Include Subdirectories"));
    d->traverseCheckBox->setChecked(true);

    QHBoxLayout *hboxSource = new QHBoxLayout;
    hboxSource->addWidget(d->sourceButton);
    hboxSource->addWidget(d->traverseCheckBox);

    QVBoxLayout *vboxSource = new QVBoxLayout;
    vboxSource->addWidget(d->sourceLineEdit);
    vboxSource->addLayout(hboxSource);
    sourceGroupBox->setLayout(vboxSource);
    return sourceGroupBox;
}

//--------------------------------------------------------------------------------------

QGroupBox* MainWindow::createTargetGroup()
{
    QGroupBox* targetGroupBox = new QGroupBox(tr("Target Folder"));
    d->targetLineEdit = new QLineEdit;
    d->targetLineEdit->setPlaceholderText(tr("Directory where Photo Mover will copy or move them"));

    d->targetButton = new QPushButton(tr("Choose target folder"));
    d->targetButton->setMaximumWidth(200);

    d->fixDuplicatesCheckBox = new QCheckBox(tr("Fix duplicates"));
    d->fixDuplicatesCheckBox->setChecked(true);

    d->copyDuplicatesCheckBox = new QCheckBox(tr("Copy to duplicates folder"));
    d->copyDuplicatesCheckBox->setChecked(false);

    QHBoxLayout *hboxTarget = new QHBoxLayout;
    hboxTarget->addWidget(d->targetButton);
    hboxTarget->addWidget(d->fixDuplicatesCheckBox);
    hboxTarget->addWidget(d->copyDuplicatesCheckBox);

    QVBoxLayout *vboxTarget = new QVBoxLayout;
    vboxTarget->addWidget(d->targetLineEdit);
    vboxTarget->addLayout(hboxTarget);
    targetGroupBox->setLayout(vboxTarget);
    return targetGroupBox;
}

//--------------------------------------------------------------------------------------

QGroupBox* MainWindow::createFolderGroup()
{
    QGroupBox* folderGroupBox = new QGroupBox(tr("Output Folder Structure"));

    d->editFolderPb = new QPushButton(tr("Edit"));
    d->patternLineEdit = new QLineEdit;
    d->patternLineEdit->setReadOnly(true);
    d->exampleLineEdit = new QLineEdit;
    d->exampleLineEdit->setReadOnly(true);

    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(d->patternLineEdit);
    hbox->addWidget(d->editFolderPb);
    
    QFormLayout* formLayoutFolderPattern = new QFormLayout;
    formLayoutFolderPattern->addRow("Pattern:", hbox);
    formLayoutFolderPattern->addRow("Example:", d->exampleLineEdit);
    formLayoutFolderPattern->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    
    folderGroupBox->setLayout(formLayoutFolderPattern);
    return folderGroupBox;
}

//--------------------------------------------------------------------------------------

QGroupBox* MainWindow::createFileGroup()
{
    QGroupBox* fileGroupBox = new QGroupBox(tr("Filename Structure"));

    d->patternFileLineEdit = new QLineEdit;
    d->patternFileLineEdit->setReadOnly(true);
    d->exampleFileLineEdit = new QLineEdit;
    d->exampleLineEdit->setReadOnly(true);

    d->editFilePb = new QPushButton(tr("Edit"));

    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(d->patternFileLineEdit);
    hbox->addWidget(d->editFilePb);

    QFormLayout* formLayoutFilePattern = new QFormLayout;
    formLayoutFilePattern->addRow("Pattern:", hbox);
    formLayoutFilePattern->addRow("Example:", d->exampleFileLineEdit);
    formLayoutFilePattern->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    
    fileGroupBox->setLayout(formLayoutFilePattern);
    return fileGroupBox;
}

//--------------------------------------------------------------------------------------

QGroupBox* MainWindow::createActionGroup()
{
    QGroupBox* actionGroupBox = new QGroupBox(tr("Action"));
    d->copyButton = new QPushButton(tr("COPY Files"));
    d->moveButton = new QPushButton(tr("MOVE Files"));

    QHBoxLayout* hboxAction = new QHBoxLayout;
    hboxAction->addWidget(d->copyButton);
    hboxAction->addWidget(d->moveButton);
    actionGroupBox->setLayout(hboxAction);
    return actionGroupBox;
}

//--------------------------------------------------------------------------------------

void MainWindow::onSourceButtonClicked()
{
    d->sourceLineEdit->setText(openFileDialog());
}

//--------------------------------------------------------------------------------------

void MainWindow::onTargetButtonClicked()
{
    d->targetLineEdit->setText(openFileDialog());
    evaluateFolderStructure();
}

//--------------------------------------------------------------------------------------

QString MainWindow::openFileDialog()
{
    QDir selDir;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    if (dialog.exec()) {
        QStringList selected = dialog.selectedFiles();
        if (!selected.empty())
            return (selected.first());
    }
    return "";
}

//--------------------------------------------------------------------------------------

void MainWindow::evaluateFolderStructure()
{
    d->currentFormat.FolderStructureContainer = d->folderPatternList;

    QString pattern;
    QString example;

    foreach( PatternFormat::eTag tag, d->currentFormat.FolderStructureContainer) {
        pattern += PatternFormat::tagToString(tag);
        example += PatternFormat::tagToExample(tag);
    }
    d->patternLineEdit->setText(pattern);
    d->exampleLineEdit->setText(example);
}

//--------------------------------------------------------------------------------------

void MainWindow::evaluateFilenameStructure()
{
    d->currentFormat.FileStructureContainer = d->filePatternList;

    QString pattern;
    QString example;

    foreach( PatternFormat::eTag tag, d->currentFormat.FileStructureContainer) {
        pattern += PatternFormat::tagToString(tag);
        example += PatternFormat::tagToExample(tag);
    }
    d->patternFileLineEdit->setText(pattern);
    d->exampleFileLineEdit->setText(example);
}

//--------------------------------------------------------------------------------------

void MainWindow::doMove()
{
    if (!validateSelection()) {
        return;
    }

    int ret = QMessageBox::warning(this, tr("Attention"),
                                    tr("Do you really want to move files?\n"
                                       "Consider using copy instead which leaves your source folder intact.\n"
                                       "Proceed at your own risk!"),
                                    QMessageBox::Ok | QMessageBox::Cancel);

    if ( ret == QMessageBox::Ok) {
        evaluateFileOptions();
        d->currentOptions.fileOp = FileOptions::MOVE;
        d->mover->performOperations(d->sourceLineEdit->text(), d->targetLineEdit->text(), d->currentOptions, d->currentFormat);
    }
}

//--------------------------------------------------------------------------------------

void MainWindow::doCopy()
{
    if (!validateSelection()) {
        return;
    }

    evaluateFileOptions();
    d->currentOptions.fileOp = FileOptions::COPY;
    d->mover->performOperations(d->sourceLineEdit->text(), d->targetLineEdit->text(), d->currentOptions, d->currentFormat);
}

//--------------------------------------------------------------------------------------

void MainWindow::evaluateFileOptions()
{
     d->currentOptions.traverseSubdirectories = d->traverseCheckBox->isChecked();
     d->currentOptions.fixDuplicates = d->fixDuplicatesCheckBox->isChecked();
     d->currentOptions.copyDuplicates = d->copyDuplicatesCheckBox->isChecked();
}

//--------------------------------------------------------------------------------------

void MainWindow::editFolderPattern()
{
    QList<PatternFormat::eTag> items;
    items << PatternFormat::NewSubDir;
    items << PatternFormat::DelimiterDash;
    items << PatternFormat::DelimiterUnderscore;
    items << PatternFormat::DelimiterDot;
    items << PatternFormat::DelimiterHash;
    items << PatternFormat::DelimiterTilde;
    items << PatternFormat::DelimiterWhiteSpace;
    items << PatternFormat::CameraMake;
    items << PatternFormat::CameraModel;
    items << PatternFormat::MediaType;
    items << PatternFormat::Year;
    items << PatternFormat::Month;
    items << PatternFormat::MonthS;
    items << PatternFormat::MonthL;
    items << PatternFormat::Day;
    items << PatternFormat::DayS;
    items << PatternFormat::DayL;
    items << PatternFormat::WeekNumber;

    QString description = tr("Usage: Compose your desired folder structure by clicking on the items in the selection list.\n"
        "Inserting a subdirectory element will mark the beginning of a new sub folder.");

    ComposerDlg composer(items, d->folderPatternList, description);
    composer.setModal(true);
    int ret = composer.exec();

    switch (ret) {
    case QDialog::Accepted:
        d->folderPatternList = composer.selectedItems();
        evaluateFolderStructure();
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------------------------------

void MainWindow::editFilePattern()
{
    QList<PatternFormat::eTag> items;
    items << PatternFormat::DelimiterDash;
    items << PatternFormat::DelimiterUnderscore;
    items << PatternFormat::DelimiterDot;
    items << PatternFormat::DelimiterHash;
    items << PatternFormat::DelimiterTilde;
    items << PatternFormat::DelimiterWhiteSpace;
    items << PatternFormat::CameraMake;
    items << PatternFormat::CameraModel;
    items << PatternFormat::MediaType;
    items << PatternFormat::Year;
    items << PatternFormat::Month;
    items << PatternFormat::MonthS;
    items << PatternFormat::MonthL;
    items << PatternFormat::Day;
    items << PatternFormat::DayS;
    items << PatternFormat::DayL;
    items << PatternFormat::Hour;
    items << PatternFormat::Minute;
    items << PatternFormat::Second;
    items << PatternFormat::Filename;

    QString description = tr("Usage: Compose your desired filename pattern by clicking on the items in the selection list.");

    ComposerDlg composer(items, d->filePatternList, description);
    composer.setModal(true);
    int ret = composer.exec();

    switch (ret) {
    case QDialog::Accepted:
        d->filePatternList = composer.selectedItems();
        evaluateFilenameStructure();
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------------------------------

bool MainWindow::validateSelection()
{
    if (d->filePatternList.isEmpty()) {
        QMessageBox::warning(this, tr("Attention"),
            tr("The file name pattern cannot be empty!"),
            QMessageBox::Ok);
        return false;
    }

    if ( !Mover::dirExists(d->sourceLineEdit->text())) {
        QMessageBox::warning(this, tr("Attention"),
            tr("Invalid source folder!"),
            QMessageBox::Ok);
        return false;
    }

    if ( !Mover::makedir(d->targetLineEdit->text()) ) {
        QMessageBox::warning(this, tr("Attention"),
            tr("Cannot create target folder!"),
            QMessageBox::Ok);
            return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------

void MainWindow::readSettings()
{
	QString inputPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
	QString outputPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/yaps_output";

    QVariantList defaultFolderPattern;
    defaultFolderPattern << PatternFormat::Year;
    defaultFolderPattern << PatternFormat::NewSubDir;
    defaultFolderPattern << PatternFormat::Month;
    defaultFolderPattern << PatternFormat::DelimiterDash;
    defaultFolderPattern << PatternFormat::MonthL;

    QVariantList varList = d->settings->value("FolderPattern", defaultFolderPattern).toList();
    foreach(const QVariant & var, varList) {
        if (var.canConvert<PatternFormat::eTag>()) {
            d->folderPatternList << var.value<PatternFormat::eTag>();
        }
    }

    QVariantList defaultFilePattern;
    defaultFilePattern << PatternFormat::Day;
    defaultFilePattern << PatternFormat::DelimiterDash;
    defaultFilePattern << PatternFormat::MonthS;
    defaultFilePattern << PatternFormat::DelimiterDash;
    defaultFilePattern << PatternFormat::Hour;
    defaultFilePattern << PatternFormat::Minute;
    defaultFilePattern << PatternFormat::Second;

    QVariantList varList2 = d->settings->value("FilePattern", defaultFilePattern).toList();
    foreach(const QVariant& var, varList2) {
        if (var.canConvert<PatternFormat::eTag>()) {
            d->filePatternList << var.value<PatternFormat::eTag>();
        }
    }

    d->sourceLineEdit->setText(d->settings->value("SourceFolder", inputPath).toString());
    d->targetLineEdit->setText(d->settings->value("TargetFolder", outputPath).toString());
}

//--------------------------------------------------------------------------------------

void MainWindow::writeSettings()
{
    QVariantList currentFolderPattern;
    foreach(PatternFormat::eTag tag, d->folderPatternList) {
        currentFolderPattern << tag;
    }
    d->settings->setValue("FolderPattern", currentFolderPattern);

    QVariantList currentFilePattern;
    foreach(PatternFormat::eTag tag, d->filePatternList) {
        currentFilePattern << tag;
    }
    d->settings->setValue("FilePattern", currentFilePattern);

    d->settings->setValue("SourceFolder", d->sourceLineEdit->text());
    d->settings->setValue("TargetFolder", d->targetLineEdit->text());
    d->settings->sync();
}

//--------------------------------------------------------------------------------------

void MainWindow::onDetermineState()
{
    bool enabled = d->fixDuplicatesCheckBox->isChecked();
    d->copyDuplicatesCheckBox->setEnabled(enabled);
}

//--------------------------------------------------------------------------------------



