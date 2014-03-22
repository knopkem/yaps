#ifndef reportDlg_h__
#define reportDlg_h__

#include <QDialog>

struct ReportDetail {
    QString ElapsedTime;
    int FilesTotal;
    int FilesWithExif;
    int FilesFailed;
    int FilesCopied;
    int Duplicates;
    QString LogfilePath;
};

class ReportDlgPrivate;
class ReportDlg : public QDialog
{
    Q_OBJECT
public:
    ReportDlg(QWidget *parent = 0);
    ~ReportDlg();

    void setReportDetail(const ReportDetail& detail);

protected slots:
    void openLogFile();

private:
    ReportDlgPrivate* d;
};

#endif // reportDlg_h__

