#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QSqlQuery>
#include <QSqlRecord>

#include "dataBase.h"


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

private:
    //variables
    Ui::MainWindow *ui;
    QTcpServer *server ;
    QHostAddress serverIP = QHostAddress::LocalHost ;
    ushort port = 6446 ;
    int condidate_1_vote = 0 ;
    int condidate_2_vote = 0 ;
    int condidate_3_vote = 0 ;
    int condidate_4_vote = 0 ;

    //func
    void setUpServer() ;
    void newClientConnectionReceived() ;
    void clientDisconnected() ;
    void sendMessage(QTcpSocket *_client ,message _msg) ;
    void dataReceived() ;
    void updateChart(int _condidateID) ;
    void checkValidity(QTcpSocket *_client, QString _id) ;

    void anlizePacket(message _msg) ;
};
#endif // MAINWINDOW_H
