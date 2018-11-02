UniFlash Project Generator
======

Description
-----------

The `uniflash_project_generator` is a simple way to generate and flash UniFlash projects for CC3220R, CC3220S and CC3220SF.`uniflash_project_generator` located at ```[sdk_install_path]\tools\cc32xx_tools\uniflash-project-generator```

Generated project includes:
* Service Pack file from ```sdk_install_path\tools\cc32xx_tools\servicepack-cc3x20``` depends on selected device
* Trusted Root-Certificate Catalog from ```sdk_install_path\tools\cc32xx_tools\certificate-playground```
* Dummy Root-Certificate and Dummy User-Certificate from ```sdk_install_path\tools\cc32xx_tools\certificate-playground```
* OTA file sign with ```sdk_install_path\tools\cc32xx_tools\ota-example-cert\dummy_ota_vendor_key.der```

Requirements
-----------

| Application | Version |
| --- |  --- |
| UniFlash | 4.3.1.1835 |
| SLImageCreator | 1.0.19.6 |


Synopsis
-----------

```batch
uniflash_project_generator.py [-h] --name PROJECT_NAME --device
                                     DEVICE_TYPE --mcu_bin MCU_FILENAME
                                     [--debug] [--flash] [--com_port COM_PORT]
                                     [--serial_number SERIAL_NUMBER]
                                     [--extra_files_json EXTRA_FILES_JSON]
                                     [--ota_tar OTA_TAR_FILE]
```

Options
-----------

**--name**
: Unique project name

**--device**
: LaunchPad type. Available options : CC3220R, CC3220S, CC3220SF

**--mcu_bin**
: MCU image file

**--debug**
: Enable debug prints

**--flash**
: Flash device after creating a project 

**--com_port**
: Flash specific device by enter number of com port that device connected to. Must be set with --serial_number

**--serial_number**
: Flash specific device by enter xds serial number of device. Must be set with --com_port

**--extra_files_json**
: User can add extra files to uniflash project by pointing to json file with settings. See section below

**--ota_tar**
: Create ota file from project. This parameter disable --flash flag it will create only ota tar file

**-h**
: Show help options

Extra Files JSON Format
-----------------------
This is json format that required in order to add extra files for uniflash project.
```json
[
  {
    "local_file": "C:\\test\\style.css",
    "fs_path": "/www/css/style.css",
    "flags": "failsafe,secure,publicwrite,nosignaturetest"
  },
  {
    "local_file": "C:\\test\\help.png",
    "fs_path": "/www/images/help.png",
    "flags": "failsafe"
  },
  {
    "local_file": "C:\\test\\ota.dat",
    "fs_path": "ota.dat",
    "flags": null
  }
]
```


Examples
-----------

Creating uniflash project for network_terminal demo on CC3220SF launchpad without flashing the device

```batch
uniflash_project_generator --name network_terminal --device CC3220SF --mcu_bin <path_to_network_terminal_bin>
```

Creating uniflash project with extra files for network_terminal demo on CC3220SF launchpad without flashing the device

```batch
uniflash_project_generator --name network_terminal --device CC3220SF --mcu_bin <path_to_network_terminal_bin> --extra_files_json "c:\temp\extra_files.json"
```

Creating and flashing uniflash project for network_terminal demo on CC3220SF launchpad

```batch
uniflash_project_generator --name network_terminal --device CC3220SF --mcu_bin <path_to_network_terminal_bin> --flash
```

Creating and flashing uniflash project for network_terminal demo on specific CC3220SF launchpad

```batch
uniflash_project_generator --name network_terminal --device CC3220SF --mcu_bin <path_to_network_terminal_bin> --flash --com_port 3 --serial_number 0123456
```

Creating and flashing uniflash project for network_terminal demo on CC3220SF launchpad with debug prints

```batch
uniflash_project_generator --name network_terminal --device CC3220SF --mcu_bin <path_to_network_terminal_bin> --flash --debug
```

Creating ota file for OOB (Out Of the Box) demo on CC3220SF launchpad with debug prints

```batch
uniflash_project_generator --name network_terminal --device CC3220SF --mcu_bin <path_to_network_terminal_bin> --debug --ota_tar "c:\temp\ota_new_file.tar"
```

