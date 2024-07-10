#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //
    ui->id_label->setAlignment(Qt::AlignCenter);
    ui->status_label->setAlignment(Qt::AlignCenter);
    ui->id_lineEdit->setAlignment(Qt::AlignCenter);

    client = new QTcpSocket() ;
    connect(client , &QTcpSocket::connected , this , &MainWindow::connectedToServer) ;
    connect(client , &QTcpSocket::readyRead , this , &MainWindow::dataReceived ) ;

    connectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectedToServer()
{
    if(client->isOpen()) {
        qDebug() << "connected to Server " ;
        ui->status_label->setStyleSheet("color:green;");
        ui->status_label->setText("connected to Server");
    } else {
        qDebug() << client->errorString() ;
        ui->status_label->setStyleSheet("color:red;");
        ui->status_label->setText("not connected to server");
    }

}

void MainWindow::connectToServer()
{
    //conncting to Server
    client->connectToHost(QHostAddress::LocalHost , 6446) ;
}

QDataStream &operator>>(QDataStream &in, MainWindow::message &msg)
{
    in >> msg.isID ;
    in >> msg.id ;
    in >> msg.isOk;
    in >> msg.haveVoted ;
    in >> msg.notFind ;
    in >> msg.isStatics ;
    in >> msg.condidateNum ;
    return in;
}

QDataStream &operator<<(QDataStream &out, MainWindow::message &msg)
{
    out << msg.isID ;
    out << msg.id ;
    out << msg.isOk;
    out << msg.haveVoted ;
    out << msg.notFind ;
    out << msg.isStatics ;
    out << msg.condidateNum ;
    return out ;
}

void MainWindow::dataReceived()
{
    auto client = qobject_cast <QTcpSocket *> (sender()) ;
    auto data = client->readAll() ;
    qDebug() << "data recieved ... " ;

    message msg ;
    QDataStream dataIn(&data , QIODevice::ReadOnly);
    dataIn >> msg ;

    if(msg.isOk) {
        qDebug() << "citizen found" ;
        VotingPage *v = new VotingPage();
        v->show();
        this->close();
    } else if(msg.haveVoted) {
        qDebug() << "citizen voted before" ;
        ui->status_label->setStyleSheet("color:red;");
        ui->status_label->setText("citizen voted before");
    } else if (msg.notFind) {
        qDebug() << "citizen not found " ;
        ui->status_label->setStyleSheet("color:red;");
        ui->status_label->setText("citizen not found !");
    }
}

void MainWindow::sendMessage(message _msg) {

    // sending id to check validity in dataBase of server
    QByteArray ba ;
    QDataStream dataOut(&ba , QIODevice::WriteOnly) ;
    dataOut << _msg ;

    client->write(ba) ;

}

void MainWindow::on_id_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.length() == 10) {
        message msg ;
        msg.isID = true ;
        msg.id = arg1 ;
        sendMessage(msg);
        ui->status_label->setStyleSheet("color:yellow ;");
        ui->status_label->setText("checking ID, please wait ...");
    }
}

void MainWindow::candidateChosn(int _condidateNum)
{
    //sending statics
    message msg ;
    msg.isStatics = true ;
    msg.condidateNum = _condidateNum ;

    sendMessage(msg);
}

