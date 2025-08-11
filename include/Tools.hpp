#ifndef  DJW_TOOLS_HPP
#define	 DJW_TOOLS_HPP


#include <QImage>

#include <opencv2/opencv.hpp>
#include<string>


//QImage CvMatToQImage(const cv::Mat& mat)
//{
//    if (mat.empty()) return QImage();
//    switch (mat.type()) {
//    case CV_8UC1: 
//        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8).copy();
//    case CV_8UC3:
//    {
//        cv::Mat rgb;
//        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
//        return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
//    }
//    case CV_8UC4: 
//    {
//        cv::Mat rgba;
//        cv::cvtColor(mat, rgba, cv::COLOR_BGRA2RGBA);
//        return QImage(rgba.data, rgba.cols, rgba.rows, rgba.step, QImage::Format_ARGB32).copy();
//    }
//    default:
//        return QImage();
//    }
//}


QImage CvMatToQImage(const cv::Mat& mat)
{
    if (mat.empty()) return QImage();
    switch (mat.type()) {
    case CV_8UC1:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8).copy();
    case CV_16UC1:
    {
        cv::Mat gray8;
        mat.convertTo(gray8, CV_8UC1);
        return QImage(gray8.data, gray8.cols, gray8.rows, gray8.step, QImage::Format_Grayscale8).copy();
    }
    case CV_32FC1:
    {
        cv::Mat gray8;
        mat.convertTo(gray8, CV_8UC1, 255.0);
        return QImage(gray8.data, gray8.cols, gray8.rows, gray8.step, QImage::Format_Grayscale8).copy();
    }
    case CV_8UC2:
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_YUV2RGB); // �������ʵ���ת��
        return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
    }
    case CV_8UC3:
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
    }
    case CV_16UC3:
    {
        cv::Mat rgb8;
        mat.convertTo(rgb8, CV_8UC3);
        return QImage(rgb8.data, rgb8.cols, rgb8.rows, rgb8.step, QImage::Format_RGB888).copy();
    }
    case CV_32FC3:
    {
        cv::Mat rgb8;
        mat.convertTo(rgb8, CV_8UC3, 255.0);
        return QImage(rgb8.data, rgb8.cols, rgb8.rows, rgb8.step, QImage::Format_RGB888).copy();
    }
    case CV_8UC4:
    {
        cv::Mat rgba;
        cv::cvtColor(mat, rgba, cv::COLOR_BGRA2RGBA);
        return QImage(rgba.data, rgba.cols, rgba.rows, rgba.step, QImage::Format_ARGB32).copy();
    }
    default:
        return QImage();
    }
}

cv::Mat QImageToMat(const QImage& qimg)
{
   
    if (qimg.isNull())
    {
        return cv::Mat();
    }
    switch (qimg.format())
    {
    case QImage::Format_RGB32:
    {
        cv::Mat mat(qimg.height(), qimg.width(), CV_8UC4, (uchar*)qimg.bits(), qimg.bytesPerLine());
        cv::Mat mat2;
        cv::cvtColor(mat, mat2, cv::COLOR_BGRA2BGR); 
        return mat2;
    }
    case QImage::Format_ARGB32:
    {
        cv::Mat mat(qimg.height(), qimg.width(), CV_8UC4, (uchar*)qimg.bits(), qimg.bytesPerLine());
        cv::Mat mat2;
        cv::cvtColor(mat, mat2, cv::COLOR_BGRA2BGR); 
        return mat2;
    }
    case QImage::Format_RGB888:
    {
        cv::Mat mat(qimg.height(), qimg.width(), CV_8UC3, (uchar*)qimg.bits(), qimg.bytesPerLine());
        return mat;
    }
    case QImage::Format_Grayscale8:
    {
        cv::Mat mat(qimg.height(), qimg.width(), CV_8UC1, (uchar*)qimg.bits(), qimg.bytesPerLine());
        return mat;
    }
    default:
    {
        QImage convertedImg = qimg.convertToFormat(QImage::Format_RGB888);
        cv::Mat mat(convertedImg.height(), convertedImg.width(), CV_8UC3, (uchar*)convertedImg.bits(), convertedImg.bytesPerLine());
        return mat;
    }
    }
}

//void replacePixel(QImage& targetImage, const QImage& sourceImage, int x, int y)
//{
//    if (x < 0 || x >= targetImage.width() || y < 0 || y >= targetImage.height())
//    {
//        return;
//    }
//    QRgb sourcePixel = sourceImage.pixel(x, y);
//    targetImage.setPixel(x, y, sourcePixel);
//}


void replacePixel(QImage& targetImage, const QImage& sourceImage, int x, int y)
{
    if (x < 0 || x >= targetImage.width() || y < 0 || y >= targetImage.height())
    {
        return;
    }
    QRgb sourcePixel = sourceImage.pixel(x, y);
    if (sourceImage.format() != targetImage.format())
    {
        QColor sourceColor(sourcePixel);
        if (targetImage.format() == QImage::Format_Grayscale8)
        {
            int gray = static_cast<int>(0.299 * sourceColor.red() + 0.587 * sourceColor.green() + 0.114 * sourceColor.blue());
            targetImage.setPixel(x, y, qRgb(gray, gray, gray));
        }
        else
        {
            targetImage.setPixel(x, y, sourceColor.rgba());
        }
    }
    else
    {
        targetImage.setPixel(x, y, sourcePixel);
    }
}

QImage MosaicAImage(std::string imagepath,int size = 25)
{
    cv::Mat img = cv::imread(imagepath);
    if (img.empty())
        return QImage();


    cv::Mat blurredImg1;
    cv::Mat blurredImg2;
    cv::blur(img, blurredImg1, cv::Size(size, size));

    cv::GaussianBlur(blurredImg1, blurredImg2, cv::Size(size, size), 0);

   /* cv::Mat blurredImg;
    cv::GaussianBlur(img, blurredImg, cv::Size(size,size), 0);*/
    return CvMatToQImage(blurredImg2);
}


#endif // ! DJW_TOOLS_HPP

