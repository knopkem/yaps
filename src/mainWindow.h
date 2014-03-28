#ifndef mainWindow_h__
#define mainWindow_h__

#include <QDialog>
#include "mover.h"

class QGroupBox;
class MainWindowPrivate;

/**
 * The main window of the desktop application
 */
class MainWindow : public QDialog
{
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow(void);

protected slots:
    void onSourceButtonClicked();
    void onTargetButtonClicked();

    void evaluateFolderStructure();
    void evaluateFilenameStructure();

    void editFilePattern();
    void editFolderPattern();

    void doMove();
    void doCopy();

    void onDetermineState();

protected:
    QGroupBox* createIntroGroup();
    QGroupBox* createSourceGroup();
    QGroupBox* createTargetGroup();
    QGroupBox* createFolderGroup();
    QGroupBox* createFileGroup();
    QGroupBox* createActionGroup();

    QString openFileDialog();
    void evaluateFileOptions();
    bool validateSelection();
    void readSettings();
    void writeSettings();

private:
    MainWindowPrivate* d;
};

#endif // mainWindow_h__
