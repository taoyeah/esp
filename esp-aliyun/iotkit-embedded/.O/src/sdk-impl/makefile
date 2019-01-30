MODULE_NAME := src/sdk-impl
ALL_SUB_DIRS := examples src/infra/log src/infra/system src/infra/utils src/protocol/alcs src/protocol/coap/cloud src/protocol/coap/local src/protocol/http src/protocol/http2 src/protocol/mqtt src/ref-impl/hal src/ref-impl/tls src/sdk-impl src/services/awss src/services/dev_bind src/services/http2_stream src/services/linkkit/cm src/services/linkkit/dev_reset src/services/linkkit/dm src/services/mdal/mal src/services/mdal/sal src/services/ota src/services/shadow src/services/subdev src/tools/linkkit_tsl_convert
AR := xtensa-esp32-elf-ar
CC := xtensa-esp32-elf-gcc
CFLAGS :=  -Iexamples -DAWSS_SUPPORT_ADHA -DAWSS_SUPPORT_AHA -DAWSS_SUPPORT_APLIST -DAWSS_SUPPORT_PHONEASAP -DAWSS_SUPPORT_ROUTER -DAWSS_SUPPORT_SMARTCONFIG -DAWSS_SUPPORT_SMARTCONFIG_WPS -DAWSS_SUPPORT_ZEROCONFIG -DBOARD_ESP32 -DCOAP_SERV_MULTITHREAD -DCONFIG_GUIDER_AUTH_TIMEOUT=500 -DCONFIG_HTTP_AUTH_TIMEOUT=500 -DCONFIG_MID_HTTP_TIMEOUT=500 -DCONFIG_MQTT_RX_MAXLEN=1200 -DCONFIG_MQTT_TX_MAXLEN=640 -DDEVICE_MODEL_ENABLED -DDEV_BIND_ENABLED -DESPOS_FOR_ESP32 -DFORCE_SSL_VERIFY -DMQTT_COMM_ENABLED -DMQTT_DIRECT -DOTA_ENABLED -DOTA_SIGNAL_CHANNEL=1 -DSUPPORT_TLS -DWIFI_PROVISION_ENABLED -DXT_USE_THREAD_SAFE_CLIB=0 -Iexamples/linkkit -Itests -Os -Wl,-static -fdata-sections -ffunction-sections -fno-inline-functions -mlongcalls -u call_user_start
COMP_LIB := libiot_sdk.a
COMP_LIB_COMPONENTS := src/infra/log src/infra/system src/infra/utils src/protocol/coap/local src/protocol/mqtt src/sdk-impl src/services/awss src/services/dev_bind src/services/linkkit/cm src/services/linkkit/dev_reset src/services/linkkit/dm src/services/ota
CONFIG_VENDOR := esp32
CROSS_CANDIDATES := AR CC CXX LD OBJCOPY STRIP
CROSS_PREFIX := xtensa-esp32-elf-
CXX := xtensa-esp32-elf-g++
EXPORT_DIR := include
IMPORT_DIR := prebuilt
INSTALL_DIR := /home/taoyeah/esp/esp-aliyun/iotkit-embedded/.O/usr
INSTALL_LIB_DIR := /home/taoyeah/esp/esp-aliyun/iotkit-embedded/.O/usr/lib
KERNEL_DIR := 
LD := xtensa-esp32-elf-ld
LDFLAGS := 
MAKE_ENV_VARS := 
NOEXEC_CMAKE_DIRS := src/services/linkkit/dm tests
OBJCOPY := xtensa-esp32-elf-objcopy
PACKAGE_DIR := /home/taoyeah/esp/esp-aliyun/iotkit-embedded/packages
RULE_DIR := /home/taoyeah/esp/esp-aliyun/iotkit-embedded/build-rules
STRIP := xtensa-esp32-elf-strip
SYSROOT_INC := /home/taoyeah/esp/esp-aliyun/iotkit-embedded/.O/usr/include
TOP_DIR := /home/taoyeah/esp/esp-aliyun/iotkit-embedded
WIN32_CMAKE_SKIP := coap-example linkkit_tsl_convert
COMP_LIB_FILES :=  libiot_utils.a  libiot_log.a  libiot_system.a  libsdk_impl.a  libiot_mqtt.a  libiot_ota.a  libiot_cm.a  libiot_dm.a  libiot_reset.a  libiot_coap_local.a  libiot_awss.a  libiot_bind.a

include $(RULE_DIR)/settings.mk
include $(CONFIG_TPL)

all:

LIBA_TARGET     := libsdk_impl.a

HDR_REFS        := src/infra
HDR_REFS        += src/protocol/mqtt
HDR_REFS        += src/services/linkkit/dm

env:
	@echo ""
	@printf -- "-----------------------------------------------------------------\n"
	@$(foreach var,$(SHOW_ENV_VARS),$(call Dump_Var,$(var)))
	@printf -- "-----------------------------------------------------------------\n"
	@echo ""

include $(RULE_DIR)/rules.mk
