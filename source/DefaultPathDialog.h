#ifndef DEFAULTPATHDIALOG_H
#define DEFAULTPATHDIALOG_H

#include <QDialog>

namespace Ui {
class DefaultPathDialog;
}

class DefaultPathDialog : public QDialog
{
    Q_OBJECT

private:

public:
    explicit DefaultPathDialog(QWidget *parent = nullptr);
    ~DefaultPathDialog();

private slots:
    void on_btnBrowser_clicked();
    void on_btnConfirm_clicked();

private:
    Ui::DefaultPathDialog *ui;
};

#endif // DEFAULTPATHDIALOG_H
