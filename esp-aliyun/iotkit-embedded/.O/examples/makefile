MODULE_NAME := examples
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

DEPENDS             := src/ref-impl/hal
DEPENDS             += src/ref-impl/tls

HDR_REFS            += src/infra
HDR_REFS            += src/services

LDFLAGS             := -Bstatic
LDFLAGS             += -liot_sdk

LDFLAGS             += -liot_hal
CFLAGS              := $(filter-out -ansi,$(CFLAGS))
ifneq (,$(filter -D_PLATFORM_IS_WINDOWS_,$(CFLAGS)))
LDFLAGS             += -lws2_32
CFLAGS              := $(filter-out -DCOAP_COMM_ENABLED,$(CFLAGS))
endif
ifneq (,$(filter -DSUPPORT_ITLS,$(CFLAGS)))
LDFLAGS             += -litls
else
LDFLAGS             += -liot_tls
endif

SRCS_mqtt-example-rrpc          := app_entry.c mqtt/mqtt_example_rrpc.c
SRCS_mqtt-example               := app_entry.c mqtt/mqtt_example.c
SRCS_mqtt-example-multithread   := app_entry.c mqtt/mqtt_example_multithread.c
SRCS_http2-example-uploadfile   := http2/http2_example_uploadfile.c app_entry.c
SRCS_http2-example              := http2/http2_example_stream.c app_entry.c
SRCS_coap-example               := coap/coap_example.c app_entry.c
SRCS_http-example               := http/http_example.c app_entry.c
SRCS_ota-example-mqtt           := ota/ota_example_mqtt.c
SRCS_linkkit-example-cota       := app_entry.c linkkit/linkkit_example_cota.c
SRCS_linkkit-example-sched      := app_entry.c cJSON.c linkkit/linkkit_example_sched.c
SRCS_linkkit-example-solo       := app_entry.c cJSON.c linkkit/linkkit_example_solo.c
SRCS_linkkit-example-countdown  := app_entry.c cJSON.c linkkit/linkkit_example_cntdown.c
SRCS_linkkit-example-gw         := app_entry.c cJSON.c linkkit/linkkit_example_gateway.c

# Syntax of Append_Conditional
# ---
#
# $(call Append_Conditional, TARGET, \  <-- Operated Variable
#   member1 member2 ...            , \  <-- Appended Members
#   switch1 switch2 ...            , \  <-- All These Switches are Defined
#   switch3 switch4 ...)                <-- All These Switches are Not Defined (Optional)


$(call Append_Conditional, TARGET, mqtt-example-rrpc,           MQTT_COMM_ENABLED)
$(call Append_Conditional, TARGET, mqtt-example,                MQTT_COMM_ENABLED)
$(call Append_Conditional, TARGET, mqtt-example-multithread,    MQTT_COMM_ENABLED)

$(call Append_Conditional, LDFLAGS, \
    -litls \
    -lid2client \
    -lkm \
    -lplat_gen \
    -lalicrypto \
    -lmbedcrypto \
, \
SUPPORT_ITLS, \
SUPPORT_TLS)

$(call Append_Conditional, TARGET, coap-example,                COAP_COMM_ENABLED)
$(call Append_Conditional, TARGET, http-example,                HTTP_COMM_ENABLED)

$(call Append_Conditional, TARGET, http2-example,               HTTP2_COMM_ENABLED)
$(call Append_Conditional, TARGET, http2-example-uploadfile,    HTTP2_COMM_ENABLED FS_ENABLED)

$(call Append_Conditional, TARGET, ota-example-mqtt,            OTA_ENABLED MQTT_COMM_ENABLED)
$(call Append_Conditional, TARGET, linkkit-example-cota, \
    OTA_ENABLED DEVICE_MODEL_ENABLED, \
    DEPRECATED_LINKKIT \
)

$(call Append_Conditional, TARGET, linkkit-example-solo,        DEVICE_MODEL_ENABLED, DEVICE_MODEL_GATEWAY)
$(call Append_Conditional, TARGET, linkkit-example-countdown,   DEVICE_MODEL_ENABLED, DEVICE_MODEL_GATEWAY)
ifneq (Darwin,$(shell uname))
$(call Append_Conditional, TARGET, linkkit-example-sched,       DEVICE_MODEL_ENABLED, DEVICE_MODEL_GATEWAY)
endif
$(call Append_Conditional, TARGET, linkkit-example-gw,          DEVICE_MODEL_ENABLED  DEVICE_MODEL_GATEWAY)

# Clear All Above when Build for Windows
#
ifneq (,$(filter -D_PLATFORM_IS_WINDOWS_,$(CFLAGS)))
    TARGET          := mqtt-example linkkit-example-solo
endif

env:
	@echo ""
	@printf -- "-----------------------------------------------------------------\n"
	@$(foreach var,$(SHOW_ENV_VARS),$(call Dump_Var,$(var)))
	@printf -- "-----------------------------------------------------------------\n"
	@echo ""

include $(RULE_DIR)/rules.mk
