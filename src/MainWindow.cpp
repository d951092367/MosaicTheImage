#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    // �����ؼ�
    imageWidget = new ImageWidget(this);
    QPushButton* loadButton = new QPushButton("Load Image", this);
    QPushButton* saveButton = new QPushButton("Save Image", this);

    // ����
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(imageWidget);
    layout->addWidget(loadButton);
    layout->addWidget(saveButton);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // �����ź����
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);
}

void MainWindow::loadImage()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!filePath.isEmpty())
    {
        imageWidget->loadPixmap(filePath);
    }
}

void MainWindow::saveImage()
{
    imageWidget->savePixmap();
}