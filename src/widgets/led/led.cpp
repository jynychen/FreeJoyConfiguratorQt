#include "led.h"
#include "ui_led.h"
#include <QEvent>
#include <QMouseEvent>
#include <QCursor>

LED::LED(int ledNumber, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LED)
    , m_ledCurrentState(false)
{
    ui->setupUi(this);
    m_ledNumber = ledNumber;
    ui->pushButton_LEDNumber->setText(QString::number(ledNumber + 1));

    for (uint i = 0; i < std::size(m_ledList); ++i) {
        ui->comboBox_Function->addItem(m_ledList[i].guiName);
    }

    for (uint i = 0; i < std::size(m_TimerList); ++i) {
        ui->comboBox_Timer->addItem(m_TimerList[i].guiName);
    }

    connect(ui->pushButton_LEDNumber, &QPushButton::clicked, this, [=](){
        if (isHostControlled()) {
            setLedState(!m_ledCurrentState);
            emit ledToggled(m_ledNumber, m_ledCurrentState);
        }
    });

    connect(ui->checkBox_HostControlled, &QCheckBox::toggled, this, &LED::updateButtonStyle);
    connect(ui->checkBox_HostControlled, &QCheckBox::toggled, this, [=](bool checked){
        ui->spinBox_InputNumber->setEnabled(!checked);
    });
}

LED::~LED()
{
    delete ui;
}

void LED::retranslateUi()
{
    ui->retranslateUi(this);
}

int LED::currentButtonSelected() const
{
    return ui->spinBox_InputNumber->value() - 1;
}

void LED::updateButtonStyle(bool checked) {
    ui->pushButton_LEDNumber->setChecked(m_ledCurrentState);

    ui->pushButton_LEDNumber->setFlat(!checked);
    ui->pushButton_LEDNumber->setEnabled(checked);
};

void LED::setLedState(bool state)
{
    if (state != m_ledCurrentState) {
        m_ledCurrentState = state;
        bool checked = ui->checkBox_HostControlled->isChecked();
        updateButtonStyle(checked);
    }
}

void LED::readFromConfig()
{
    led_config_t *led = &gEnv.pDeviceConfig->config.leds[m_ledNumber];
    if (gEnv.pDeviceConfig->config.leds[m_ledNumber].input_num == SOURCE_HOST) {
        ui->checkBox_HostControlled->setChecked(true);
        ui->spinBox_InputNumber->setEnabled(false);
    } else {
        ui->checkBox_HostControlled->setChecked(false);
        ui->spinBox_InputNumber->setEnabled(true);
        ui->spinBox_InputNumber->setValue(led->input_num + 1);
    }
    ui->comboBox_Function->setCurrentIndex(led->type);
    ui->comboBox_Timer->setCurrentIndex(led->timer + 1); // +1 because first element in m_TimerList = -1
}

void LED::writeToConfig()
{
    led_config_t *led = &gEnv.pDeviceConfig->config.leds[m_ledNumber];
    led->input_num = (ui->checkBox_HostControlled->isChecked())
        ? SOURCE_HOST
        : ui->spinBox_InputNumber->value() - 1;
    led->type = ui->comboBox_Function->currentIndex();
    led->timer = ui->comboBox_Timer->currentIndex() - 1; // -1 because first element in m_TimerList = -1
}

bool LED::isHostControlled() const
{
    return ui->checkBox_HostControlled->isChecked();
}

void LED::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::PaletteChange) {
        updateButtonStyle(isHostControlled());
    }
    QWidget::changeEvent(event);
}
