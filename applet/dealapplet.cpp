#include "dealapplet.h"

#include <QPainter>
#include <QPixmap>
#include <QGraphicsLinearLayout>
#include <KConfigDialog>
#include <KNotification>
#include <KNotifyConfigWidget>
#include <KRun>

DealApplet::DealApplet(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
     m_Icon = new Plasma::IconWidget();
     m_Label = new Plasma::Label();

    setBackgroundHints(DefaultBackground);
    setAspectRatioMode(Plasma::FixedSize);
    setMinimumSize(QSizeF(400, 200));

    setHasConfigurationInterface(true);
}

DealApplet::~DealApplet()
{
     delete(m_Icon);
     delete(m_Label);
}

void DealApplet::init()
{
     KConfigGroup cg = config();
     m_site = cg.readEntry("site", "Brociety");

     m_engine = dataEngine("deal");

     if(m_engine->isValid())
	  m_engine->connectSource(m_site, this, 60000);
     else
	  setFailedToLaunch(true, "Deal engine was not valid.");

     QGraphicsLinearLayout* mainLayout = new QGraphicsLinearLayout(Qt::Horizontal);

     mainLayout->addItem(m_Icon);
     mainLayout->addItem(m_Label);

     connect(m_Icon, SIGNAL(clicked()), this, SLOT(openLink()));

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
	  event->setActions(QStringList() << "View");
     
	  connect(event, SIGNAL(activated(unsigned int)), this, SLOT(openLink()));

	  event->sendEvent();
     
	  m_oldPubDate = data["pubDate"].toString();
     }

     m_Label->setText("<h2>" + data["name"].toString() + "</h2>" +
		      "<h3>$" + data["priceCurrent"].toString() + "</h3>" +
		      "<p>" + data["listDescription"].toString() + "</p>");

     m_link = data["link"].toString();

     m_Icon->setIcon(pixmap);
     m_Icon->setMinimumSize(m_Icon->sizeFromIconSize(128));

     update();
}

void DealApplet::openLink()
{
     if(m_link != QString())
	  KRun::runUrl(m_link, "text/html", 0L);
}

void DealApplet::paintInterface(QPainter *p,
				const QStyleOptionGraphicsItem *, const QRect &)
{
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);
}

void DealApplet::createConfigurationInterface(KConfigDialog *parent)
{
     connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
     connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));

     QWidget *configWidget = new QWidget();
     configUi.setupUi(configWidget);

     QString source;
     int selectedIndex = 0;
     foreach(source, m_engine->sources())
     {
	  if(source == m_site)
	       selectedIndex = configUi.siteCombo->count();

	  configUi.siteCombo->addItem(source);
     }

     configUi.siteCombo->setCurrentIndex(selectedIndex);

     parent->addPage(configWidget, i18n("General"), icon());

     KNotifyConfigWidget* notifyWidget = new KNotifyConfigWidget(parent);
     notifyWidget->setApplication("deal");
     
     parent->addPage(notifyWidget, i18n("Notifications"), "preferences-desktop-notification");
}

void DealApplet::configAccepted()
{
     QString new_site = configUi.siteCombo->currentText();

     KConfigGroup cg = config();

     if(m_site != new_site)
     {
	  cg.writeEntry("site", new_site);

	  m_engine->disconnectSource(m_site, this);

	  m_engine->connectSource(new_site, this, 60000);

	  m_site = new_site;

	  emit configNeedsSaving();
     }
}

#include "dealapplet.moc"
