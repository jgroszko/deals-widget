#ifndef DEAL_HEADER
#define DEAL_HEADER

#include <plasma/plasma.h>
#include <Plasma/Applet>
#include <Plasma/DataEngine>
#include <Plasma/Svg>
#include <plasma/widgets/label.h>
#include <plasma/widgets/iconwidget.h>
#include <KComponentData>

#include "ui_configuration.h"

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
    void createConfigurationInterface(KConfigDialog *parent);

    void openLink();

protected slots:
    void configAccepted();

protected:
    Plasma::DataEngine* m_engine;
    QString m_site;

    QString m_oldPubDate;
    QString m_link;

    Plasma::IconWidget* m_Icon;
    Plasma::Label* m_Label;

    Ui::DealConfig configUi;

};

K_EXPORT_PLASMA_APPLET(deal, DealApplet)

#endif
