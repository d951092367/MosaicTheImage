
#include<QMouseEvent>
#include<QPainter>
#include<QLayout>
#include<QFileDialog>
#include<QGuiApplication>
#include<QClipboard>
#include "CanvasLabel.h"
#include"Tools.hpp"



CanvasLabel::CanvasLabel(QWidget* parent)
    : QLabel(parent)
{
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setFocusPolicy(Qt::StrongFocus);  
}

CanvasLabel::~CanvasLabel()
{
}

bool CanvasLabel::LoadImage(const QString& file)
{

    cv::Mat img = cv::imread(file.toStdString());
    m_src_img = CvMatToQImage(img);

    if(m_src_img.isNull()) 
        return false;

    m_src_mosaic_img = MosaicAImage(file.toStdString(),17);
    if (m_src_mosaic_img.isNull()) 
        return false;

    //ShowMosaicImage(m_src_mosaic_img);
    m_mask_img = QImage(m_src_img.size(), QImage::Format_Grayscale8);
    m_mask_img.fill(0);
    m_display_img = m_src_img.copy();

    m_history_display.clear();
    m_history_mask.clear();

    UpdatePixmap();
    SaveState();
    return true;
}

void CanvasLabel::ShowMosaicImage(const QImage& image)
{
   
    QLabel* label = new QLabel();
    label->setPixmap(QPixmap::fromImage(image));
    label->setScaledContents(true);
    label->resize(image.width(), image.height()); 

    QWidget* window = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(window);
    layout->addWidget(label);
    window->setLayout(layout);
    window->setWindowTitle("Mosaic Image");
    window->show();
}

QImage CanvasLabel::GetImage()
{
    return m_display_img;
}

void CanvasLabel::ExtractAndSave()
{
    if (m_display_img.isNull())
        return;
    QImage result(m_display_img.size(), QImage::Format_ARGB32);
    for (int y = 0; y < m_src_img.height(); ++y)
    {
        for (int x = 0; x < m_src_img.width(); ++x)
        {
            replacePixel(result, m_display_img, x, y);
        }
    }
  
    QString out = QFileDialog::getSaveFileName(this,
        "Save extracted",
        "extracted.png",
        "PNG (*.png)");
    if (!out.isEmpty())
    {
        result.save(out, "PNG");
        QGuiApplication::clipboard()->setImage(result);
    }
        
}

void CanvasLabel::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        last_point = e->pos();
        DrawPoint(last_point);
    }
}

void CanvasLabel::mouseMoveEvent(QMouseEvent* e)
{
    if (e->buttons() & Qt::LeftButton) {
        DrawLine(last_point, e->pos());
        last_point = e->pos();
    }
}

void CanvasLabel::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        SaveState();
    }
       
}

void CanvasLabel::keyPressEvent(QKeyEvent* e)
{
    if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_Z)
    {
        RestoreState();
    }
}

void CanvasLabel::UpdatePixmap()
{
    this->setPixmap(QPixmap::fromImage(m_display_img));
}

void CanvasLabel::SaveState()
{
    if (m_history_display.size() >= maxHistory) {
       
        auto f_1 = m_history_display.front();
        auto f_2 = m_history_mask.front ();
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

void CanvasLabel::RestoreState()
{
    if (m_history_display.size() < 2)
    {
        return;
    }
    m_history_display.pop_back();
    m_display_img = m_history_display.back();
    m_history_mask.pop_back();
    m_mask_img = m_history_mask.back();
    UpdatePixmap();
}

void CanvasLabel::DrawPoint(const QPoint& p)
{
    QPainter pm(&m_mask_img);
    pm.setCompositionMode(QPainter::CompositionMode_Source);
    pm.setPen(QPen(Qt::white, pen_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    pm.drawPoint(p);
    //����diplayimg
    for (int y = 0; y < m_src_img.height(); ++y)
    {
        const uchar* m = m_mask_img.constScanLine(y);
        /*const QRgb* s = reinterpret_cast<const QRgb*>(m_src_mosaic_img.constScanLine(y));
        QRgb* d = reinterpret_cast<QRgb*>(m_display_img.scanLine(y));*/
        for (int x = 0; x < m_src_img.width(); ++x) {
            if (m[x] == 255)
            {
                replacePixel(m_display_img, m_src_mosaic_img, x, y);
            }
                
        }
    }
    UpdatePixmap();
}

void CanvasLabel::DrawLine(const QPoint& a, const QPoint& b)
{
    QPainter pm(&m_mask_img);
    pm.setCompositionMode(QPainter::CompositionMode_Source);
    pm.setPen(QPen(Qt::white, pen_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    pm.drawLine(a, b);

    //����diplayimg
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
    UpdatePixmap();
}
