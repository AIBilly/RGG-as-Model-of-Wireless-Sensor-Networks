#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include "node.h"
#include <QPixmap>
#include <QTime>
#include <QVector>
#include <math.h>
#include <QDebug>
#include <random>
#include <QFile>
#include <QTextStream>
#include <QQueue>

namespace Ui {
class PaintWidget;
}

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = 0);
    ~PaintWidget();

private:
    QPixmap mypixmap;
    Node* nodes;
    QVector<QVector<Node>> cellNodes;
    int cellNo;
    int edges;
    int total_degree;
    double avg_degree;
    Node max_degree, min_degree;
    int* degree_distribution;
    QVector<Result> results;
    int count;

    DegreeLinklist* degree_linklist;
    QVector<int> SLO;
    QString* color_list;
    int colorcnt;
    int max_color_size;
    int terminal_clique_size;
    int* average_degree_wd;
    int* degree_wd;
    int* color_distribution;
    QVector<Result_p2> results_p2;

    QVector<Node> bipartite01;
    QVector<Node> bipartite02;
    QVector<Node> bipartite03;
    QVector<Node> bipartite12;
    QVector<Node> bipartite13;
    QVector<Node> bipartite23;

    QVector<Node> backbone1;
    QVector<Node> backbone2;

    QVector<QVector<Node>> backbone_array;
    QVector<QVector<Node>> bipartite_array;

    int max, second_max;
    double RR;

    QVector<Result_p3> results_p3;

    Ui::PaintWidget *ui;
    void buildCell(double R);
    void calDegDistri(int N);

    void clear_p2(int N);
    void genDisList(int N);
    void genSLO(int N);
    void coloring(int realtime);
    void calColorDistri(int N);
    void output_to_file(int N);
    void print_step();
    void print_original(int N);
    QVector<Node> get_backbone(QVector<Node> bipartite);
    void get_bipartite(int N);

signals:
    void send_result(Result result);
    void send_result_p2(Result_p2 result);
    void send_result_p3(Result_p3 result);

public slots:
    void generatePoints(int N, double R, int type, int realtime);
    void drawEdges(int N, double R, int realtime);
    void maxAndMin(double R);
    void automatic_p1(int N, double R, int type, int realtime);
    void automatic_p2(int N, int realtime);
    void clear(void);
    void generateBackbone(int N);
    void drawBackbone(int backbone_id, int realtime);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // PAINTWIDGET_H
