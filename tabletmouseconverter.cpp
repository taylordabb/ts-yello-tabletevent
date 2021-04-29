#include "tabletmouseconverter.h"
#include <QTabletEvent>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QApplication>
#include <QDebug>

TabletMouseConverter::TabletMouseConverter(QObject *parent) :
    QObject(parent),
    m_pWindow(nullptr)
{

}

void TabletMouseConverter::setWindow(QObject *window)
{
    m_pWindow = window;
}

bool TabletMouseConverter::eventFilter(QObject *watched, QEvent *ev)
{
    if(ev->type() == QEvent::MouseButtonPress || ev->type() == QEvent::MouseButtonRelease ||
       ev->type() == QEvent::TabletPress || ev->type() == QEvent::TabletRelease ||
       ev->type() == QEvent::GraphicsSceneMouseMove || ev->type() == QEvent::GraphicsSceneMousePress) {

        if(ev->type() == QEvent::MouseButtonPress) {
            qDebug() << "Caught QEvent::MouseButtonPress";
        }
        else if(ev->type() == QEvent::MouseButtonRelease) {
            qDebug() << "Caught QEvent::MouseButtonRelease";
        }
        else if(ev->type() == QEvent::TabletPress) {
            qDebug() << "Caught QEvent::TabletPress";

            QTabletEvent* tabletEvent = dynamic_cast<QTabletEvent*>(ev);
            if(tabletEvent == nullptr) {
                qDebug() << "QTabletEvent* cast failed, returning";
                return QObject::eventFilter(watched, ev);
            }

            // Uncomment for position output
            /*qDebug() << "tabletEvent->pos().x():" << tabletEvent->pos().x();
            qDebug() << "tabletEvent->pos().y():" << tabletEvent->pos().y();
            qDebug() << "tabletEvent->globalPos().x()" << tabletEvent->globalPos().x();
            qDebug() << "tabletEvent->globalPos().y()" << tabletEvent->globalPos().y();*/


            // Generating a matching QMouseEvent like this results in only the first-clicked QML Item ever
            // receiving subsequent clicks. I.e. in touchTest example, click red square first, and *all*
            // clicks afterwards anywhere else on the screen will land on the red square.
            // To get around this, signal to QML the tabletPress, and then let some top-level MouseArea
            // receive the first "click" such that it can manually set mouse.accepted = false, and let the
            // mouse event fallthrough to Items below.
            //QMouseEvent* mouseEventPress = new QMouseEvent(QEvent::MouseButtonPress, tabletEvent->posF(), tabletEvent->globalPosF(), Qt::LeftButton, Qt::LeftButton, tabletEvent->modifiers());
            //QCoreApplication::postEvent(m_pWindow, mouseEventPress);
            emit tabletPress(tabletEvent->globalPos().x(), tabletEvent->globalPos().y());

            return true; // May as well eat the tablet event
        }
        else if(ev->type() == QEvent::TabletRelease) {
            qDebug() << "Caught QEvent::TabletRelease";

            QTabletEvent* tabletEvent = dynamic_cast<QTabletEvent*>(ev);
            if(tabletEvent == nullptr) {
                qDebug() << "Release cast failed, returning";
                return QObject::eventFilter(watched, ev);
            }

            // Generating a matching QMouseEvent like this results in only the first-clicked QML Item ever
            // receiving subsequent clicks. I.e. in touchTest example, click red square first, and *all*
            // clicks afterwards anywhere else on the screen will land on the red square.
            // To get around this, signal to QML the tabletPress, and then let some top-level MouseArea
            // receive the first "click" such that it can manually set mouse.accepted = false, and let the
            // mouse event fallthrough to Items below.
            //QMouseEvent* mouseEventRelease = new QMouseEvent(QEvent::MouseButtonRelease, tabletEvent->posF(), tabletEvent->globalPosF(), Qt::LeftButton, Qt::LeftButton, tabletEvent->modifiers());
            //QCoreApplication::postEvent(watched, mouseEventRelease);
            emit tabletRelease(tabletEvent->globalPos().x(), tabletEvent->globalPos().y());

            return true; // May as well eat the tablet event
        }
        else {
            qDebug() << "Caught" << ev->type();
        }
        return false;
    }

    return QObject::eventFilter(watched, ev); // Not anything we were interested in catching...
}

void TabletMouseConverter::fireMouseClickEvent(int x, int y)
{
    QMouseEvent mouseEventPress(QEvent::MouseButtonPress, QPointF(x, y), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(m_pWindow, &mouseEventPress);
    QMouseEvent mouseEventRelease(QEvent::MouseButtonRelease, QPointF(x, y), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(m_pWindow, &mouseEventRelease);
}
