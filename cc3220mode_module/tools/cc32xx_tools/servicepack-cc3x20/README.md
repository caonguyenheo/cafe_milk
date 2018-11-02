## Introduction
The SimpleLink™ Wi-Fi® CC3220 device is a single-chip microcontroller (MCU) with built-in Wi-Fi connectivity, created for the Internet of Things (IoT). 
The CC3220 device is a wireless MCU that integrates a high-performance ARM® Cortex®-M4 MCU, allowing customers to develop an entire application with a single IC. 

## Content and Documentation
This release include the ServicePack binary image to be programmed into CC3120/CC3220 devices.
**ServicePack must be applied both in production and development stage of the CC3120/CC3220 devices**
  
| File |  Notes |
| --- | --- | 
| sp_3.9.0.6_2.0.0.0_2.2.0.6.bin | ServicePack binary for UniFlash |
| sp_3.9.0.6_2.0.0.0_2.2.0.6.ucf | ServicePack UCF for host driver API |
| sp_3.9.0.6_2.0.0.0_2.2.0.6.ucf.signed.bin | ServicePack UCF signature |


**Version information**

| Component |  Version |
| --- | --- | 
| NWP | 3.9.0.6 |
| MAC | 2.0.0.0 |
| PHY | 2.2.0.6 |

**Note:**
Upon successful ServicePack programming, version can be retrieved using 'sl_DeviceGet()' API, with SL_DEVICE_GENERAL_VERSION option.

## What's New

* Added ALPN general - can now add wild card string to the ALPN list, for more information refer to the SimpleLink host driver doxygen
* Added downgrade protection to the SP when the vendor certificate catalog new feature exist as from this version the NWP SP contains code implemented fr that feature. please refer to the vendor certificate catalog feature manual - swru574


## Upgrade and Compatibility Information

The ServicePack can be programmed using UniFlash utility.
Latest UniFlash utility can be downloaded from <http://www.ti.com/tool/UniFlash>. 

The ServicePack can also be flushed using host driver API's or OTA application 
(please refer for the SimpleLink CC32xx SDK for more information)

## Dependencies

This release requires the following software components and tools:

* UniFlash latest version - [Download page](http://www.ti.com/tool/UniFlash).
* The ServicePack is bounded to host driver 3.0.1.41

## Device Support
* CC3120R 
* CC3220R – Base variant 
* CC3220S – CC3220 and MCU security 
* CC3220SF – CC3220S and internal flash.

**Evaluation Boards**
* CC3120\_BOOSTXL
* CC3220R\_LAUNCHXL
* CC3220S\_LAUNCHXL
* CC3220SF\_LAUNCHXL

## Fixed Issues
${GEN2_3906_FIXED}
## Known Issues

${GEN2_OPEN_ISSUES}

## Versioning

This product's version follows a version format, **M.mm.pp.bb**, where **M** is a single digit Major number, **mm** is 2 digit minor number, **pp** should be zero indicating official version and **b** is an unrestricted set of digits used as an incremented build counter.

## Technical Support and Product Updates

* Visit the [E2E Forum](https://e2e.ti.com/support/wireless_connectivity/simplelink_wifi_cc31xx_cc32xx/f/)
