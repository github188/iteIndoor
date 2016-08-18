set(PROJ_NAME ${CMAKE_PROJECT_NAME})

if (DEFINED CFG_BOOTLOADER_ENABLE)
    set(CMAKE_PROJECT_NAME bootloader)
    add_subdirectory(../bootloader ../bootloader)
endif()

set(CMAKE_PROJECT_NAME ${PROJ_NAME})

add_definitions(
    -DCFG_SNTP_PORT=${CFG_SNTP_PORT}
    -DCFG_SNTP_UPDATE_DELAY=${CFG_SNTP_UPDATE_DELAY}
    -DCFG_WEBSERVER_PORT=${CFG_WEBSERVER_PORT}
    -DCFG_WEBSERVER_PORT_STR="${CFG_WEBSERVER_PORT}"
    -DCFG_HW_VERSION="${CFG_HW_VERSION}"
    )

if (DEFINED CFG_GUARD_ENABLE)
    add_definitions(
        -DCFG_GUARD_ENABLE
        -DCFG_GPIO_GUARD_SENSORS=${CFG_GPIO_GUARD_SENSORS}
        )
endif()

if (DEFINED CFG_CHECK_FILES_CRC_ON_BOOTING)
    add_definitions(
        -DCFG_CHECK_FILES_CRC_ON_BOOTING
        )
endif()

if (DEFINED CFG_UDP_HEARTBEAT)
    add_definitions(
        -DCFG_UDP_HEARTBEAT
        -DCFG_UDP_HEARTBEAT_PORT=${CFG_UDP_HEARTBEAT_PORT}
        )
endif()

if (DEFINED CFG_CARDLIST_ENABLE)
    add_definitions(
        -DCFG_CARDLIST_ENABLE
        )
endif()

if (DEFINED CFG_SCREENSHOT_ENABLE)
    add_definitions(
        -DCFG_SCREENSHOT_ENABLE
        )
endif()

if (DEFINED CFG_AUTO_ANSWER)
    add_definitions(
        -DCFG_AUTO_ANSWER
        )
endif()

if (DEFINED CFG_REGISTER_SIP_SERVER)
    add_definitions(
        -DCFG_REGISTER_SIP_SERVER
        )
endif()

if (DEFINED CFG_MP3_RING)
    add_definitions(
        -DCFG_MP3_RING
        )
endif()
include_directories(
	${PROJECT_SOURCE_DIR}/sdk/include/leaf
	${PROJECT_SOURCE_DIR}/sdk/driver/mmp/include
    ${PROJECT_SOURCE_DIR}/sdk/share/audio_mgr
    ${PROJECT_SOURCE_DIR}/sdk/share/ffmpeg
    ${PROJECT_SOURCE_DIR}/sdk/share/ffmpeg/libavcodec
    ${PROJECT_SOURCE_DIR}/sdk/share/ffmpeg/libavformat
    ${PROJECT_SOURCE_DIR}/sdk/share/ffmpeg/libavdevice
    ${PROJECT_SOURCE_DIR}/sdk/share/ffmpeg/libavutil
    ${PROJECT_SOURCE_DIR}/sdk/share/ffmpeg/libswresample
    ${PROJECT_SOURCE_DIR}/sdk/share/ffmpeg/libswscale
    ${PROJECT_SOURCE_DIR}/sdk/driver/itv/ferrari_foundry/
    ${PROJECT_SOURCE_DIR}/sdk/driver/itv/ferrari_foundry/core
    ${PROJECT_SOURCE_DIR}/sdk/share/microhttpd
    ${PROJECT_SOURCE_DIR}/sdk/share/microhttpd/include
    ${PROJECT_SOURCE_DIR}/sdk/share/sntp
    ${PROJECT_SOURCE_DIR}/sdk/share/wifi_mgr/        
    )

#if (DEFINED CFG_RS485_0_ENABLE)
#    include_directories(
#        ${PROJECT_SOURCE_DIR}/sdk/share/modbus
#        )
#endif()

add_executable(${CMAKE_PROJECT_NAME}
function_table.c
main.c
scene.c
scene.h
)

#if (DEFINED CFG_RS485_0_ENABLE)
#    add_executable(${CMAKE_PROJECT_NAME}
#        modbus_ctrl.c
#        )
#endif()

#add by chenbh 
include_directories(
include
storage
driver
logic
gui
)
add_executable(${CMAKE_PROJECT_NAME}
storage/language/storage_language.c
storage/language/storage_language.h
storage/timer/storage_timer.c
storage/timer/storage_timer.h
storage/alarm/storage_alarm.c
storage/alarm/storage_alarm.h
storage/call/storage_call.c
storage/call/storage_call.h
storage/comm/storage_comm.c
storage/comm/storage_comm.h
storage/comm/storage_syssave.c
storage/comm/storage_syssave.h
storage/comm/storage_path.h
storage/comm/storage_types.h
storage/ipmodule/storage_ipmodule.c
storage/ipmodule/storage_ipmodule.h
storage/jiadian/storage_jd.c
storage/jiadian/storage_jd.h
storage/jrly/storage_jrly.c
storage/jrly/storage_jrly.h
storage/lyly/storage_lyly.c
storage/lyly/storage_lyly.h
storage/message/storage_message.c
storage/message/storage_message.h
storage/photo/storage_photo.c
storage/photo/storage_photo.h
storage/rtsp/storage_rtsp.c
storage/rtsp/storage_rtsp.h
storage/yuyue/storage_yuyue.c
storage/yuyue/storage_yuyue.h
storage/storage_include.h
driver/hw_com_api.c
driver/hw_com_api.h
driver/hw_ioctrl_api.c
driver/hw_ioctrl_api.h
driver/driver_include.h
logic/media/logic_media.c
logic/media/logic_media.h
logic/ipmodule/Logic_ipmodule_alarm.c
logic/ipmodule/logic_ipmodule_call.c
logic/ipmodule/logic_ipmodule_jd.c
logic/ipmodule/logic_ipmodule_set.c
logic/ipmodule/logic_ipmodule.h
logic/call/logic_call.c
logic/call/logic_call.h
logic/call/logic_interphone.c
logic/call/logic_interphone.h
logic/call/logic_monitor.c
logic/call/logic_monitor.h
logic/call/loigc_rtsp_monitor.c
logic/call/loigc_rtsp_monitor.h
logic/alarm/logic_alarm.c
logic/alarm/logic_alarm.h
logic/alarm/logic_alarm_net.c
logic/alarm/logic_alarm_net.h
logic/netcom/logic_getip.c
logic/netcom/logic_getip.h
logic/netcom/logic_net_param.c
logic/netcom/logic_net_param.h
logic/netcom/logic_packet_list.c
logic/netcom/logic_packet_list.h
logic/netcom/logic_send_event.c
logic/netcom/logic_send_event.h
logic/netcom/logic_udp_comm.c
logic/netcom/logic_udp_comm.h
logic/netcom/logic_distribute.c
logic/netcom/logic_distribute.h
logic/set/logic_wlmqj_set.c
logic/set/logic_wlmqj_set.h
logic/linkage/logic_linkage.c
logic/linkage/logic_linkage.h
logic/linkage/logic_sys_arbitration.c
logic/linkage/logic_sys_arbitration.h
logic/dianti/logic_dianti.c
logic/dianti/logic_dianti.h
logic/comm/logic_pubcmd.c
logic/comm/logic_pubcmd.h
logic/comm/logic_appinit.c
logic/comm/logic_appinit.h
logic/comm/logic_model_func.c
logic/comm/logic_model_func.h
logic/timer/logic_timer.c
logic/timer/logic_timer.h
logic/timer/logic_time.c
logic/timer/logic_time.h
logic/yuyue/logic_yuyue.c
logic/yuyue/logic_yuyue.h
logic/jiadian/logic_jiadian.c
logic/jiadian/logic_jiadian.h
logic/message/logic_message.c
logic/message/logic_message.h
logic/logic_include.h
gui/mainmenu/layer_mainmenu.c
gui/mainmenu/layer_mainmenu.h
gui/cleanscreen/layer_cleanscreen.c
gui/cleanscreen/layer_cleanscreen.h
gui/information/layer_information.c
gui/information/layer_information.h
gui/photomsg/layer_photomsg.c
gui/photomsg/layer_photomsg.h
gui/recorder/layer_recorder.c
gui/recorder/layer_recorder.h
gui/elevator/layer_elevator.c
gui/elevator/layer_elevator.h
gui/call/layer_call.h
gui/call/layer_call.c
gui/call/layer_becall.c
gui/call/layer_monitor_search.c
gui/call/layer_monitor.c
gui/comm/au_model_func.c	
gui/set/layer_set_menu.c
gui/set/layer_set_bottom_return.c   
gui/set/layer_set_personality_desk.c
gui/set/layer_set_personality_photoframe.c
gui/set/layer_set_ring.c
gui/set/layer_set_volume.c
gui/set/layer_set_no_disturb.c
gui/set/layer_set_numkeybord.c
gui/set/layer_set_password.c
gui/set/layer_set_link.c
gui/set/layer_set_language.c 
gui/set/layer_set_data_time.c 
gui/set/layer_set_sysinfo.c
gui/set/layer_set_storage.c
gui/set/layer_set_ip_protocol_host.c
gui/set/layer_set_ip_protocol_fenji.c
gui/set/user/layer_set_user.c
gui/set/user/layer_set_alarm_num.c
gui/set/user/layer_set_alarm_area_list.c
gui/set/user/layer_set_alarm_remote.c
gui/set/project/layer_set_alarm.c
gui/set/project/layer_set_netparam.c
gui/set/project/layer_set_extmodule.c
gui/set/project/layer_set_devno.c
gui/set/project/layer_set_project.c
gui/set/project/layer_set_diantiIP.c
gui/set/project/layer_set_rtsp.c
gui/set/project/layer_set_alarm_single_attr.c
gui/set/project/layer_set_netdoor.c
gui/set/project/layer_set_netdoor_netparam.c
gui/set/project/layer_set_netdoor_card.c
gui/set/project/layer_set_netdoor_lock.c
gui/set/project/layer_set_netdoor_other.c
gui/set/project/layer_set_netdoor_sysinfo.c
gui/set/msg/layer_msg_fail_hint_success.c
gui/set/msg/layer_msg_link_out_time.c
gui/set/layer_set.h
    )

# files
configure_file(${PROJECT_SOURCE_DIR}/sdk/share/wifi_hostapd/hostapd.conf.in ${CMAKE_BINARY_DIR}/data/public/hostapd.conf COPYONLY)
if (DEFINED CFG_NET_WIFI_WPA_ENABLE)
configure_file(
    ${PROJECT_SOURCE_DIR}/sdk/share/wifi_hostapd/hostapd_wpa.conf.in
    ${CMAKE_BINARY_DIR}/data/public/hostapd.conf
    NEWLINE_STYLE UNIX
    )
else()
configure_file(
    ${PROJECT_SOURCE_DIR}/sdk/share/wifi_hostapd/hostapd.conf.in
    ${CMAKE_BINARY_DIR}/data/public/hostapd.conf
    NEWLINE_STYLE UNIX
    )
endif()

file(COPY
    ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/res
    ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/${CFG_ITU_PATH}/doorbell_indoor.itu
    DESTINATION ${CMAKE_BINARY_DIR}/data/private
    )

# codec
if ($ENV{CFG_PLATFORM} STREQUAL openrtos)
    configure_file(${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/codec/lib/sm32/mp3.codecs ${CMAKE_BINARY_DIR}/data/private/codec/mp3.codecs COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/codec/lib/sm32/wave.codecs ${CMAKE_BINARY_DIR}/data/private/codec/wave.codecs COPYONLY)
endif()

# crc
if (DEFINED CFG_CHECK_FILES_CRC_ON_BOOTING)
    file(REMOVE ${CMAKE_BINARY_DIR}/data/private/backup/${CFG_PUBLIC_DRIVE}/ite_crc.dat)
    execute_process(COMMAND makecrc -c ${CMAKE_BINARY_DIR}/data/public/ite_crc.dat -i ${CMAKE_BINARY_DIR}/data/private/backup/${CFG_PUBLIC_DRIVE})
    execute_process(COMMAND makecrc -v ${CMAKE_BINARY_DIR}/data/public/ite_crc.dat -i ${CMAKE_BINARY_DIR}/data/private/backup/${CFG_PUBLIC_DRIVE})
    file(COPY ${CMAKE_BINARY_DIR}/data/public/ite_crc.dat DESTINATION ${CMAKE_BINARY_DIR}/data/private/backup/${CFG_PUBLIC_DRIVE})
endif()

# font
file(COPY
    ${PROJECT_SOURCE_DIR}/data/font/${CFG_FONT_FILENAME}
    DESTINATION ${CMAKE_BINARY_DIR}/data/private/font
    )

# web
file(GLOB files ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/web/*.*)
foreach (src ${files})
    string(REPLACE "${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/web/" "${CMAKE_BINARY_DIR}/data/private/web/" dest ${src})
    configure_file(${src} ${dest} COPYONLY)
endforeach()

# build
include(${PROJECT_SOURCE_DIR}/sdk/build.cmake)
