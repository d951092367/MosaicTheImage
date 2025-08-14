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
    void updatePixmap(); // 新增方法，用于更新 QPixmap
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
    QGraphicsPixmapItem* pixmapItem; // 保存 QPixmapItem 的指针
    bool isDrawing;
    bool isPanning; // 新增变量，标记是否正在拖动
    QPointF lastPoint;
    //QPen drawingPen;
    QColor drawingColor;
    QPointF panStartPos; // 新增变量，记录拖动的起始位置

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