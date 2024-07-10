#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QByteArray>

#include "VotingPage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    struct message {
        bool isID = false;
        QString id ;
        bool isOk = false;
        bool haveVoted = false;
        bool notFind = false;
        bool isStatics = false ;
        int condidateNum = false;
    };
    ~MainWindow();

private slots :
    void connectedToServer() ;

    void on_id_lineEdit_textChanged(const QString &arg1);

public slots:
    void candidateChosn(int _condidateNum) ;

private:
    //variables
    Ui::MainWindow *ui;
    QTcpSocket *client ;

    //functions
    void connectToServer() ;
    void dataReceived() ;
    void sendMessage(message _msg) ;


};
#endif // MAINWINDOW_H
