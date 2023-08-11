// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#include <tusb_config.h>
#include <tusb.h>
#include "config.h"

static const char *const descriptor_string[] = {
    (const char[]){0x09, 0x04},
    STRING_VENDOR,
    STRING_PRODUCT,
    STRING_DEVICE_VERSION,
    STRING_INTERFACE_0,
    STRING_INTERFACE_1,
    STRING_INTERFACE_2,
};

uint8_t const descriptor_report_generic[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(REPORT_MOUSE)),
    TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(REPORT_GAMEPAD)),
};

uint8_t const descriptor_report_xinput[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(REPORT_MOUSE)),
};

uint8_t descriptor_configuration_generic[] = {
    DESCRIPTOR_CONFIGURATION(1),
    DESCRIPTOR_INTERFACE_HID(sizeof(descriptor_report_generic))
};

uint8_t descriptor_configuration_xinput[] = {
    DESCRIPTOR_CONFIGURATION(2),
    DESCRIPTOR_INTERFACE_HID(sizeof(descriptor_report_xinput)),
    DESCRIPTOR_INTERFACE_WEBUSB,
    DESCRIPTOR_INTERFACE_XINPUT
};

uint8_t const descriptor_bos[] = {
    TUD_BOS_DESCRIPTOR(TUD_BOS_DESC_LEN + TUD_BOS_WEBUSB_DESC_LEN + TUD_BOS_MICROSOFT_OS_DESC_LEN, 2),
    TUD_BOS_WEBUSB_DESCRIPTOR(BOS_WEBUSB, 1),
    TUD_BOS_MS_OS_20_DESCRIPTOR(BOS_WEBUSB_MS_LEN, BOS_WEBUSB_MS)
};

const tusb_desc_webusb_url_t webusb_url = {
    .bLength         = sizeof(BOS_WEBUSB_URL) + 2,
    .bDescriptorType = 3,
    .bScheme         = 1,  // HTTPS.
    .url             = BOS_WEBUSB_URL
};

// uint8_t const descriptor_bos_ms[] = {
//     U16_TO_U8S_LE(0x000A), U16_TO_U8S_LE(MS_OS_20_SET_HEADER_DESCRIPTOR), U32_TO_U8S_LE(0x06030000), U16_TO_U8S_LE(BOS_MS_LEN),
//     U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_CONFIGURATION), 0, 0, U16_TO_U8S_LE(BOS_MS_LEN-0x0A),
//     U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION), ITF_WEBUSB, 0, U16_TO_U8S_LE(BOS_MS_LEN-0x0A-0x08),
//     U16_TO_U8S_LE(0x0014), U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID),
//     'W', 'I', 'N', 'U', 'S', 'B', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     U16_TO_U8S_LE(BOS_MS_LEN-0x0A-0x08-0x08-0x14), U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY),
//     U16_TO_U8S_LE(0x0007), U16_TO_U8S_LE(0x002A),
//     'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0,
//     'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0, 'G', 0, 'U', 0, 'I', 0, 'D', 0, 's', 0, 0, 0,
//     U16_TO_U8S_LE(0x0050),
//     '{', 0, '9', 0, '7', 0, '5', 0, 'F', 0, '4', 0, '4', 0, 'D', 0, '9', 0, '-', 0,
//     '0', 0, 'D', 0, '0', 0, '8', 0, '-', 0, '4', 0, '3', 0, 'F', 0, 'D', 0, '-', 0,
//     '8', 0, 'B', 0, '3', 0, 'E', 0, '-', 0, '1', 0, '2', 0, '7', 0, 'C', 0, 'A', 0,
//     '8', 0, 'A', 0, 'F', 0, 'F', 0, 'F', 0, '9', 0, 'D', 0, '}', 0,  0 , 0,  0 , 0
// };

uint8_t const *tud_descriptor_device_cb() {
    printf("USB: tud_descriptor_device_cb\n");
    static tusb_desc_device_t descriptor_device = {DESCRIPTOR_DEVICE};
    if (config_get_os_mode() == OS_MODE_XINPUT_WIN) {
        descriptor_device.idVendor = USB_WIN_VENDOR;
        descriptor_device.idProduct = USB_WIN_PRODUCT;
    }
    if (config_get_os_mode() == OS_MODE_XINPUT_UNIX) {
        descriptor_device.idVendor = USB_UNIX_VENDOR;
        descriptor_device.idProduct = USB_UNIX_PRODUCT;
    }
    if (config_get_os_mode() == OS_MODE_GENERIC) {
        descriptor_device.idVendor = USB_GENERIC_VENDOR;
        descriptor_device.idProduct = USB_GENERIC_PRODUCT;
    }
    return (uint8_t const *) &descriptor_device;
}

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    printf("USB: tud_descriptor_configuration_cb index=0x%x\n", index);
    if (config_get_os_mode() == OS_MODE_GENERIC) {
        descriptor_configuration_generic[2] = sizeof(descriptor_configuration_generic);
        return descriptor_configuration_generic;
    } else {
        descriptor_configuration_xinput[2] = sizeof(descriptor_configuration_xinput);
        return descriptor_configuration_xinput;
    }
}

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    printf("USB: tud_hid_descriptor_report_cb\n");
    if (config_get_os_mode() == OS_MODE_GENERIC) return descriptor_report_generic;
    else return descriptor_report_xinput;
}

const uint16_t *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    printf("USB: tud_descriptor_string_cb index=0x%x\n", index);
    if (index == 0xEE && config_get_os_mode() == OS_MODE_XINPUT_WIN) {
        static uint8_t msos[18] = {MS_OS_DESCRIPTORS_MAGIC_PAYLOAD};
        return (uint16_t*)msos;
    }
    if (index >= sizeof(descriptor_string) / sizeof(descriptor_string[0])) {
        return NULL;
    }
    static uint16_t response[256];
    const char *string = descriptor_string[index];
    uint8_t i = 0;
    for (i; string[i]; i++) {
        response[i + 1] = string[i];
    }
    response[0] = TUSB_DESC_STRING << 8;  // String type.
    response[0] |= (i * 2) + 2;           // Total length.
    return response;
}

const bool tud_vendor_control_xfer_cb(
    uint8_t rhport,
    uint8_t stage,
    tusb_control_request_t const *request
) {
    printf(
        "USB: tud_vendor_control_xfer_cb stage=%i wIndex=0x%x\n",
        stage,
        request->wIndex
    );
    if (stage != CONTROL_STAGE_SETUP) return true;
    // WebUSB URL & popup.
    if (request->bRequest == BOS_WEBUSB) {
        return tud_control_xfer(
            rhport,
            request,
            (void*)(uintptr_t) &webusb_url,
            webusb_url.bLength
        );
    }
    // WebUSB in Windows.
    if (request->wIndex == 7 && request->bRequest == BOS_WEBUSB_MS) {
        static uint8_t response[] = {MS_WEBUSB_MAGIC_PAYLOAD};
        return tud_control_xfer(rhport, request, response, sizeof(response));
    }
    // WCID.
    if (
        request->wIndex == 0x0004 &&
        request->bRequest == WCID_VENDOR &&
        config_get_os_mode() == OS_MODE_XINPUT_WIN
    ) {
        static uint8_t response[40] = {MS_WCID_MAGIC_PAYLOAD};
        return tud_control_xfer(rhport, request, response, 40);
    }
    return false;
}

uint16_t tud_hid_get_report_cb(
    uint8_t instance,
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t* buffer,
    uint16_t reqlen
) {
    return 0;
}

void tud_hid_set_report_cb(
    uint8_t instance,
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t const* buffer,
    uint16_t bufsize
) {}

uint8_t const *tud_descriptor_bos_cb(void) {
    printf("USB: tud_descriptor_bos_cb\n");
    return descriptor_bos;
}

void tud_mount_cb(void) {
    printf("USB: tud_mount_cb\n");
}

void tud_umount_cb(void) {
    printf("USB: tud_umount_cb\n");
}

void tud_suspend_cb(bool remote_wakeup_en) {
    printf("USB: tud_suspend_cb\n");
}

void tud_resume_cb(void) {
    printf("USB: tud_resume_cb\n");
}
