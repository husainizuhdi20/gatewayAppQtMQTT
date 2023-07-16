#ifndef DIALOGLABEL_H
#define DIALOGLABEL_H

#include <QDialog>



namespace Ui {
class dialogLabel;
}

class dialogLabel : public QDialog
{
    Q_OBJECT

public:
    explicit dialogLabel(QWidget *parent = nullptr);
    ~dialogLabel();

private:
    Ui::dialogLabel *ui;
};

#endif // DIALOGLABEL_H
