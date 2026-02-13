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
    ui->label_LEDNumber->setNum(ledNumber + 1);

    // cache default style per-instance
   m_defaultStyle = ui->label_LEDNumber->styleSheet();

    for (uint i = 0; i < std::size(m_ledList); ++i) {
        ui->comboBox_Function->addItem(m_ledList[i].guiName);
    }

    for (uint i = 0; i < std::size(m_TimerList); ++i) {
        ui->comboBox_Timer->addItem(m_TimerList[i].guiName);
    }

    // allow clicking the label when host controlled
    ui->label_LEDNumber->installEventFilter(this);

    connect(ui->checkBox_HostControlled, &QCheckBox::toggled, this, [=](bool checked){
        ui->spinBox_InputNumber->setEnabled(!checked);
        // visual hint and interaction
        ui->label_LEDNumber->setCursor(checked ? Qt::PointingHandCursor : Qt::ArrowCursor);
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

void LED::setLedState(bool state)
{
    if (state != m_ledCurrentState) {
        if (state) {
            ui->label_LEDNumber->setStyleSheet(m_defaultStyle + QStringLiteral("background-color: rgb(0, 128, 0);"));
        } else {
            ui->label_LEDNumber->setStyleSheet(m_defaultStyle);
        }
        m_ledCurrentState = state;
        // Force the style to refresh
        ui->label_LEDNumber->style()->unpolish(ui->label_LEDNumber);
        ui->label_LEDNumber->style()->polish(ui->label_LEDNumber);
        ui->label_LEDNumber->update();
    }
}

void LED::readFromConfig()
{
    led_config_t *led = &gEnv.pDeviceConfig->config.leds[m_ledNumber];
    if (gEnv.pDeviceConfig->config.leds[m_ledNumber].input_num == SOURCE_HOST) {
        ui->checkBox_HostControlled->setChecked(true);
        ui->spinBox_InputNumber->setEnabled(false);
        ui->label_LEDNumber->setCursor(Qt::PointingHandCursor);
    } else {
        ui->checkBox_HostControlled->setChecked(false);
        ui->spinBox_InputNumber->setEnabled(true);
        ui->label_LEDNumber->setCursor(Qt::ArrowCursor);
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

bool LED::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->label_LEDNumber && isHostControlled()) {
        if (event->type() == QEvent::MouseButtonRelease) {
            auto *me = static_cast<QMouseEvent *>(event);
            if (me->button() == Qt::LeftButton) {
                // toggle and emit
                setLedState(!m_ledCurrentState);
                emit ledToggled(m_ledNumber, m_ledCurrentState);
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}
