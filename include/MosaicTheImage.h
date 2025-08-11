#pragma once

#include <QtWidgets/QDialog>
#include "ui_MosaicTheImage.h"

class MosaicTheImage : public QDialog
{
    Q_OBJECT

public:
    MosaicTheImage(QWidget *parent = nullptr);
    ~MosaicTheImage();

private:
    Ui::MosaicTheImageClass ui;
};

