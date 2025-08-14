#include "ImageWidget.h"
#include <QPainter>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QScrollBar>
#include <opencv2/opencv.hpp>
#include<string>
#include"Tools.hpp"

ImageWidget::ImageWidget(QWidget* parent) : QGraphicsView(parent)
{
    // 创建场景
    scene_ = new QGraphicsScene(this);
    this->setScene(scene_);
    // 初始化变量
    isDrawing = false;
    /*drawingColor = Qt::black;
    drawingPen.setWidth(3);
    drawingPen.setColor(drawingColor);*/
    // 设置鼠标跟踪
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void ImageWidget::loadPixmap(const QString& file)
{
    cv::Mat img = cv::imread(file.toStdString());
    m_src_img = CvMatToQImage(img);
    if (m_src_img.isNull())
        return;
    m_src_mosaic_img = MosaicAImage(file.toStdString(), 17);
    if (m_src_mosaic_img.isNull())
        return ;

    m_mask_img = QImage(m_src_img.size(), QImage::Format_Grayscale8);
    m_mask_img.fill(0);
    m_display_img = m_src_img.copy();

    m_history_display.clear();
    m_history_mask.clear();

    //imagePixmap = QPixmap::fromImage(m_src_img);
    scene_->clear(); // 清空场景
    pixmapItem = scene_->addPixmap(QPixmap::fromImage(m_src_img)); // 保存 QPixmapItem 的指针


    updatePixmap();
    saveState();

}

void ImageWidget::savePixmap()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!filePath.isEmpty())
    {
        QRectF rect = pixmapItem->boundingRect();
        QPixmap finalPixmap(rect.size().toSize());
        finalPixmap.fill(Qt::transparent);

        QPainter painter(&finalPixmap);
        scene_->render(&painter, rect, rect); // 只渲染 boundingRect 的范围
        finalPixmap.save(filePath);
    }
}

void ImageWidget::updatePixmap()
{
    pixmapItem->setPixmap(QPixmap::fromImage(m_display_img));
}

void ImageWidget::saveState()
{
    if (m_history_display.size() >= maxHistory) {

        auto f_1 = m_history_display.front();
        auto f_2 = m_history_mask.front();
        m_history_display.pop_front();
        m_history_mask.pop_front();
        m_history_display.pop_front();
        m_history_mask.pop_front();
        m_history_display.push_front(f_1);
        m_history_mask.push_front(f_2);

    }
    m_history_display.append(m_display_img.copy());
    m_history_mask.append(m_mask_img.copy());
}

void ImageWidget::restoreState()
{
    if (m_history_display.size() < 2)
    {
        return;
    }
    m_history_display.pop_back();
    m_display_img = m_history_display.back();
    m_history_mask.pop_back();
    m_mask_img = m_history_mask.back();
    updatePixmap();
}

void ImageWidget::drawPoint(const QPointF& p)
{
    QPainter pm(&m_mask_img);
    pm.setCompositionMode(QPainter::CompositionMode_Source);
    pm.setPen(QPen(Qt::white, pen_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    pm.drawPoint(p);

    for (int y = 0; y < m_src_img.height(); ++y)
    {
        const uchar* m = m_mask_img.constScanLine(y);
        for (int x = 0; x < m_src_img.width(); ++x) {
            if (m[x] == 255)
            {
                replacePixel(m_display_img, m_src_mosaic_img, x, y);
            }

        }
    }
    updatePixmap();
}

void ImageWidget::drawLine(const QPointF& a, const QPointF& b)
{
    QPainter pm(&m_mask_img);
    pm.setCompositionMode(QPainter::CompositionMode_Source);
    pm.setPen(QPen(Qt::white, pen_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    pm.drawLine(a, b);
    for (int y = 0; y < m_src_img.height(); ++y)
    {
        const uchar* m = m_mask_img.constScanLine(y);
        const QRgb* s = reinterpret_cast<const QRgb*>(m_src_mosaic_img.constScanLine(y));
        QRgb* d = reinterpret_cast<QRgb*>(m_display_img.scanLine(y));
        for (int x = 0; x < m_src_img.width(); ++x) {
            if (m[x] == 255)
            {
                replacePixel(m_display_img, m_src_mosaic_img, x, y);
            }
        }
    }
    updatePixmap();
}

void ImageWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDrawing = true;
        lastPoint = mapToScene(event->pos());
        drawPoint(lastPoint);
    }
    else if (event->button() == Qt::RightButton) // 处理右键按下
    {
        isPanning = true;
        panStartPos = event->pos();
    }
    else
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (isDrawing)
    {
        QPointF currentPoint = mapToScene(event->pos());
        drawLine(lastPoint, currentPoint);
        lastPoint = currentPoint;
    }
    else if (isPanning) // 处理拖动画面
    {
        QPointF delta = panStartPos - event->pos();
        QScrollBar* hBar = horizontalScrollBar();
        QScrollBar* vBar = verticalScrollBar();
        hBar->setValue(hBar->value() + delta.x());
        vBar->setValue(vBar->value() + delta.y());
        panStartPos = event->pos();

        // 动态扩展场景范围
        QRectF sceneRect = scene_->sceneRect();
        QRectF viewRect = mapToScene(rect()).boundingRect();
        if (sceneRect.width() < viewRect.width() || sceneRect.height() < viewRect.height())
        {
            sceneRect = sceneRect.united(viewRect);
            scene_->setSceneRect(sceneRect);
        }
    }
    else
    {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDrawing = false;
        saveState();
    }
    else if (event->button() == Qt::RightButton) // 处理右键释放
    {
        isPanning = false;
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

//void ImageWidget::wheelEvent(QWheelEvent* event)
//{
//    QPoint prev_viewPos = event->pos();
//    QPointF prev_scenePos = mapToScene(prev_viewPos);
//    qreal factor = event->angleDelta().y() > 0 ? 1.1 : 0.9;
//    this->scale(factor, factor);
//
//    // 调整场景范围以适应视图
//    QRectF sceneRect = scene_->sceneRect();
//    QRectF viewRect = mapToScene(rect()).boundingRect();
//    if (sceneRect.width() < viewRect.width() || sceneRect.height() < viewRect.height())
//    {
//        sceneRect = sceneRect.united(viewRect);
//        scene_->setSceneRect(sceneRect);
//    }
//
//    // 确保鼠标位置在缩放后保持一致
//    QPointF scenePos = mapToScene(prev_viewPos);
//    QPointF disPointF = scenePos - prev_scenePos;
//    scene_->setSceneRect(scene_->sceneRect().x() - disPointF.x(), scene_->sceneRect().y() - disPointF.y(),
//        scene_->sceneRect().width(), scene_->sceneRect().height());
//}

void ImageWidget::wheelEvent(QWheelEvent* event)
{

    double scaleFactor = event->angleDelta().y() > 0 ? 1.1 : 0.9;
    this->scale(scaleFactor, scaleFactor);

    // 调整场景范围以适应视图
    QRectF sceneRect = scene_->sceneRect();
    QRectF viewRect = mapToScene(rect()).boundingRect();
    if (sceneRect.width() < viewRect.width() || sceneRect.height() < viewRect.height())
    {
        sceneRect = sceneRect.united(viewRect);
        scene_->setSceneRect(sceneRect);
    }  
}

void ImageWidget::keyPressEvent(QKeyEvent* e)
{


    std::cout << "press" << std::endl;
    if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_Z)
    {
        restoreState();
    }
}
