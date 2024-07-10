#include "VotingPage.h"
#include "ui_VotingPage.h"

VotingPage::VotingPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VotingPage)
{
    ui->setupUi(this);

    addCandidates();

}

VotingPage::~VotingPage()
{
    delete ui;
}

void VotingPage::addCandidates()
{
    QSqlQuery query ;
    QSqlRecord record ;

    query.prepare("SELECT * FROM condidates");

    if(query.exec()) {
        while(query.next()) {
        record = query.record() ;
        QString candidateName = record.value("name").toString() ;
        QString candidatePic = QString(":/pics/res/"+ candidateName + ".png");
        qDebug() << candidatePic ;
        QPushButton *candidate = new QPushButton() ;
        candidate->setProperty("id" , record.value("id").toInt()) ;
        candidate->setProperty("name" , record.value("name").toString()) ;
        candidate->setIcon(QIcon(candidatePic));
        candidate->setIconSize(candidate->size());
        candidate->setCursor(Qt::PointingHandCursor);
        connect(candidate , &QPushButton::clicked , this , &VotingPage::finalChoose) ;
        ui->horizontalLayout->addWidget(candidate);
        }
    } else {
        QMessageBox::critical(this, "" , "not connected to dataBase") ;
        qDebug() << query.lastError().text();
    }

}

void VotingPage::finalChoose()
{
    auto candidate = qobject_cast<QPushButton *>(sender()) ;

    int ret = QMessageBox::information(this , "" , QString( "Your choice :\n\n" +
                                                   candidate->property("name").toString() +
                                                   "\n\nAre you sure ?") ,
                                                   QMessageBox::Yes , QMessageBox::No) ;

    if(ret) {
        MainWindow *m = new MainWindow() ;
        connect(this , &VotingPage::candidateChosen , m , &MainWindow::candidateChosn) ;
        emit candidateChosen(candidate->property("id").toInt());
        m->show();
        this->close();
    } else {
        return;
    }
}
