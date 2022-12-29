# Overview
DeskLampAutomation/vscode is built an ESP-IDF project built using VSCode on Windows. The supplied build script is written in powershell. I don't have a Linux/Unix dev environment currently so I haven't converted it to bash or zsh. As such, Windows is the targeted environment for this installation.

¯\\\_(ツ)\_/¯

The VSCode version of this project uses a build script written in PowerShell 7 to wrap the ESP-IDF tools. This work around was chosen after many frustrating hours trying to get the Espressif IDE to work, and the VSCode extension to be consistent. I've found this script to be a reliable tool, but alternatively the Espressif tools *should* work, so you could try those instead.

Additionally, the ESP-IDF tools require some software to be installed before use--mainly you'll need CMake and Ninja. I use choco to install things on Windows but you can use whatever works for you.

For security, I use 1Password to store my WiFi password and UUID. If you're not concerned about storing you're WiFi credentials in the sdkconfig, there are instructions in the 1Password section of this guide to adjust the esp_tools.ps1 build script to not use the 1Password CLI. 

# General
After downloading the project, define it's location as an environment variable named `DESK_LAMP_AUTOMATION_DIR`

    > [System.Environment]::SetEnvironmentVariable("DESK_LAMP_AUTOMATION_DIR", "E:\Projects\home_automation\DeskLampAutomation\", "User")

# Windows
Install Windows 10 or 11 using the following guides
- [Windows 10](https://www.microsoft.com/en-us/software-download/windows10%20)
- [Windows 11](https://support.microsoft.com/en-us/windows/ways-to-install-windows-11-e0edbbfb-cfc5-4011-868b-2ce77ac7c70e)

# PowerShell 7
Install the latest version of PowerShell using this guide
- [PowerShell](https://learn.microsoft.com/en-us/powershell/scripting/install/installing-powershell-on-windows?view=powershell-7.3)

After installing Powershell, I recommend aliasing the esp_tool.ps1 script.
- Open `Documents/Microsoft.PowerShell_profile.ps1`
- Append the following snippet

        Set-Alias -Name "ESP_TOOL" -Value "$env:DESK_LAMP_AUTOMATION_DIR\vscode\Scripts\esp_tool.ps1"
- Reload PowerShell terminal

# Choco
Choco is my preferred way of installing software on Windows. It offers an easy to cli similar to apt on Linux. If you choose not to install Choco, the following installation steps will not apply; however, you will still need the software listed in this guide. 

Install Choco using this guide
- [Choco](https://chocolatey.org/install)

# CMake
Install CMake using this command:

    > choco install cmake

# Ninja
Install Ninja using this command:

    > choco install ninja

# VSCode
Install VSCode using this command:
    
    > choco install vscode

# Espressif IDF
The ESP-IDF installation has two parts. There's a VSCode extension you'll need and source code you'll need from the ESP-IDF github. It doesn't particularly matter the order you install each part, but if you install the source code first, you can use that during the VSCode extension installation.


## Github Source Code
For the source code:

- Go to the [ESP-IDF github](https://github.com/espressif/esp-idf) and download the latest release

- Run the installation script `install.ps1`
    - Note, you may need to adjust your *ExecutionPolicy* using the following command

            > Set-ExecutionPolicy -Scope CurrentUser Unrestricted
        This will set the *ExecutionPolicy* to *Unrestricted* for the current user. If that scope is not acceptable see more information in the [Microsoft documentation](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.security/set-executionpolicy?view=powershell-7.3).

- Define environment variable IDF_PATH to the root of the source code

        > [System.Environment]::SetEnvironmentVariable("IDF_PATH","C:\Espressif\esp-idf-v5.0", "User")

- Define environment variable IDF_PYTHON_PATH to the root of the installed python directory

        > [System.Environment]::SetEnvironmentVariable("IDF_PYTHON_PATH","C:\Users\<your_username>\.espressif\python_env\idf5.0_py3.11_env", "User")

## VSCode Extension
In VSCode, search for the `Espressif IDF` extension and install. Then follow the setup steps in the extension.


# 1Password
1Password is a subscription based password manager with a CLI. It is not essential to building this project, but I prefer it so my WiFi credentials are not stored in plaintext inside the sdkconfig file.


### Removing 1Password Dependency
If you do not wish to use 1Password:

- Open `Scripts/esp_tools.ps1`

- Find the following code block

        elseif ($do_flash){
            GetOnePasswordInfo
            _set_sdk_wifi_password
            ESP_Flash
            _set_sdk_wifi_placeholder
        }

- Apply the following changes
        
        elseif ($do_flash){
            # GetOnePasswordInfo
            # _set_sdk_wifi_password
            ESP_Flash
            # _set_sdk_wifi_placeholder
        }
- Open `sdkconfig`

- Find the following code

        CONFIG_EXAMPLE_WIFI_SSID="WIFI_SSID_PLACEHOLDER"
        CONFIG_EXAMPLE_WIFI_PASSWORD="WIFI_PASSWORD_PLACEHOLDER"

- Edit the placeholders to use your WiFi information

### Installing 1Password
For 1Password, choco does not currently have v8 which is what we want. So, you'll need to go to the website to download the installer. 
- [1Password](https://1password.com/downloads/windows/)

To install 1Password CLI run the following command
    
    choco install op

After installing 1Password, input your WiFi information into your vault. I used the *Wireless Router* item type, but I don't think it matters in terms of retrieving your information. 

Now we need to setup 1Password to authenticate with Windows Hello. To do that, go into Settings > Security and check the `Unlock using Windows Hello` box.

Next, we'll need to enable the CLI feature. Go into Settings > Developer and check the `Connect with 1Password CLI` box

With the WiFi item created and the features we need enabled, now we'll need the ID for the WiFi item. To find the ID I recommend the following:
- Open a PowerShell terminal
- Run 
        
        op item list
- Find the name of your WiFi item under the TITLE column
- Copy the ID

With the Wifi item ID copied, define an environment variable for the WiFi object in your vault using the following command. Make sure to paste in your ID.

    [System.Environment]::SetEnvironmentVariable("OP_WIFI_ID","<PASTE ID HERE>", "User")

# Setting up Intellisense 

VSCode's Intellisense is a little finicky. I was able to get most of the includes to include correctly, but there's probably a cleaner way of accomplishing this.

Anyway, your .vscode/c_cpp_properties.json should look something like this (assuming you haven't found a better way to define all this)

    {
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/Espressif/esp-idf-v5.0/components/**",
                "C:/Espressif/esp-idf-v5.0/examples/common_components/protocol_examples_common/include/**",
                "${workspaceFolder}//build/config",
                "C:/Espressif/esp-idf-v5.0/components/freertos/FreeRTOS-Kernel/include",
                "C:/Espressif/esp-idf-v5.0/components/freertos/esp_additions/include",
                "C:/Espressif/esp-idf-v5.0/components/freertos/FreeRTOS-Kernel-SMP/portable/xtensa/include"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "windowsSdkVersion": "10.0.22621.0",
            "compilerPath": "C:/Espressif/tools/tools/xtensa-esp32-elf/esp-2022r1-11.2.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc.exe",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "gcc-x86"
        }
    ],
    "version": 4
    }