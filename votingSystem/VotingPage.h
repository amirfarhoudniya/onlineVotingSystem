#ifndef VOTINGPAGE_H
#define VOTINGPAGE_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QPushButton>

#include "dataBase.h"
#include "mainwindow.h"

namespace Ui {
class VotingPage;
}

class VotingPage : public QWidget
{
    Q_OBJECT

public:
    explicit VotingPage(QWidget *parent = nullptr);
    ~VotingPage();

signals:
    void candidatePushed() ;
    void candidateChosen(int _candidateNum);

private:
    //variables
    Ui::VotingPage *ui;

    //
    void addCandidates() ;
    void finalChoose() ;
};

#endif // VOTINGPAGE_H
