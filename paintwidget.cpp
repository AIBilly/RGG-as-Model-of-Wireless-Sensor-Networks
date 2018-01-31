#include "paintwidget.h"
#include "ui_paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintWidget)
{
    ui->setupUi(this);

    nodes = NULL;
    mypixmap = QPixmap(800, 800);
    cellNo = 0;
    edges = 0;
    total_degree = 0;
    avg_degree = 0.0;
    degree_distribution = NULL;
    degree_linklist = NULL;
    count = 0;

    mypixmap.fill(Qt::white);

    color_list = new QString[146] {"#FF44AA", "#FF3333", "#FF7744", "#FFAA33", "#FFCC22", "#FFFF33", "#CCFF33",
                                   "#99FF33", "#33FF33", "#33FF33", "#33FF33", "#33FF33", "#33CCFF", "#5599FF",
                                   "#5555FF", "#7744FF", "#9955FF", "#B94FFF", "#E93EFF", "#FF3EFF",
                                   "#FF0088", "#FF0000", "#FF5511", "#FF8800", "#FFBB00", "#FFFF00", "#BBFF00",
                                   "#77FF00", "#00FF00", "#00FF99", "#00FFCC", "#00FFFF", "#00BBFF", "#0066FF",
                                   "#0000FF", "#5500FF", "#7700FF", "#9900FF", "#CC00FF", "#FF00FF",
                                   "#C10066", "#CC0000", "#E63F00", "#EE7700", "#DDAA00", "#EEEE00", "#99DD00",
                                   "#66DD00", "#00DD00", "#00DD77", "#00DDAA", "#00DDDD", "#009FCC", "#0044BB",
                                   "#0000CC", "#4400CC", "#5500DD", "#7700BB", "#A500CC", "#CC00CC",
                                   "#FF88C2", "#FF8888", "#FFA488", "#FFBB66", "#FFDD55", "#FFFF77", "#DDFF77",
                                   "#BBFF66", "#66FF66", "#77FFCC", "#77FFEE", "#66FFFF", "#77DDFF", "#99BBFF",
                                   "#9999FF", "#9F88FF", "#B088FF", "#D28EFF", "#E38EFF", "#FF77FF",
                                   "#A20055", "#AA0000", "#C63300", "#CC6600", "#AA7700", "#BBBB00", "#88AA00",
                                   "#55AA00", "#00AA00", "#00AA55", "#00AA88", "#00AAAA", "#0088A8", "#003C9D",
                                   "#0000AA", "#2200AA", "#4400B3", "#66009D", "#7A0099", "#990099",
                                   "#FFB7DD", "#FFCCCC", "#FFC8B4", "#FFDDAA", "#FFEE99", "#FFFFBB", "#EEFFBB",
                                   "#CCFF99", "#99FF99", "#BBFFEE", "#AAFFEE", "#99FFFF", "#CCEEFF", "#CCDDFF",
                                   "#CCCCFF", "#CCBBFF", "#D1BBFF", "#E8CCFF", "#F0BBFF", "#FFB3FF",
                                   "#8C0044", "#880000", "#A42D00", "#BB5500", "#886600", "#888800", "#668800",
                                   "#227700", "#008800", "#008844", "#008866", "#008888", "#007799", "#003377",
                                   "#000088", "#220088", "#3A0088", "#550088", "#660077", "#770077",
                                   "#DDDDDD", "#AAAAAA", "#888888", "#666666", "#444444", "#000000"};
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), mypixmap);
}

PaintWidget::~PaintWidget()
{
    delete ui;
}

void PaintWidget::generatePoints(int N, double R, int type, int realtime)
{
    clear();
    buildCell(R);
    nodes = new Node[N];
    QPainter painter(&mypixmap);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);

    RR = R;

    painter.setPen(pen);
    double i, j, x, y;

    std::uniform_real_distribution<double> distribution(0.0,1.0);
    std::default_random_engine generator(time(NULL));

    int e = 0;

    if(type == 1)
    {
        while(e < N)
        {
            i = distribution(generator);
            j = distribution(generator);

            x = i * 800;
            y = j * 800;

            QPointF pointf;
            pointf.setX(x);
            pointf.setY(y);
            painter.drawPoint(pointf);
            nodes[e].degree = 0;
            nodes[e].point = pointf;
            nodes[e].index = e;
            int column = x/R;
            int row = y/R;
            nodes[e].belongTo = row * cellNo + column;
            cellNodes[nodes[e].belongTo].push_back(nodes[e]);
            e++;
            if(realtime == 1)
                repaint();
        }
    }
    else if(type == 2)
    {
        while(e < N)
        {
            i = distribution(generator);
            j = distribution(generator);

            x = i * 800;
            y = j * 800;

            if(sqrt((x-400) * (x-400) + (y-400) * (y-400)) < 400)
            {
                QPointF pointf;
                pointf.setX(x);
                pointf.setY(y);
                painter.drawPoint(pointf);
                nodes[e].degree = 0;
                nodes[e].point = pointf;
                nodes[e].index = e;
                int column = x/R;
                int row = y/R;
                nodes[e].belongTo = row * cellNo + column;
                cellNodes[nodes[e].belongTo].push_back(nodes[e]);
                e++;
                if(realtime == 1)
                    repaint();
            }
        }
    }

    update();
}

void PaintWidget::buildCell(double R)
{
    cellNo = 800/R + 1;
    for (int i = 0; i < cellNo * cellNo; i++)
    {
        QVector<Node> temp;
        cellNodes.push_back(temp);
    }

}

void PaintWidget::drawEdges(int N, double R, int realtime)
{
    QPainter painter(&mypixmap);
    QPen pen;
    pen.setColor(Qt::black);
    painter.setPen(pen);

    int max = 0;
    int min = 999999999;

    for(int e = 0; e < N; e++)
    {
        int belongTo = nodes[e].belongTo;
        int column = belongTo % cellNo;
        int row = belongTo / cellNo;
        for(int i = 0; i < cellNodes[belongTo].length(); i++)
        {
            double x1 = nodes[e].point.x();
            double y1 = nodes[e].point.y();
            double x2 = cellNodes[belongTo][i].point.x();
            double y2 = cellNodes[belongTo][i].point.y();
            if(sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2)) < R)
            {
                int target_index = cellNodes[belongTo][i].index;
                if(e != target_index)
                {
                    painter.drawLine(nodes[e].point, cellNodes[belongTo][i].point);
                    nodes[e].degree++;
                    nodes[e].neighbor.push_back(cellNodes[belongTo][i].index);
                    total_degree++;
                    if(realtime == 1)
                        repaint();
                }
            }
        }
        if(row + 1 < cellNo)//down
        {
            for(int i = 0; i < cellNodes[belongTo + cellNo].length(); i++)
            {
                double x1 = nodes[e].point.x();
                double y1 = nodes[e].point.y();
                double x2 = cellNodes[belongTo + cellNo][i].point.x();
                double y2 = cellNodes[belongTo + cellNo][i].point.y();
                if(sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2)) < R)
                {
                    painter.drawLine(nodes[e].point, cellNodes[belongTo + cellNo][i].point);
                    nodes[e].degree++;
                    //cellNodes[belongTo + cellNo][i].degree++;
                    int target_index = cellNodes[belongTo + cellNo][i].index;
                    nodes[target_index].degree++;
                    nodes[e].neighbor.push_back(target_index);
                    nodes[target_index].neighbor.push_back(e);
                    total_degree+=2;
                    if(realtime == 1)
                        repaint();
                }
            }
        }
        if(column + 1 < cellNo)//right
        {
            for(int i = 0; i < cellNodes[belongTo + 1].length(); i++)
            {
                double x1 = nodes[e].point.x();
                double y1 = nodes[e].point.y();
                double x2 = cellNodes[belongTo + 1][i].point.x();
                double y2 = cellNodes[belongTo + 1][i].point.y();
                if(sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2)) < R)
                {
                    painter.drawLine(nodes[e].point, cellNodes[belongTo + 1][i].point);
                    nodes[e].degree++;
                    //cellNodes[belongTo + 1][i].degree++;
                    int target_index = cellNodes[belongTo + 1][i].index;
                    nodes[target_index].degree++;
                    nodes[e].neighbor.push_back(target_index);
                    nodes[target_index].neighbor.push_back(e);
                    total_degree+=2;
                    if(realtime == 1)
                        repaint();
                }
            }
        }
        if(column + 1 < cellNo && row + 1 < cellNo)//right down
        {
            for(int i = 0; i < cellNodes[belongTo + cellNo + 1].length(); i++)
            {
                double x1 = nodes[e].point.x();
                double y1 = nodes[e].point.y();
                double x2 = cellNodes[belongTo + cellNo + 1][i].point.x();
                double y2 = cellNodes[belongTo + cellNo + 1][i].point.y();
                if(sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2)) < R)
                {
                    painter.drawLine(nodes[e].point, cellNodes[belongTo + cellNo + 1][i].point);
                    nodes[e].degree++;
                    //cellNodes[belongTo + cellNo + 1][i].degree++;
                    int target_index = cellNodes[belongTo + cellNo + 1][i].index;
                    nodes[target_index].degree++;
                    nodes[e].neighbor.push_back(target_index);
                    nodes[target_index].neighbor.push_back(e);
                    total_degree+=2;
                    if(realtime == 1)
                        repaint();
                }
            }
        }
        if(column + 1 < cellNo && row - 1 >= 0)//right up
        {
            for(int i = 0; i < cellNodes[belongTo - cellNo + 1].length(); i++)
            {
                double x1 = nodes[e].point.x();
                double y1 = nodes[e].point.y();
                double x2 = cellNodes[belongTo - cellNo + 1][i].point.x();
                double y2 = cellNodes[belongTo - cellNo + 1][i].point.y();
                if(sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2)) < R)
                {
                    painter.drawLine(nodes[e].point, cellNodes[belongTo - cellNo + 1][i].point);
                    nodes[e].degree++;
                    //cellNodes[belongTo - cellNo + 1][i].degree++;
                    int target_index = cellNodes[belongTo - cellNo + 1][i].index;
                    nodes[target_index].degree++;
                    nodes[e].neighbor.push_back(target_index);
                    nodes[target_index].neighbor.push_back(e);
                    total_degree+=2;
                    if(realtime == 1)
                        repaint();
                }
            }
        }

        /*qDebug() << nodes[e].degree << nodes[e].neighbor.length();
        for(int i = 0; i < nodes[e].neighbor.length(); i++)
        {
            qDebug() << nodes[e].neighbor[i];
        }*/
        //qDebug() << "\n";
    }

    for(int i = 0; i < N; i++)
    {
        if(nodes[i].degree > max)
        {
            max_degree = nodes[i];
            max = nodes[i].degree;
        }
        if(nodes[i].degree < min)
        {
            min_degree = nodes[i];
            min = nodes[i].degree;
        }
    }

    edges = total_degree/2;
    avg_degree = (double)total_degree / N;

    //qDebug() << "\n";
    //qDebug() << max << min;
    //qDebug() << max_degree.degree << min_degree.degree;

    update();
}

void PaintWidget::maxAndMin(double R)
{
    QPainter painter(&mypixmap);
    QPen pen;

    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(255, 0, 0, 50)));
    painter.drawPoint(max_degree.point);
    painter.drawEllipse(max_degree.point, R, R);

    pen.setColor(Qt::blue);
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(0, 0, 255, 50)));
    painter.drawPoint(min_degree.point);
    painter.drawEllipse(min_degree.point, R, R);

    update();
}

void PaintWidget::automatic_p1(int N, double R, int type, int realtime)
{
    QTime time;
    time.start();
    generatePoints(N, R, type, realtime);
    drawEdges(N, R, realtime);
    maxAndMin(R);
    int timeElapsed = time.elapsed();
    double runtime = timeElapsed/1000.0;
    calDegDistri(N);

    count++;
    Result result;
    result.ID = count;
    result.N = N;
    if(type == 1)
    {
        result.R = R;
    }
    else
    {
        result.R = R * 2.0;
    }
    result.M = edges;
    result.min_degree = min_degree.degree;
    result.avg_degree = avg_degree;
    result.max_degree = max_degree.degree;
    result.running_time = runtime;
    result.type = type;
    results.push_back(result);
    emit send_result(result);
}

void PaintWidget::clear(void)
{
    mypixmap.fill(Qt::white);
    update();
    if(cellNodes.length() != 0)
    {
        for (int i = 0; i < cellNodes.length(); i++)
        {
            cellNodes[i].clear();
        }
        cellNodes.clear();
    }
    cellNo = 0;
    edges = 0;
    total_degree = 0;
    avg_degree = 0;
    if(nodes != NULL)
    {
        delete [] nodes;
        nodes = NULL;
    }
    if(degree_distribution != NULL)
    {
        delete degree_distribution;
        degree_distribution = NULL;
    }
    if(degree_linklist != NULL)
    {
        delete [] degree_linklist;
        degree_linklist = NULL;
    }
    if(SLO.length() != 0)
    {
        SLO.clear();
    }
}

void PaintWidget::calDegDistri(int N)
{
    int n = max_degree.degree;
    degree_distribution = new int[n + 1]();
    for(int e = 0; e < N; e++)
    {
        degree_distribution[nodes[e].degree]++;
    }
}

void PaintWidget::clear_p2(int N)
{
    mypixmap.fill(Qt::white);
    update();
    if(degree_linklist != NULL)
    {
        delete [] degree_linklist;
        degree_linklist = NULL;
    }
    if(SLO.length() != 0)
    {
        SLO.clear();
    }
    for(int e = 0; e < N; e++)
    {
        nodes[e].dptr = NULL;
        nodes[e].color = -1;
    }
}

void PaintWidget::automatic_p2(int N, int realtime)
{
    clear_p2(N);
    QTime time;
    time.start();
    genDisList(N);

    //print_original(N);

    genSLO(N);
    coloring(realtime);
    int timeElapsed = time.elapsed();
    double runtime = timeElapsed/1000.0;
    //qDebug() << runtime;
    calColorDistri(N);
    int max_when_deleted = 0;
    for(int e = 0; e < N; e++)
    {
        if(degree_wd[e] > max_when_deleted)
            max_when_deleted = degree_wd[e];
    }

    output_to_file(N);

    Result_p2 result;
    result.max_degree_wd = max_when_deleted;
    result.number_of_colors = colorcnt;
    result.max_color_size = max_color_size;
    result.terminal_clique_size = terminal_clique_size;
    result.running_time = runtime;
    results_p2.push_back(result);
    emit send_result_p2(result);
}

void PaintWidget::genDisList(int N)
{
    int max = max_degree.degree;
    degree_linklist = new DegreeLinklist[max + 1];
    for(int e = 0; e < N; e++)
    {
        int degree = nodes[e].degree;
        if(degree_linklist[degree].length == 0)
        {
            DegreeNode* new_node = new DegreeNode;
            new_node->node = nodes[e];
            new_node->pre = NULL;
            new_node->next = NULL;
            new_node->cur_degree = degree;
            degree_linklist[degree].head = new_node;
            degree_linklist[degree].rear = new_node;
            degree_linklist[degree].length++;

            nodes[e].dptr = new_node;
        }
        else
        {
            DegreeNode* new_node = new DegreeNode;
            new_node->node = nodes[e];
            new_node->pre = degree_linklist[degree].rear;
            new_node->next = NULL;
            new_node->cur_degree = degree;
            degree_linklist[degree].rear->next = new_node;
            degree_linklist[degree].rear = new_node;
            degree_linklist[degree].length++;

            nodes[e].dptr = new_node;
        }
    }
}

void PaintWidget::genSLO(int N)
{    
    int search_index = 0;
    int count = N;
    int max = max_degree.degree;

    degree_wd = new int[N];
    average_degree_wd = new int[N];

    int find_flag = 0;

    int cur_total_degree = total_degree;

    while(count > 0)
    {
        int count_delist = 0;
        int record = -1;
        if(find_flag == 0)
        {
            for(int k = 0; k < max + 1; k++)
            {
                if(degree_linklist[k].length != 0)
                {
                    count_delist++;
                    record = k;
                }
            }
            if(count_delist == 1)
            {
                terminal_clique_size = degree_linklist[record].length;
                find_flag = 1;
            }
        }

        for(int i = search_index; i < max + 1; i++)
        {
            if(degree_linklist[i].length == 0)
                continue;

            int tget_index = degree_linklist[i].head->node.index;
            SLO.push_back(tget_index);

            degree_wd[tget_index] = degree_linklist[i].head->cur_degree;
            cur_total_degree = cur_total_degree - 2 * degree_linklist[i].head->cur_degree;
            average_degree_wd[tget_index] = cur_total_degree/count;

            //delete original point
            if(degree_linklist[i].head == degree_linklist[i].rear)
            {
                delete(degree_linklist[i].head);
                degree_linklist[i].head = NULL;
                degree_linklist[i].rear = NULL;
            }
            else
            {
                degree_linklist[i].head = degree_linklist[i].head->next;
                delete(degree_linklist[i].head->pre);
                degree_linklist[i].head->pre = NULL;
            }
            degree_linklist[i].length--;
            nodes[tget_index].dptr = NULL;

            //move adj list degree
            for(int j = 0; j < nodes[tget_index].neighbor.length(); j++)
            {
                DegreeNode* ptr = nodes[nodes[tget_index].neighbor[j]].dptr;
                if(ptr == NULL)
                    continue;

                int cur_degree = ptr->cur_degree;

                //qDebug() << ptr->cur_degree << degree_linklist[cur_degree].length;

                if(ptr->pre == NULL && ptr->next == NULL)
                {
                    degree_linklist[cur_degree].head = NULL;
                    degree_linklist[cur_degree].rear = NULL;
                }
                else if(ptr->pre == NULL)
                {
                    degree_linklist[cur_degree].head = ptr->next;
                    nodes[nodes[tget_index].neighbor[j]].dptr->next->pre = NULL;
                    nodes[nodes[tget_index].neighbor[j]].dptr->next = NULL;
                }
                else if(ptr->next == NULL)
                {
                    degree_linklist[cur_degree].rear = ptr->pre;
                    nodes[nodes[tget_index].neighbor[j]].dptr->pre->next = NULL;
                    nodes[nodes[tget_index].neighbor[j]].dptr->pre = NULL;
                }
                else
                {
                    nodes[nodes[tget_index].neighbor[j]].dptr->pre->next = ptr->next;
                    nodes[nodes[tget_index].neighbor[j]].dptr->next->pre = ptr->pre;
                    nodes[nodes[tget_index].neighbor[j]].dptr->next = NULL;
                    nodes[nodes[tget_index].neighbor[j]].dptr->pre = NULL;
                }
                degree_linklist[cur_degree].length--;
                nodes[nodes[tget_index].neighbor[j]].dptr->cur_degree--;
                cur_degree--;
                //add to lower layer
                if(degree_linklist[cur_degree].length == 0)
                {
                    degree_linklist[cur_degree].head = nodes[nodes[tget_index].neighbor[j]].dptr;
                    degree_linklist[cur_degree].rear = nodes[nodes[tget_index].neighbor[j]].dptr;
                }
                else
                {
                    nodes[nodes[tget_index].neighbor[j]].dptr->pre = degree_linklist[cur_degree].rear;
                    degree_linklist[cur_degree].rear->next = nodes[nodes[tget_index].neighbor[j]].dptr;
                    degree_linklist[cur_degree].rear = nodes[nodes[tget_index].neighbor[j]].dptr;
                }
                degree_linklist[cur_degree].length++;
            }

            //print_step();

            search_index = i;

            if(search_index > 0)
                search_index--;
            break;
        }
        count--;
    }
}

void PaintWidget::coloring(int realtime)
{
    QPainter painter(&mypixmap);
    QPen pen;
    pen.setWidth(5);

    colorcnt = 0;

    for(int e = SLO.length() - 1; e >= 0; e--)
    {
        int flags[1000] = {0};
        int all_occipied = 1;
        for(int j = 0; j < nodes[SLO[e]].neighbor.length(); j++)
        {
            if(nodes[nodes[SLO[e]].neighbor[j]].color != -1)
                flags[nodes[nodes[SLO[e]].neighbor[j]].color] = 1;
        }
        for(int j = 0; j < colorcnt; j++)
        {
            if(flags[j] == 0)
            {
                all_occipied = 0;
                nodes[SLO[e]].color = j;
                QColor mycolor = QColor();
                mycolor.setNamedColor(color_list[j]);
                pen.setColor(mycolor);
                painter.setPen(pen);
                painter.drawPoint(nodes[SLO[e]].point);
                if(realtime == 1)
                    repaint();
                break;
            }
        }
        if(all_occipied == 1)
        {
            nodes[SLO[e]].color = colorcnt;
            QColor mycolor = QColor();
            mycolor.setNamedColor(color_list[colorcnt]);
            pen.setColor(mycolor);
            painter.setPen(pen);
            painter.drawPoint(nodes[SLO[e]].point);
            if(realtime == 1)
                repaint();
            colorcnt++;
        }
    }

    update();
}

void PaintWidget::calColorDistri(int N)
{
    QFile file("color_distribution.txt");
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);
    int n = colorcnt;
    max_color_size = 0;
    color_distribution = new int[n] {0};
    for(int e = 0; e < N; e++)
    {
        //qDebug() << nodes[e].color;
        color_distribution[nodes[e].color]++;
    }
    for(int i = 0; i < n; i++)
    {
        out << color_distribution[i] << '\n';
        if(color_distribution[i] > max_color_size)
            max_color_size = color_distribution[i];
    }
}

void PaintWidget::output_to_file(int N)
{
    QFile file1("degree when deleted.txt");
    if (!file1.open(QIODevice::WriteOnly|QIODevice::Text)) {
        return;
    }
    QTextStream out1(&file1);

    QFile file2("avg degree when deleted.txt");
    if (!file2.open(QIODevice::WriteOnly|QIODevice::Text)) {
        return;
    }
    QTextStream out2(&file2);

    QFile file3("origin degree.txt");
    if (!file3.open(QIODevice::WriteOnly|QIODevice::Text)) {
        return;
    }
    QTextStream out3(&file3);

    QFile file4("color.txt");
    if (!file4.open(QIODevice::WriteOnly|QIODevice::Text)) {
        return;
    }
    QTextStream out4(&file4);

    for(int e = N - 1; e >= 0; e--)
    {
        out1 << degree_wd[SLO[e]] << "\n";
        out2 << average_degree_wd[SLO[e]] << "\n";
        out3 << nodes[SLO[e]].degree << "\n";
    }
    for(int e = 0; e < N; e++)
        out4 << "Node: " << SLO[e] << " Color: " << nodes[SLO[e]].color << "\n";
}

void PaintWidget::print_step()
{
    QFile file("step by step.txt");
    if (!file.open(QIODevice::Append|QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);

    int max = max_degree.degree;

    for(int i = 0; i < max + 1; i++)
    {
        out << "degree " << i << ": ";
        DegreeNode* p = degree_linklist[i].head;
        while(p != NULL)
        {
            out << p->node.index << " ";
            p = p->next;
        }
        out << "\n";
    }
    out << "\n";
}

void PaintWidget::print_original(int N)
{
    QFile file("original.txt");
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);

    out << "adjacency list:\n";
    for(int e = 0; e < N; e++)
    {
        out << "node " << e << ": ";
        for(int i = 0; i < nodes[e].neighbor.length(); i++)
        {
            out << nodes[e].neighbor[i] << " ";
        }
        out << "\n";
    }
    out << "\n";

    int max = max_degree.degree;

    out << "original degree distribution list:\n";
    for(int i = 0; i < max + 1; i++)
    {
        out << "degree " << i << ": ";
        DegreeNode* p = degree_linklist[i].head;
        while(p != NULL)
        {
            out << p->node.index << " ";
            p = p->next;
        }
        out << "\n";
    }
}

void PaintWidget::generateBackbone(int N)
{
    backbone1.clear();
    backbone2.clear();
    backbone_array.clear();
    max = 0;
    second_max = 0;

    QTime time;
    time.start();

    get_bipartite(N);
    backbone_array.push_back(get_backbone(bipartite01));
    backbone_array.push_back(get_backbone(bipartite02));
    backbone_array.push_back(get_backbone(bipartite03));
    backbone_array.push_back(get_backbone(bipartite12));
    backbone_array.push_back(get_backbone(bipartite13));
    backbone_array.push_back(get_backbone(bipartite23));
    for(int i = 0; i < backbone_array.length(); i++)
    {
        if(backbone_array[i].length() > backbone1.length())
        {
            backbone2.swap(backbone1);
            backbone1 = backbone_array[i];
            second_max = max;
            max = i;
        }
        else if(backbone_array[i].length() > backbone2.length())
        {
            backbone2 = backbone_array[i];
            second_max = i;
        }
    }

    int* visited = new int[N];
    int counter = 0;

    for(int i = 0; i < N; i++)
    {
        visited[i] = 0;
    }

    for(int i = 0; i < backbone1.length(); i++)
    {
        if(visited[backbone1[i].index] == 0)
        {
            visited[backbone1[i].index] = 1;
            counter++;
        }
        for(int j = 0; j < nodes[backbone1[i].index].neighbor.length(); j++)
        {
            if(visited[nodes[backbone1[i].index].neighbor[j]] == 0)
            {
                visited[nodes[backbone1[i].index].neighbor[j]] = 1;
                counter++;
            }
        }
    }

    double percentage1 = (double)counter/N;

    int backbone1_total_degree = 0;
    for(int i = 0; i < backbone1.length(); i++)
    {
        backbone1_total_degree = backbone1_total_degree + backbone1[i].neighbor.length();
    }

    int edges1 = backbone1_total_degree/2;

    counter = 0;
    for(int i = 0; i < N; i++)
    {
        visited[i] = 0;
    }

    for(int i = 0; i < backbone2.length(); i++)
    {
        if(visited[backbone2[i].index] == 0)
        {
            visited[backbone2[i].index] = 1;
            counter++;
        }
        for(int j = 0; j < nodes[backbone2[i].index].neighbor.length(); j++)
        {
            if(visited[nodes[backbone2[i].index].neighbor[j]] == 0)
            {
                visited[nodes[backbone2[i].index].neighbor[j]] = 1;
                counter++;
            }
        }
    }

    double percentage2 = (double)counter/N;

    int backbone2_total_degree = 0;
    for(int i = 0; i < backbone2.length(); i++)
    {
        backbone2_total_degree = backbone2_total_degree + backbone2[i].neighbor.length();
    }

    int edges2 = backbone2_total_degree/2;

    int timeElapsed = time.elapsed();
    double runtime = timeElapsed/1000.0;

    int vertices1 = backbone1.length();
    int vertices2 = backbone2.length();

    Result_p3 result;
    result.ID = count;
    result.vertices1 = vertices1;
    result.vertices2 = vertices2;
    result.edges1 = edges1;
    result.edges2 = edges2;
    result.percentage1 = percentage1;
    result.percentage2 = percentage2;
    result.running_time = runtime;
    results_p3.push_back(result);
    emit send_result_p3(result);
}

void PaintWidget::drawBackbone(int backbone_id, int realtime)
{
    mypixmap.fill(Qt::white);
    QPainter painter(&mypixmap);
    QPen pen;
    pen.setColor(Qt::red);

    painter.setBrush(QBrush(QColor(255, 0, 0, 50)));

    QVector<Node> target_backbone;

    int index;

    if(backbone_id == 1)
    {
        target_backbone = backbone1;
        index = max;
    }
    else
    {
        target_backbone = backbone2;
        index = second_max;
    }
    for(int i = 0; i < target_backbone.length(); i++)
    {
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawPoint(target_backbone[i].point);
        //painter.drawEllipse(target_backbone[i].point, RR, RR);
        if(realtime == 1)
            repaint();
        for(int j = 0; j < target_backbone[i].neighbor.length(); j++)
        {
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawLine(target_backbone[i].point, bipartite_array[index][target_backbone[i].neighbor[j]].point);
            //painter.drawEllipse(target_backbone[i].point, RR, RR);
            if(realtime == 1)
                repaint();
        }
    }
    update();
}

QVector<Node> PaintWidget::get_backbone(QVector<Node> bipartite)
{
    QVector<Node> new_backbone;
    QVector<int> container;
    QQueue<int> q;
    int V = bipartite.length();
    bool* visited = new bool[V];
    for(int i = 0; i < V; i++)
    {
        visited[i] = false;
    }
    for(int i = 0; i < V; i++)
    {
        QVector<int> temp;
        if(visited[i] == false)
        {
            q.enqueue(i);
            visited[i] = true;
            temp.push_back(i);
            while(!q.isEmpty())
            {
                int traversal = q.dequeue();
                for(int j = 0; j < bipartite[traversal].neighbor.length(); j++)
                {
                    if(visited[bipartite[traversal].neighbor[j]] == false)
                    {
                        q.enqueue(bipartite[traversal].neighbor[j]);
                        visited[bipartite[traversal].neighbor[j]] = true;
                        temp.push_back(bipartite[traversal].neighbor[j]);
                    }
                }
            }
        }
        if(temp.length() > container.length())
        {
            container.swap(temp);
        }
    }
    while(1)
    {
        int flag = 0;
        for(int i = 0; i < container.length(); i++)
        {
            if(bipartite[container[i]].degree == 1)
            {
                flag = 1;
                bipartite[bipartite[container[i]].neighbor[0]].degree--;
                for(int j = 0; j < bipartite[bipartite[container[i]].neighbor[0]].neighbor.length(); j++)
                {
                    if(bipartite[bipartite[container[i]].neighbor[0]].neighbor[j] == container[i])
                    {
                        bipartite[bipartite[container[i]].neighbor[0]].neighbor.remove(j);
                    }
                }
                container.remove(i);
            }
        }
        if(flag == 0)
            break;
    }
    for(int i = 0; i < container.length(); i++)
    {
        new_backbone.push_back(bipartite[container[i]]);
    }
    return new_backbone;
}

void PaintWidget::get_bipartite(int N)
{
    bipartite_array.clear();
    QVector<Node> bipartite01_t;
    QVector<Node> bipartite02_t;
    QVector<Node> bipartite03_t;
    QVector<Node> bipartite12_t;
    QVector<Node> bipartite13_t;
    QVector<Node> bipartite23_t;
    bipartite01.clear();
    bipartite02.clear();
    bipartite03.clear();
    bipartite12.clear();
    bipartite13.clear();
    bipartite23.clear();
    int count01 = 0;
    int count02 = 0;
    int count03 = 0;
    int count12 = 0;
    int count13 = 0;
    int count23 = 0;
    for(int e = 0; e < N; e++)
    {
        if(nodes[e].color == 0)
        {
            bipartite01_t.push_back(nodes[e]);
            bipartite02_t.push_back(nodes[e]);
            bipartite03_t.push_back(nodes[e]);
            bipartite01.push_back(nodes[e]);
            bipartite02.push_back(nodes[e]);
            bipartite03.push_back(nodes[e]);
            nodes[e].bi_index[0] = count01;
            nodes[e].bi_index[1] = count02;
            nodes[e].bi_index[2] = count03;
            count01++;
            count02++;
            count03++;
        }
        else if(nodes[e].color == 1)
        {
            bipartite01_t.push_back(nodes[e]);
            bipartite12_t.push_back(nodes[e]);
            bipartite13_t.push_back(nodes[e]);
            bipartite01.push_back(nodes[e]);
            bipartite12.push_back(nodes[e]);
            bipartite13.push_back(nodes[e]);
            nodes[e].bi_index[0] = count01;
            nodes[e].bi_index[3] = count12;
            nodes[e].bi_index[4] = count13;
            count01++;
            count12++;
            count13++;
        }
        else if(nodes[e].color == 2)
        {
            bipartite02_t.push_back(nodes[e]);
            bipartite12_t.push_back(nodes[e]);
            bipartite23_t.push_back(nodes[e]);
            bipartite02.push_back(nodes[e]);
            bipartite12.push_back(nodes[e]);
            bipartite23.push_back(nodes[e]);
            nodes[e].bi_index[1] = count02;
            nodes[e].bi_index[3] = count12;
            nodes[e].bi_index[5] = count23;
            count02++;
            count12++;
            count23++;
        }
        else if(nodes[e].color == 3)
        {
            bipartite13_t.push_back(nodes[e]);
            bipartite23_t.push_back(nodes[e]);
            bipartite03_t.push_back(nodes[e]);
            bipartite13.push_back(nodes[e]);
            bipartite23.push_back(nodes[e]);
            bipartite03.push_back(nodes[e]);
            nodes[e].bi_index[4] = count13;
            nodes[e].bi_index[5] = count23;
            nodes[e].bi_index[2] = count03;
            count13++;
            count23++;
            count03++;
        }
    }
    for(int i = 0; i < bipartite01.length(); i++)
    {
        bipartite01[i].neighbor.clear();
        for(int j = 0; j < bipartite01_t[i].neighbor.length(); j++)
        {
            if(nodes[bipartite01_t[i].neighbor[j]].color == 0 || nodes[bipartite01_t[i].neighbor[j]].color == 1)
            {
                bipartite01[i].neighbor.push_back(nodes[bipartite01_t[i].neighbor[j]].bi_index[0]);
            }
        }
        bipartite01[i].degree = bipartite01[i].neighbor.length();
    }
    for(int i = 0; i < bipartite02.length(); i++)
    {
        bipartite02[i].neighbor.clear();
        for(int j = 0; j < bipartite02_t[i].neighbor.length(); j++)
        {
            if(nodes[bipartite02_t[i].neighbor[j]].color == 0 || nodes[bipartite02_t[i].neighbor[j]].color == 2)
            {
                bipartite02[i].neighbor.push_back(nodes[bipartite02_t[i].neighbor[j]].bi_index[1]);
            }
        }
        bipartite02[i].degree = bipartite02[i].neighbor.length();
    }
    for(int i = 0; i < bipartite03.length(); i++)
    {
        bipartite03[i].neighbor.clear();
        for(int j = 0; j < bipartite03_t[i].neighbor.length(); j++)
        {
            if(nodes[bipartite03_t[i].neighbor[j]].color == 0 || nodes[bipartite03_t[i].neighbor[j]].color == 3)
            {
                bipartite03[i].neighbor.push_back(nodes[bipartite03_t[i].neighbor[j]].bi_index[2]);
            }
        }
        bipartite03[i].degree = bipartite03[i].neighbor.length();
    }
    for(int i = 0; i < bipartite12.length(); i++)
    {
        bipartite12[i].neighbor.clear();
        for(int j = 0; j < bipartite12_t[i].neighbor.length(); j++)
        {
            if(nodes[bipartite12_t[i].neighbor[j]].color == 1 || nodes[bipartite12_t[i].neighbor[j]].color == 2)
            {
                bipartite12[i].neighbor.push_back(nodes[bipartite12_t[i].neighbor[j]].bi_index[3]);
            }
        }
        bipartite12[i].degree = bipartite12[i].neighbor.length();
    }
    for(int i = 0; i < bipartite13.length(); i++)
    {
        bipartite13[i].neighbor.clear();
        for(int j = 0; j < bipartite13_t[i].neighbor.length(); j++)
        {
            if(nodes[bipartite13_t[i].neighbor[j]].color == 1 || nodes[bipartite13_t[i].neighbor[j]].color == 3)
            {
                bipartite13[i].neighbor.push_back(nodes[bipartite13_t[i].neighbor[j]].bi_index[4]);
            }
        }
        bipartite13[i].degree = bipartite13[i].neighbor.length();
    }
    for(int i = 0; i < bipartite23.length(); i++)
    {
        bipartite23[i].neighbor.clear();
        for(int j = 0; j < bipartite23_t[i].neighbor.length(); j++)
        {
            if(nodes[bipartite23_t[i].neighbor[j]].color == 2 || nodes[bipartite23_t[i].neighbor[j]].color == 3)
            {
                bipartite23[i].neighbor.push_back(nodes[bipartite23_t[i].neighbor[j]].bi_index[5]);
            }
        }
        bipartite23[i].degree = bipartite23[i].neighbor.length();
    }
    bipartite_array.push_back(bipartite01);
    bipartite_array.push_back(bipartite02);
    bipartite_array.push_back(bipartite03);
    bipartite_array.push_back(bipartite12);
    bipartite_array.push_back(bipartite13);
    bipartite_array.push_back(bipartite23);
}
