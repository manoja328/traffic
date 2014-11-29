#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStringList>
#include <QTime>
#include <QFile>
#include <QFileDialog>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

QStringList mylist;
QStringList mylist_colval;
QList<QLabel *> arr;
QTime time;
QByteArray requestData;
QString sel_port;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AddCOMListtoActions();
    arr = findChildren<QLabel *>();
    foreach(QLabel *lb, arr){
        if(lb != NULL){
            mylist.append(lb->objectName());
            mylist_colval.append("X");
            lb->installEventFilter(this);
            lb->setPixmap(colorSwatch(QColor(0,0,0)));
        }

    }

    on_spinBox_valueChanged(1);


}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddCOMListtoActions()
{

    QActionGroup *anActionGroup = new QActionGroup(this);
    QList<QAction*> menuAction;
    int COM_index=0;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        //        QString s = QObject::tr("Port: ") + info.portName() + "\n"
        //                + QObject::tr("Location: ") + info.systemLocation() + "\n"
        //                + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";


        //QAction *act=new QAction(this);
        QAction *act=new QAction(info.portName(),anActionGroup);
        //act->setText(info.portName());
        act->setData(info.portName());
        act->setCheckable(true);
        act->setChecked(false);
        menuAction.append(act);
        ui->menuConnect_COM_port->addAction(menuAction.at(COM_index));
        connect(menuAction.at(COM_index),SIGNAL(triggered()),this,SLOT(selectedFromCOMList()));
        COM_index++;
    }

}


void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}



QPixmap MainWindow::colorSwatch( const QColor color )
{
    QPixmap pixmap(20, 20 );
    pixmap.fill(QColor(0,170,0));
    QPainter painter;
    painter.begin( &pixmap );
    //painter.setPen( Qt::NoPen);
    painter.setBrush( color );
    painter.drawEllipse( 0, 0,15,15 );
    painter.end();
    return pixmap;
}



bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if( event->type() == QEvent::MouseButtonPress) {
        QLabel* widget = qobject_cast<QLabel*>(obj);
        //        qDebug()<<widget->objectName();
        //        qDebug()<<obj;
        //        qDebug()<<mylist.indexOf(widget->objectName());
        //        qDebug()<< mylist_colval[mylist.indexOf(widget->objectName())];
        QString a= mylist_colval[mylist.indexOf(widget->objectName())];

        if (a=="X"){
            widget->setPixmap(colorSwatch(Qt::red));
            mylist_colval[mylist.indexOf(widget->objectName())]='R';


        }

        if (a=="R"){
            widget->setPixmap(colorSwatch(Qt::green));
            mylist_colval[mylist.indexOf(widget->objectName())]='G';


        }

        if (a=="G"){
            widget->setPixmap(colorSwatch(QColor(255, 191, 0))); //abmber
            mylist_colval[mylist.indexOf(widget->objectName())]='A';


        }


        if (a=="A"){
            widget->setPixmap(colorSwatch(Qt::black));
            mylist_colval[mylist.indexOf(widget->objectName())]='X';

        }
        return false;
    }


    else {
        return QObject::eventFilter(obj, event);
    }

}








void MainWindow::on_spinBox_valueChanged(int no)
{
    ui->tableWidget->setRowCount(no);
    ui->tableWidget->setColumnCount(2);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

//    for (int i=0;i < ui->tableWidget->rowCount();i++)
//    {

//       QTableWidgetItem *item= new QTableWidgetItem(1);
//        item->setFlags(item->flags() & ~Qt::ItemIsEditable );
//        ui->tableWidget->setItem(i,0,item);




//    }

    ui->tableWidget->setColumnWidth(0,100);
    QStringList TableHeader;
    TableHeader<<"Data"<<"Delay";
    ui->tableWidget->setHorizontalHeaderLabels(TableHeader);

}







void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    //qDebug()<<"clicked";
    //qDebug()<<row<<","<<column;

    if ( ui->tableWidget->item(row,0))
    {

        //qDebug()<<ui->tableWidget->item(row,0);
        mylist_colval=ui->tableWidget->item(row,0)->text().split("");
        mylist_colval.removeFirst();
        mylist_colval.removeLast();
        //qDebug()<<mylist_colval;
        showdata();


    }

    ui->show_rowno->setText(QString::number(row+1));

}

void MainWindow::showdata(){


    foreach(QLabel *lb, arr)
    {
        if(lb != NULL)
        {

            //qDebug()<<mylist_colval[mylist.indexOf(lb->objectName())];
            QString a=mylist_colval[mylist.indexOf(lb->objectName())];

            if (a=="X")
            {
                lb->setPixmap(colorSwatch(QColor(0,0,0)));


            }

            else if (a=="R")
            {
                lb->setPixmap(colorSwatch(Qt::red));


            }

            else if (a=="G")
            {
                lb->setPixmap(colorSwatch(Qt::green));


            }


            else if (a=="A")
            {
                lb->setPixmap(colorSwatch(QColor(255, 191, 0)));

            }
        }

    }



}

void MainWindow::on_write_cell_clicked()
{

    if (ui->tableWidget->currentIndex().row() == -1){

         ui->statusBar->showMessage("Select a Cell from the table above first!!",3000);

    }


    QTableWidgetItem *pCell = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 0);
    if(!pCell)
    {
        pCell = new QTableWidgetItem;
        ui->tableWidget->setItem(ui->tableWidget->currentIndex().row(), 0, pCell);
    }


    pCell->setText(mylist_colval.join(""));




//    QTableWidgetItem *paCell = new QTableWidgetItem;
//    ui->tableWidget->setItem(ui->tableWidget->currentIndex().row(), 1, paCell);
//    paCell->setText("1235");

}

void MainWindow::on_play_clicked()
{
    //qDebug()<<mylist_colval.join("");


    //qDebug()<<ui->spinBox->value();


    ui->statusBar->showMessage("Playing scheme .....");

    for (int row=0;row<ui->spinBox->value();row++)
    {
        if (!ui->tableWidget->item(row,0))
            break;
        delay();
        ui->show_rowno->setText(QString::number(row+1));
        ui->tableWidget->selectRow(row);
        mylist_colval=ui->tableWidget->item(row,0)->text().split("");
        mylist_colval.removeFirst();
        mylist_colval.removeLast();
        showdata();

    }

    ui->statusBar->showMessage("");

}



void MainWindow::selectedFromCOMList()
{

    QAction* action = qobject_cast<QAction*>(sender());
    if(action)
    {
        sel_port=action->data().toString();
        ui->statusBar->showMessage(sel_port+" selected",2000);
    }


}

void MainWindow::on_Send_data_triggered()
{

    if (!sel_port.isNull())
    {
        ui->statusBar->showMessage("Sending Data To "+sel_port ,2000);
        QSerialPort serial;
        serial.setPortName(sel_port);
        serial.setBaudRate(9600);
        serial.setDataBits(QSerialPort::Data8);



        for (int row=0;row<ui->spinBox->value();row++)
        {
            if (!ui->tableWidget->item(row,0))
                break;

            if (serial.open(QIODevice::ReadWrite))
            {
                mylist_colval=ui->tableWidget->item(row,0)->text().split("");
                requestData=QString(mylist_colval.join("")+"\n").toLocal8Bit();
                serial.write(requestData);

            }


            else{
                ui->statusBar->showMessage(tr("Can't open %1, error code %2")
                                           .arg(serial.portName()).arg(serial.error()));
                return;
            }

            serial.close();



        }

        ui->statusBar->showMessage("Sending Completed!",2000);

    }

    else{

        ui->statusBar->showMessage("Select a COM port first!!");

    }







}


void MainWindow::on_actionSave_triggered()
{

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Traffic File"), "",
                                                    tr("Traffic file(*.trf);;All Files (*)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream  out(&file);
        for (int row=0;row<ui->spinBox->value();row++)
        {
            if (ui->tableWidget->item(row,0))
            {
                mylist_colval=ui->tableWidget->item(row,0)->text().split("");
                out<<mylist_colval.join("")<<"\n";

            }

        }

        file.close();
    }
}

void MainWindow::on_actionOpen_triggered()
{

    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open Traffic Files"),
                QDir::currentPath(),
                tr("Traffic files (*.trf);;All files (*.*)") );

    if( !filename.isNull() )
    {

        QFile file(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream  in(&file);
        int row=0;
        while ( !in.atEnd() )
        {
            QString line = in.readLine();
            //qDebug()<< line;
            ui->tableWidget->insertRow(row);
            QTableWidgetItem *item1 = new QTableWidgetItem;
            item1->setText(line);
            ui->tableWidget->setItem(row, 0, item1);
            row++;

        }


        ui->spinBox->setValue(row);
        file.close();
    }
}



void MainWindow::on_actionClear_Current_triggered()
{
    mylist_colval=QString("XXXXXXXXXXXXXXXXXXXXXXXX").split("");
    mylist_colval.removeFirst();
    mylist_colval.removeLast();
    showdata();
}
