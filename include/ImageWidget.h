#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QPen>
#include <QColor>
#include <QMouseEvent>
#include <QWheelEvent>
#include<QImage>

class ImageWidget : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget* parent = nullptr);
    void loadPixmap(const QString& file);
    void savePixmap();
   
private:
    void updatePixmap(); // �������������ڸ��� QPixmap
    void saveState();
    void restoreState();
    void drawPoint(const QPointF& p);
    void drawLine(const QPointF& a, const QPointF& b);
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* e)		override;
private:
    QGraphicsScene* scene_;
    //QPixmap imagePixmap;
    QGraphicsPixmapItem* pixmapItem; // ���� QPixmapItem ��ָ��
    bool isDrawing;
    bool isPanning; // ��������������Ƿ������϶�
    QPointF lastPoint;
    //QPen drawingPen;
    QColor drawingColor;
    QPointF panStartPos; // ������������¼�϶�����ʼλ��

    QImage m_src_img;
    QImage m_src_mosaic_img;
    QImage m_mask_img;
    QImage m_display_img;

    QVector<QImage> m_history_display;
    QVector<QImage> m_history_mask;


    const int maxHistory = 50;  //min_value = 2
    int		  pen_width = 1;


};

#endif // IMAGEWIDGET_H