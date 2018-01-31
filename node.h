#ifndef NODE_H
#define NODE_H

#include <QPainter>

struct DegreeNode;

struct Node
{
    Node()
    {
        dptr = NULL;
        color = -1;
    }
    QPointF point;
    int degree;
    int belongTo;
    int index;
    int color;
    QVector<int> neighbor;
    DegreeNode* dptr;
    int bi_index[6];
};

struct Result
{
    int ID;
    int N;
    double R;
    int M;
    int min_degree;
    double avg_degree;
    int max_degree;
    double running_time;
    int type;
};

struct Result_p2
{
    int max_degree_wd;
    int number_of_colors;
    int max_color_size;
    int terminal_clique_size;
    double running_time;
};

struct Result_p3
{
    int ID;
    int vertices1;
    int vertices2;
    int edges1;
    int edges2;
    double percentage1;
    double percentage2;
    double running_time;
};

struct DegreeNode
{
    DegreeNode()
    {
        pre = NULL;
        next = NULL;
        cur_degree = 0;
    }
    Node node;
    int cur_degree;
    DegreeNode* pre;
    DegreeNode* next;
};

struct DegreeLinklist
{
    DegreeLinklist()
    {
        head = NULL;
        rear = NULL;
        length = 0;
    }
    DegreeNode* head;
    DegreeNode* rear;
    int length;
};

#endif // NODE_H
