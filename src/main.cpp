#include "MosaicTheImage.h"
#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include<qmessagebox.h>
#include <CanvasLabel.h>

//int main(int argc, char *argv[])
//{
//    QApplication app(argc, argv);
//    MosaicTheImage window;
//    window.show();
//    return app.exec();
//}


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    CanvasLabel* canvas = new CanvasLabel;
    QPushButton* btnOpen = new QPushButton("Open");
    QPushButton* btnSave = new QPushButton("Extract");

    QHBoxLayout* h = new QHBoxLayout;
    h->addWidget(btnOpen);
    h->addWidget(btnSave);

    QVBoxLayout* v = new QVBoxLayout(&w);
    v->addLayout(h);
    v->addWidget(canvas);

    QObject::connect(btnOpen, &QPushButton::clicked, [&]() {
        QString f = QFileDialog::getOpenFileName(nullptr,
            "Open Image",
            "",
            "Images (*.png *.jpg *.bmp)");
        if (!f.isEmpty() && !canvas->LoadImage(f))
            QMessageBox::warning(nullptr, "Error", "Cannot load image!");
        });

    QObject::connect(btnSave, &QPushButton::clicked, canvas, &CanvasLabel::ExtractAndSave);
    w.resize(800, 600);
    w.show();
    return a.exec();
}
