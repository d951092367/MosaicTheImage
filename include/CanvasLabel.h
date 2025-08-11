#ifndef DJW_CANVAS_LABEL_H
#define DJW_CANVAS_LABEL_H


#include <QLabel>
#include <QImage>
#include <QPoint>
#include <QVector>


class CanvasLabel : public QLabel
{
	Q_OBJECT
public:
	explicit CanvasLabel(QWidget* parent = nullptr);
	~CanvasLabel();

public:
	bool LoadImage(const QString& file);
	void ShowMosaicImage(const QImage& image);
	QImage GetImage();
	void ExtractAndSave();

	
	

protected:
	void mousePressEvent(QMouseEvent* e)    override;
	void mouseMoveEvent(QMouseEvent* e)     override;
	void mouseReleaseEvent(QMouseEvent* e)  override;
	void keyPressEvent(QKeyEvent* e)		override;  

private:
	void UpdatePixmap();
	void SaveState();
	void RestoreState();
	

	void DrawPoint(const QPoint& p);
	void DrawLine(const QPoint& a, const QPoint& b);


private:
	QImage m_src_img;
	QImage m_src_mosaic_img;
	QImage m_mask_img;
	QImage m_display_img;

	QPoint last_point;     // ��һ�����λ��

	QVector<QImage> m_history_display;
	QVector<QImage> m_history_mask;


	const int maxHistory = 50;  //min_value = 2
	int		  pen_width = 15;
};

#endif // CANVAS_H