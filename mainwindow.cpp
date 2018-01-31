#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(start_dp(int,double,int,int)), ui->display, SLOT(generatePoints(int,double,int,int)));
    connect(this, SIGNAL(start_de(int,double,int)), ui->display, SLOT(drawEdges(int,double,int)));
    connect(this, SIGNAL(start_auto(int,double,int,int)), ui->display, SLOT(automatic_p1(int,double,int,int)));
    connect(this, SIGNAL(start_dmm(double)), ui->display, SLOT(maxAndMin(double)));
    connect(this, SIGNAL(clear()), ui->display, SLOT(clear()));

    connect(ui->display, SIGNAL(send_result(Result)), this, SLOT(receive_result(Result)));
    connect(ui->display, SIGNAL(send_result_p2(Result_p2)), this, SLOT(receive_result_p2(Result_p2)));

    connect(this, SIGNAL(start_auto_p2(int,int)), ui->display, SLOT(automatic_p2(int,int)));

    connect(this, SIGNAL(start_gen_backbone(int)), ui->display, SLOT(generateBackbone(int)));
    connect(this, SIGNAL(start_draw_backbone(int,int)), ui->display, SLOT(drawBackbone(int,int)));
    connect(ui->display, SIGNAL(send_result_p3(Result_p3)),this, SLOT(receive_result_p3(Result_p3)));

    ui->draw_max_min->setDisabled(1);
    ui->draw_edges->setDisabled(1);
    ui->automatic_p2->setDisabled(1);
    ui->generate_backbone->setDisabled(1);
    ui->draw_backbone_1->setDisabled(1);
    ui->draw_backbone_2->setDisabled(1);

    ui->results->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->results->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->results->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->results_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->results_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->results_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->results_2->resizeColumnsToContents();
    ui->results_3->resizeColumnsToContents();

    realtime_p1 = 0;
    realtime_p2 = 0;
    realtime_p3 = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    switch(value)
    {
        case 0:
        ui->number->setText("20");
        break;

        case 1:
        ui->number->setText("100");
        break;

        case 2:
        ui->number->setText("4000");
        break;

        case 3:
        ui->number->setText("8000");
        break;

        case 4:
        ui->number->setText("16000");
        break;

        case 5:
        ui->number->setText("32000");
        break;

        case 6:
        ui->number->setText("64000");
        break;

        case 7:
        ui->number->setText("128000");
        break;
    }
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    switch(value)
    {
        case 0:
        ui->avg_degree->setText("8");
        break;

        case 1:
        ui->avg_degree->setText("16");
        break;

        case 2:
        ui->avg_degree->setText("32");
        break;

        case 3:
        ui->avg_degree->setText("64");
        break;

        case 4:
        ui->avg_degree->setText("128");
        break;
    }
}

void MainWindow::on_draw_points_clicked()
{
    if(ui->square->isChecked())
    {
        type = 1;
        N = ui->number->text().toInt();
        int avg_degree_i = ui->avg_degree->text().toInt();
        R = sqrt(avg_degree_i/(N * MyPI)) * 800;
    }
    else if(ui->unit_disk->isChecked())
    {
        N = ui->number->text().toInt();
        int avg_degree_i = ui->avg_degree->text().toInt();
        R = sqrt(avg_degree_i/(N * 4.0)) * 800;
        type = 2;
    }
    if(ui->real_time_display_p1->isChecked())
    {
        realtime_p1 = 1;
    }
    else
    {
        realtime_p1 = 0;
    }
    emit start_dp(N, R, type, realtime_p1);
    ui->draw_edges->setEnabled(1);
    ui->generate_backbone->setDisabled(1);
    ui->draw_backbone_1->setDisabled(1);
    ui->draw_backbone_2->setDisabled(1);
    ui->draw_max_min->setDisabled(1);
}

void MainWindow::on_draw_edges_clicked()
{
    if(ui->real_time_display_p1->isChecked())
    {
        realtime_p1 = 1;
    }
    else
    {
        realtime_p1 = 0;
    }
    emit start_de(N, R, realtime_p1);
    ui->draw_edges->setDisabled(1);
    ui->automatic_p2->setEnabled(1);
    ui->draw_max_min->setEnabled(1);
}

void MainWindow::on_automatic_p1_clicked()
{
    if(ui->square->isChecked())
    {
        type = 1;
        N = ui->number->text().toInt();
        int avg_degree_i = ui->avg_degree->text().toInt();
        R = sqrt(avg_degree_i/(N * MyPI)) * 800;
    }
    else if(ui->unit_disk->isChecked())
    {
        type = 2;
        N = ui->number->text().toInt();
        int avg_degree_i = ui->avg_degree->text().toInt();
        R = sqrt(avg_degree_i/(N * 4.0)) * 800;
    }
    if(ui->real_time_display_p1->isChecked())
    {
        realtime_p1 = 1;
    }
    else
    {
        realtime_p1 = 0;
    }
    emit start_auto(N, R, type, realtime_p1);
    ui->automatic_p2->setEnabled(1);
    ui->generate_backbone->setDisabled(1);
    ui->draw_backbone_1->setDisabled(1);
    ui->draw_backbone_2->setDisabled(1);
}

void MainWindow::on_clear_clicked()
{
    emit clear();
    ui->draw_edges->setDisabled(1);
    ui->automatic_p2->setDisabled(1);
    ui->generate_backbone->setDisabled(1);
    ui->draw_backbone_1->setDisabled(1);
    ui->draw_backbone_2->setDisabled(1);
    ui->draw_max_min->setDisabled(1);
}

void MainWindow::receive_result(Result result)
{
    int rowIndex = ui->results->rowCount();
    ui->results->setRowCount(rowIndex + 1);
    ui->results->setItem(rowIndex, 0, new QTableWidgetItem(QString::number(result.ID)));
    ui->results->setItem(rowIndex, 1, new QTableWidgetItem(QString::number(result.N)));
    ui->results->setItem(rowIndex, 2, new QTableWidgetItem(QString::number(result.R/800.0)));
    ui->results->setItem(rowIndex, 3, new QTableWidgetItem(QString::number(result.M)));
    ui->results->setItem(rowIndex, 4, new QTableWidgetItem(QString::number(result.min_degree)));
    ui->results->setItem(rowIndex, 5, new QTableWidgetItem(QString::number(result.avg_degree)));
    ui->results->setItem(rowIndex, 6, new QTableWidgetItem(QString::number(result.max_degree)));
    ui->results->setItem(rowIndex, 7, new QTableWidgetItem(QString::number(result.running_time)));
    if(result.type == 1)
    {
        ui->results->setItem(rowIndex, 8, new QTableWidgetItem("Square"));
    }
    else if (result.type == 2)
    {
        ui->results->setItem(rowIndex, 8, new QTableWidgetItem("Unit Disk"));
    }
    for(int i = 0; i < 9; i++)
    {
        ui->results->item(rowIndex, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
}

void MainWindow::receive_result_p2(Result_p2 result)
{
    int rowIndex = ui->results_2->rowCount();
    ui->results_2->setRowCount(rowIndex + 1);
    ui->results_2->setItem(rowIndex, 0, new QTableWidgetItem(QString::number(result.max_degree_wd)));
    ui->results_2->setItem(rowIndex, 1, new QTableWidgetItem(QString::number(result.number_of_colors)));
    ui->results_2->setItem(rowIndex, 2, new QTableWidgetItem(QString::number(result.max_color_size)));
    ui->results_2->setItem(rowIndex, 3, new QTableWidgetItem(QString::number(result.terminal_clique_size)));
    ui->results_2->setItem(rowIndex, 4, new QTableWidgetItem(QString::number(result.running_time)));
    for(int i = 0; i < 5; i++)
    {
        ui->results_2->item(rowIndex, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
}

void MainWindow::on_automatic_p2_clicked()
{
    if(ui->real_time_display_p2->isChecked())
    {
        realtime_p2 = 1;
    }
    else
    {
        realtime_p2 = 0;
    }
    emit start_auto_p2(N, realtime_p2);
    ui->generate_backbone->setEnabled(1);
}

void MainWindow::on_draw_backbone_1_clicked()
{
    if(ui->real_time_display_p3->isChecked())
    {
        realtime_p3 = 1;
    }
    else
    {
        realtime_p3 = 0;
    }
    emit start_draw_backbone(1, realtime_p3);
}

void MainWindow::on_draw_backbone_2_clicked()
{
    if(ui->real_time_display_p3->isChecked())
    {
        realtime_p3 = 1;
    }
    else
    {
        realtime_p3 = 0;
    }
    emit start_draw_backbone(2, realtime_p3);
}

void MainWindow::on_generate_backbone_clicked()
{
    if(ui->real_time_display_p3->isChecked())
    {
        realtime_p3 = 1;
    }
    else
    {
        realtime_p3 = 0;
    }
    emit start_gen_backbone(N);
    ui->draw_backbone_1->setEnabled(1);
    ui->draw_backbone_2->setEnabled(1);
}

void MainWindow::receive_result_p3(Result_p3 result)
{
    int rowIndex = ui->results_3->rowCount();
    ui->results_3->setRowCount(rowIndex + 1);
    ui->results_3->setItem(rowIndex, 0, new QTableWidgetItem(QString::number(result.ID)));
    ui->results_3->setItem(rowIndex, 1, new QTableWidgetItem("Max"));
    ui->results_3->setItem(rowIndex, 2, new QTableWidgetItem(QString::number(result.vertices1)));
    ui->results_3->setItem(rowIndex, 3, new QTableWidgetItem(QString::number(result.edges1)));
    ui->results_3->setItem(rowIndex, 4, new QTableWidgetItem(QString::number(result.percentage1)));
    ui->results_3->setItem(rowIndex, 5, new QTableWidgetItem(QString::number(result.running_time)));
    for(int i = 0; i < 6; i++)
    {
        ui->results_3->item(rowIndex, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }

    rowIndex = ui->results_3->rowCount();
    ui->results_3->setRowCount(rowIndex + 1);
    ui->results_3->setItem(rowIndex, 0, new QTableWidgetItem(""));
    ui->results_3->setItem(rowIndex, 1, new QTableWidgetItem("Second Max"));
    ui->results_3->setItem(rowIndex, 2, new QTableWidgetItem(QString::number(result.vertices2)));
    ui->results_3->setItem(rowIndex, 3, new QTableWidgetItem(QString::number(result.edges2)));
    ui->results_3->setItem(rowIndex, 4, new QTableWidgetItem(QString::number(result.percentage2)));
    ui->results_3->setItem(rowIndex, 5, new QTableWidgetItem(""));
    for(int i = 0; i < 6; i++)
    {
        ui->results_3->item(rowIndex, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
}

void MainWindow::on_draw_max_min_clicked()
{
    emit start_dmm(R);
    ui->draw_max_min->setDisabled(1);
}
