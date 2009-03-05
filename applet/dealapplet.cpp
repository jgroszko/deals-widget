#include "dealapplet.h"

#include <QPainter>
#include <QPixmap>
#include <QGraphicsLinearLayout>
#include <KNotification>

DealApplet::DealApplet(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
     m_Icon = new Plasma::IconWidget();
     m_Label = new Plasma::Label();

    setBackgroundHints(DefaultBackground);
    setAspectRatioMode(Plasma::FixedSize);
    setMinimumSize(QSizeF(400, 200));
}

DealApplet::~DealApplet()
{
     delete(m_Icon);
     delete(m_Label);
}

void DealApplet::init()
{
    Plasma::DataEngine* dealEngine = dataEngine("deal");

    if(dealEngine->isValid())
	 dealEngine->connectSource("Brociety", this, 600, Plasma::NoAlignment);
    else
	 setFailedToLaunch(true, "Deal engine was not valid.");

    QGraphicsLinearLayout* mainLayout = new QGraphicsLinearLayout(Qt::Horizontal);

    mainLayout->addItem(m_Icon);
    mainLayout->addItem(m_Label);

    setLayout(mainLayout);
}

void DealApplet::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
     Q_UNUSED(source);

     QPixmap pixmap;
     pixmap.loadFromData(data["pixmap"].toByteArray());

     if(m_oldPubDate != data["pubDate"].toString())
     {
	  QPixmap pixmap_small = pixmap.scaled(QSize(64, 64), Qt::KeepAspectRatio);

	  KComponentData componentData("deal");
	  KNotification* event = new KNotification("newItem");
	  event->setComponentData(componentData);
	  event->setText("$" + data["priceCurrent"].toString() + " - " + data["name"].toString());
	  event->setPixmap(pixmap_small);
     
	  event->sendEvent();
     
	  m_oldPubDate = data["pubDate"].toString();
     }

     m_Label->setText("<h2>" + data["name"].toString() + "</h2>" +
		      "<h3>$" + data["priceCurrent"].toString() + "</h3>" +
		      "<p>" + data["listDescription"].toString() + "</p>");

     m_Icon->setIcon(pixmap);
     m_Icon->setMinimumSize(m_Icon->sizeFromIconSize(128));

     update();
}

void DealApplet::paintInterface(QPainter *p,
				const QStyleOptionGraphicsItem *, const QRect &)
{
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);
}

#include "dealapplet.moc"
