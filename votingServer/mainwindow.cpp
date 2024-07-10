#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //run server
    setUpServer();
    updateChart(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkValidity(QTcpSocket *_client, QString _id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM citizen WHERE id = :id");
    query.bindValue(":id", _id);

    if (query.exec() && query.next()) {
        QSqlRecord record = query.record();
        if (record.value("voted").toInt() == 1) {
            message msg ;
            msg.haveVoted = true ;
            sendMessage(_client , msg);
            qDebug() << "status : citizen voted " ;

        } else if (record.value("voted").toInt() == 0){
            message msg;
            msg.isOk = true ;
            sendMessage(_client , msg);
            qDebug() << "status : citizen didn't voted " ;
            query.prepare("UPDATE citizen SET voted = 1 WHERE id = :id ");
            query.bindValue(":id" , _id);
            query.exec() ;

        }
    } else {
        message msg ;
        msg.notFind = true ;
        sendMessage(_client , msg);


        if (!query.isValid()) {
            qDebug() << "Query error: " << query.lastError().text();
        }
    }
}

void MainWindow::anlizePacket(message _msg)
{
    qDebug() << QString("is the packet id ? %1").arg(_msg.isID);
    qDebug() << QString("is the packet statics ? %1").arg(_msg.isStatics);
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

void MainWindow::setUpServer()
{
    server = new QTcpServer(this) ;

    connect(server , &QTcpServer::newConnection , this , &MainWindow::newClientConnectionReceived) ;

    //listen to new connection
    server->listen(QHostAddress::Any , port) ;

    qDebug() << "Setting up server " ;
}

void MainWindow::newClientConnectionReceived()
{
    qDebug() << "new connection recieved " ;
    auto client = server->nextPendingConnection();
    // clients << client ;
    // auto id = clients.length() ;
    // client->setProperty("id" , id) ;

    qDebug() << QString("client 1 connected .. ") ;

    connect(client , &QTcpSocket::readyRead , this , &MainWindow::dataReceived );

}

void MainWindow::clientDisconnected()
{
    auto client = qobject_cast< QTcpSocket * > (sender()) ;

    int id = client->property("id").toInt() ;

    qDebug() << QString(" voting System %1 dissconnected !" ).arg(id) ;
}

void MainWindow::dataReceived()
{
    auto client = qobject_cast <QTcpSocket *> (sender()) ;
    auto data = client->readAll() ;
    qDebug() << "data recieved ... " ;

    message msg ;
    QDataStream dataIn(&data , QIODevice::ReadOnly);
    dataIn >> msg ;

    anlizePacket(msg) ;

    if(msg.isID) {
        checkValidity(client , msg.id);
        qDebug() << QString("recived id : %1").arg(msg.id) ;
    } else if(msg.isStatics) {
        updateChart(msg.condidateNum) ;
        qDebug() << QString("recived condidate id : %1").arg(msg.condidateNum) ;
    }
}

void MainWindow::updateChart(int _condidateID)
{
    qDebug() << QString("updating chart ..");

    QBarSet *set0 = new QBarSet("votes");
    //clear chartItems and widget
    QLayoutItem* chartLayoutItem = ui->chart_gridLayout->takeAt(0);
    if (chartLayoutItem) {
        delete chartLayoutItem->widget();
        delete chartLayoutItem;
    }

    switch (_condidateID) {
    case 55:
        condidate_1_vote ++ ;
        break;
    case 66:
        condidate_2_vote ++ ;
        break;
    // case 3:
    //     condidate_3_vote ++ ;
    //     break;
    // case 4:
    //     condidate_4_vote ++ ;
    //     break;
    default:
        break;
    }

    set0->append(condidate_1_vote);
    set0->append(condidate_2_vote);
    set0->append(condidate_3_vote);
    set0->append(condidate_4_vote);

    // make series of dataSets
    QBarSeries *series = new QBarSeries();
    series->append(set0);

    // make chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    // chart->setTitle("votes");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // categories of chart
    QStringList categories;
    categories << "condidate 1" << "condidate 2" << "condidate 3" << "condidate 4" ;

    // configure axisX of chart
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // configure axisY of chart
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,36);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // add chart to chartView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->chart_gridLayout->addWidget(chartView) ;

    this->setLayout(ui->chart_gridLayout);
}

void MainWindow::sendMessage(QTcpSocket *_client , message _msg) {

    // sending id to check validity in dataBase of server
    QByteArray ba ;
    QDataStream dataOut(&ba , QIODevice::WriteOnly) ;
    dataOut << _msg ;

    _client->write(ba) ;

}
