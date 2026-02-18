
#ifndef LEDRGB_H
#define LEDRGB_H


#include <QObject>
#include <QListWidgetItem>


class LedRGB : public QObject
{
    Q_OBJECT
public:
    explicit LedRGB(QObject *parent = nullptr);
    ~LedRGB();

    QListWidgetItem *item() const; //?
    bool isSelected() const;

    QColor color() const;
    void setColor(const QColor &color);

    QIcon icon() const;
    void setIcon(const QIcon &icon, const QColor &color);

    QString text() const;
    void setText(const QString &text);

    void setButtonNumber(int num);
    int buttonNumber() const;

    void setButtonInverted(bool inverted);
    int buttonInverted() const;

    void setButtonState(bool state);

signals:
    //void ledSelected();

private:
    int m_number;
    bool m_buttonInverted;
    bool m_buttonState;

    QColor m_color;
    void updateLedColor();

    //QScopedPointer<QListWidgetItem> m_item;
    QListWidgetItem *m_item;

    // optimization
    static const QIcon m_baseIcon;

    static const QString m_buttonOffPixPath;
    static const QString m_buttonOnPixPath;
};

#endif // LEDRGB_H
