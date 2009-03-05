#ifndef DEAL_HEADER
#define DEAL_HEADER

#include <plasma/plasma.h>
#include <Plasma/Applet>
#include <Plasma/DataEngine>
#include <Plasma/Svg>
#include <plasma/widgets/label.h>
#include <plasma/widgets/iconwidget.h>
#include <KComponentData>

class QGraphicsImageWidget;

class DealApplet : public Plasma::Applet
{
    Q_OBJECT
 public:
    // Basic create/destroy
    DealApplet(QObject *parent, const QVariantList &args);
    ~DealApplet();

    // The paintInterface procedure paints the applet to the screen
    void paintInterface(QPainter *painter,
			const QStyleOptionGraphicsItem *option,
			const QRect& contentsRect);
    void init();

public slots:
    void dataUpdated(const QString &name, const Plasma::DataEngine::Data &data);

protected:
    QString m_oldPubDate;

    Plasma::IconWidget* m_Icon;
    Plasma::Label* m_Label;

};

K_EXPORT_PLASMA_APPLET(deal, DealApplet)

#endif
