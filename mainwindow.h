#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MyPI 3.1415926

#include <QMainWindow>
#include "paintwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_draw_points_clicked();

    void on_draw_edges_clicked();

    void on_automatic_p1_clicked();

    void on_clear_clicked();

    void on_automatic_p2_clicked();

    void on_draw_backbone_1_clicked();

    void on_draw_backbone_2_clicked();

    void on_generate_backbone_clicked();

    void on_draw_max_min_clicked();

public slots:
    void receive_result(Result result);
    void receive_result_p2(Result_p2 result);
    void receive_result_p3(Result_p3 result);

signals:
    void start_dp(int N, double R, int type, int realtime);
    void start_de(int N, double R, int realtime);
    void start_dmm(double R);
    void start_auto(int N, double R, int type, int realtime);
    void start_auto_p2(int N, int realtime);
    void start_gen_backbone(int N);
    void start_draw_backbone(int backbone_id, int realtime);
    void clear();

private:
    Ui::MainWindow *ui;
    int N;
    double R;
    int type;
    int realtime_p1;
    int realtime_p2;
    int realtime_p3;
};

#endif // MAINWINDOW_H
