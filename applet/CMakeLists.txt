set(deal_applet_SRCS dealapplet.cpp)

kde4_add_plugin(plasma_applet_deal ${deal_applet_SRCS})
target_link_libraries(plasma_applet_deal
	${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS plasma_applet_deal
		DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-deal.desktop
	      DESTINATION ${SERVICES_INSTALL_DIR})
