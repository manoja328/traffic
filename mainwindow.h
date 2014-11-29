#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPixmap colorSwatch( const QColor color );
    bool eventFilter(QObject *obj, QEvent *event);


private slots:
    void on_tableWidget_cellClicked(int row, int column);
    void on_play_clicked();
    void on_spinBox_valueChanged(int );
    void showdata();
    void on_Send_data_triggered();
    void selectedFromCOMList();
    void AddCOMListtoActions();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_write_cell_clicked();
    void on_actionClear_Current_triggered();
};

#endif // MAINWINDOW_H
