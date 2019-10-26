/***************************************************************************//**
* \file USBTMC_descr.c
* \version 3.20
*
* \brief
*  This file contains the USB descriptors and storage.
*
********************************************************************************
* \copyright
* Copyright 2008-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBTMC_pvt.h"


/*****************************************************************************
*  User supplied descriptors.  If you want to specify your own descriptors,
*  define USER_SUPPLIED_DESCRIPTORS below and add your descriptors.
*****************************************************************************/
/* `#START USER_DESCRIPTORS_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
*  USB Customizer Generated Descriptors
***************************************/

#if !defined(USER_SUPPLIED_DESCRIPTORS)
/*********************************************************************
* Device Descriptors
*********************************************************************/
const uint8 CYCODE USBTMC_DEVICE0_DESCR[18u] = {
/* Descriptor Length                       */ 0x12u,
/* DescriptorType: DEVICE                  */ 0x01u,
/* bcdUSB (ver 2.0)                        */ 0x00u, 0x02u,
/* bDeviceClass                            */ 0x00u,
/* bDeviceSubClass                         */ 0x00u,
/* bDeviceProtocol                         */ 0x00u,
/* bMaxPacketSize0                         */ 0x08u,
/* idVendor                                */ 0xB4u, 0x04u,
/* idProduct                               */ 0x51u, 0x80u,
/* bcdDevice                               */ 0x03u, 0x00u,
/* iManufacturer                           */ 0x01u,
/* iProduct                                */ 0x02u,
/* iSerialNumber                           */ 0x80u,
/* bNumConfigurations                      */ 0x01u
};
/*********************************************************************
* Config Descriptor  
*********************************************************************/
const uint8 CYCODE USBTMC_DEVICE0_CONFIGURATION0_DESCR[39u] = {
/*  Config Descriptor Length               */ 0x09u,
/*  DescriptorType: CONFIG                 */ 0x02u,
/*  wTotalLength                           */ 0x27u, 0x00u,
/*  bNumInterfaces                         */ 0x01u,
/*  bConfigurationValue                    */ 0x01u,
/*  iConfiguration                         */ 0x00u,
/*  bmAttributes                           */ 0x80u,
/*  bMaxPower                              */ 0x19u,
/*********************************************************************
* Interface Descriptor
*********************************************************************/
/*  Interface Descriptor Length            */ 0x09u,
/*  DescriptorType: INTERFACE              */ 0x04u,
/*  bInterfaceNumber                       */ 0x00u,
/*  bAlternateSetting                      */ 0x00u,
/*  bNumEndpoints                          */ 0x03u,
/*  bInterfaceClass                        */ 0xFEu,
/*  bInterfaceSubClass                     */ 0x03u,
/*  bInterfaceProtocol                     */ 0x01u,
/*  iInterface                             */ 0x00u,
/*********************************************************************
* Endpoint Descriptor
*********************************************************************/
/*  Endpoint Descriptor Length             */ 0x07u,
/*  DescriptorType: ENDPOINT               */ 0x05u,
/*  bEndpointAddress                       */ 0x81u,
/*  bmAttributes                           */ 0x02u,
/*  wMaxPacketSize                         */ 0x40u, 0x00u,
/*  bInterval                              */ 0x00u,
/*********************************************************************
* Endpoint Descriptor
*********************************************************************/
/*  Endpoint Descriptor Length             */ 0x07u,
/*  DescriptorType: ENDPOINT               */ 0x05u,
/*  bEndpointAddress                       */ 0x02u,
/*  bmAttributes                           */ 0x02u,
/*  wMaxPacketSize                         */ 0x40u, 0x00u,
/*  bInterval                              */ 0x00u,
/*********************************************************************
* Endpoint Descriptor
*********************************************************************/
/*  Endpoint Descriptor Length             */ 0x07u,
/*  DescriptorType: ENDPOINT               */ 0x05u,
/*  bEndpointAddress                       */ 0x83u,
/*  bmAttributes                           */ 0x03u,
/*  wMaxPacketSize                         */ 0x02u, 0x00u,
/*  bInterval                              */ 0x01u
};

/*********************************************************************
* String Descriptor Table
*********************************************************************/
const uint8 CYCODE USBTMC_STRING_DESCRIPTORS[27u] = {
/*********************************************************************
* Language ID Descriptor
*********************************************************************/
/* Descriptor Length                       */ 0x04u,
/* DescriptorType: STRING                  */ 0x03u,
/* Language Id                             */ 0x09u, 0x04u,
/*********************************************************************
* String Descriptor: "Blah"
*********************************************************************/
/* Descriptor Length                       */ 0x0Au,
/* DescriptorType: STRING                  */ 0x03u,
 (uint8)'B', 0u,(uint8)'l', 0u,(uint8)'a', 0u,(uint8)'h', 0u,
/*********************************************************************
* String Descriptor: "Yaugi"
*********************************************************************/
/* Descriptor Length                       */ 0x0Cu,
/* DescriptorType: STRING                  */ 0x03u,
 (uint8)'Y', 0u,(uint8)'a', 0u,(uint8)'u', 0u,(uint8)'g', 0u,(uint8)'i', 0u,
/*********************************************************************/
/* Marks the end of the list.              */ 0x00u};
/*********************************************************************/

/*********************************************************************
* Serial Number String Descriptor
*********************************************************************/
const uint8 CYCODE USBTMC_SN_STRING_DESCRIPTOR[22] = {
/* Descriptor Length                       */ 0x16u,
/* DescriptorType: STRING                  */ 0x03u,
(uint8)'M', 0u,(uint8)'Y', 0u,(uint8)'1', 0u,(uint8)'2', 0u,(uint8)'3', 0u,
 (uint8)'4', 0u,(uint8)'5', 0u,(uint8)'6', 0u,(uint8)'7', 0u,(uint8)'8', 0u
};



/*********************************************************************
* Endpoint Setting Table -- This table contain the endpoint setting
*                           for each endpoint in the configuration. It
*                           contains the necessary information to
*                           configure the endpoint hardware for each
*                           interface and alternate setting.
*********************************************************************/
const T_USBTMC_EP_SETTINGS_BLOCK CYCODE USBTMC_DEVICE0_CONFIGURATION0_EP_SETTINGS_TABLE[3u] = {
/* IFC  ALT    EPAddr bmAttr MaxPktSize Class ********************/
{0x00u, 0x00u, 0x81u, 0x02u, 0x0040u,   0xFEu},
{0x00u, 0x00u, 0x02u, 0x02u, 0x0040u,   0xFEu},
{0x00u, 0x00u, 0x83u, 0x03u, 0x0002u,   0xFEu}
};
const uint8 CYCODE USBTMC_DEVICE0_CONFIGURATION0_INTERFACE_CLASS[1u] = {
0xFEu
};
/*********************************************************************
* Config Dispatch Table -- Points to the Config Descriptor and each of
*                          and endpoint setup table and to each
*                          interface table if it specifies a USB Class
*********************************************************************/
const T_USBTMC_LUT CYCODE USBTMC_DEVICE0_CONFIGURATION0_TABLE[4u] = {
    {0x01u,     &USBTMC_DEVICE0_CONFIGURATION0_DESCR},
    {0x03u,     &USBTMC_DEVICE0_CONFIGURATION0_EP_SETTINGS_TABLE},
    {0x00u,    NULL},
    {0x00u,     &USBTMC_DEVICE0_CONFIGURATION0_INTERFACE_CLASS}
};
/*********************************************************************
* Device Dispatch Table -- Points to the Device Descriptor and each of
*                          and Configuration Tables for this Device 
*********************************************************************/
const T_USBTMC_LUT CYCODE USBTMC_DEVICE0_TABLE[3u] = {
    {0x01u,     &USBTMC_DEVICE0_DESCR},
    {0x00u,    NULL},
    {0x01u,     &USBTMC_DEVICE0_CONFIGURATION0_TABLE}
};
/*********************************************************************
* Device Table -- Indexed by the device number.
*********************************************************************/
const T_USBTMC_LUT CYCODE USBTMC_TABLE[1u] = {
    {0x01u,     &USBTMC_DEVICE0_TABLE}
};

#endif /* USER_SUPPLIED_DESCRIPTORS */

#if defined(USBTMC_ENABLE_MSOS_STRING)

    /******************************************************************************
    *  USB Microsoft OS String Descriptor
    *  "MSFT" identifies a Microsoft host
    *  "100" specifies version 1.00
    *  USBTMC_GET_EXTENDED_CONFIG_DESCRIPTOR becomes the bRequest value
    *  in a host vendor device/class request
    ******************************************************************************/

    const uint8 CYCODE USBTMC_MSOS_DESCRIPTOR[USBTMC_MSOS_DESCRIPTOR_LENGTH] = {
    /* Descriptor Length                       */   0x12u,
    /* DescriptorType: STRING                  */   0x03u,
    /* qwSignature - "MSFT100"                 */   (uint8)'M', 0u, (uint8)'S', 0u, (uint8)'F', 0u, (uint8)'T', 0u,
                                                    (uint8)'1', 0u, (uint8)'0', 0u, (uint8)'0', 0u,
    /* bMS_VendorCode:                         */   USBTMC_GET_EXTENDED_CONFIG_DESCRIPTOR,
    /* bPad                                    */   0x00u
    };

    /* Extended Configuration Descriptor */

    const uint8 CYCODE USBTMC_MSOS_CONFIGURATION_DESCR[USBTMC_MSOS_CONF_DESCR_LENGTH] = {
    /*  Length of the descriptor 4 bytes       */   0x28u, 0x00u, 0x00u, 0x00u,
    /*  Version of the descriptor 2 bytes      */   0x00u, 0x01u,
    /*  wIndex - Fixed:INDEX_CONFIG_DESCRIPTOR */   0x04u, 0x00u,
    /*  bCount - Count of device functions.    */   0x01u,
    /*  Reserved : 7 bytes                     */   0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    /*  bFirstInterfaceNumber                  */   0x00u,
    /*  Reserved                               */   0x01u,
    /*  compatibleID    - "CYUSB\0\0"          */   (uint8)'C', (uint8)'Y', (uint8)'U', (uint8)'S', (uint8)'B',
                                                    0x00u, 0x00u, 0x00u,
    /*  subcompatibleID - "00001\0\0"          */   (uint8)'0', (uint8)'0', (uint8)'0', (uint8)'0', (uint8)'1',
                                                    0x00u, 0x00u, 0x00u,
    /*  Reserved : 6 bytes                     */   0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
    };

#endif /* USBTMC_ENABLE_MSOS_STRING */

/* DIE ID string descriptor for 8 bytes ID */
#if defined(USBTMC_ENABLE_IDSN_STRING)
    uint8 USBTMC_idSerialNumberStringDescriptor[USBTMC_IDSN_DESCR_LENGTH];
#endif /* USBTMC_ENABLE_IDSN_STRING */


/* [] END OF FILE */
