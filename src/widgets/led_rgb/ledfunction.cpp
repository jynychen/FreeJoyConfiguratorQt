#include "ledfunction.h"
#include "ui_ledfunction.h"

LedFunction::LedFunction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedFunction)
{
    ui->setupUi(this);

    connect(ui->spinBox_buttonNum, SIGNAL(valueChanged(int)), this, SIGNAL(logButtonChanged(int)));
    connect(ui->checkBox_isInverted, SIGNAL(toggled(bool)), this, SIGNAL(invertChanged(bool)));
}

LedFunction::~LedFunction()
{
    delete ui;
}


void LedFunction::setLogButton(int num)
{
    ui->spinBox_buttonNum->setValue(num);
}

int LedFunction::logButtonNum() const
{
    return ui->spinBox_buttonNum->value();
}


void LedFunction::setInverted(bool inverted)
{
    ui->checkBox_isInverted->setChecked(inverted);
}

bool LedFunction::Inverted() const
{
    return ui->checkBox_isInverted->isChecked();
}



void LedFunction::mousePressEvent(QMouseEvent *event)
{

}

void LedFunction::mouseReleaseEvent(QMouseEvent *event)
{

}
