#ifndef TABLETMOUSECONVERTER_H
#define TABLETMOUSECONVERTER_H

#include <QObject>

// Helper class to convert TabletEvents into MouseEvents
class TabletMouseConverter : public QObject
{
    Q_OBJECT
public:
    explicit TabletMouseConverter(QObject *parent = 0);

    void setWindow(QObject* window);

    virtual bool eventFilter(QObject *watched, QEvent *ev);

    Q_INVOKABLE void fireMouseClickEvent(int x, int y) ;

signals:
    void tabletPress(int tabletX, int tabletY);
    void tabletRelease(int tabletX, int tabletY);
    void tabletMove(int x, int y);

private:
    QObject* m_pWindow;
};

#endif // TABLETMOUSECONVERTER_H
