#ifndef LEDFUNCTION_H
#define LEDFUNCTION_H

#include <QWidget>

namespace Ui {
class LedFunction;
}

class LedFunction : public QWidget
{
    Q_OBJECT

public:
    explicit LedFunction(QWidget *parent = nullptr);
    ~LedFunction();

    void setLogButton(int num);
    int logButtonNum() const;

    void setInverted(bool inverted);
    bool Inverted() const;

signals:
    void logButtonChanged(int button);
    void invertChanged(bool inverted);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::LedFunction *ui;
};

#endif // LEDFUNCTION_H
