include_directories(${Boost_INCLUDE_DIR} ${KDEPIMLIBS_INCLUDE_DIR})

set(QT_USE_QTXMLPATTERNS 1)

include(${QT_USE_FILE})

set(deal_engine_SRCS
    dealengine.cpp
)

kde4_add_plugin(plasma_engine_deal ${deal_engine_SRCS})
target_link_libraries(plasma_engine_deal ${KDE4_PLASMA_LIBS}
					 ${KDE4_KDECORE_LIBS}
					 ${KDE4_KIO_LIBS}
					 ${QT_LIBRARIES})

install(TARGETS plasma_engine_deal DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-deal.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
