# Overview
DeskLampAutomation/vscode is built an ESP-IDF project built using VSCode on Windows. The supplied build script is written in powershell. I don't have a Linux/Unix dev environment currently so I haven't converted it to bash or zsh. As such, Windows is the targeted environment for this installation.

¯\\\_(ツ)\_/¯

The VSCode version of this project uses a build script written in PowerShell 7 to wrap the ESP-IDF tools. This work around was chosen after many frustrating hours trying to get the Espressif IDE to work, and the VSCode extension to be consistent. I've found this script to be a reliable tool, but alternatively the Espressif tools *should* work, so you could try those instead.

Additionally, the ESP-IDF tools require some software to be installed before use--mainly you'll need CMake and Ninja. I use choco to install things on Windows but you can use whatever works for you.

For security, I use 1Password to store my WiFi password and UUID. If you're not concerned about storing you're WiFi credentials in the sdkconfig, there are instructions in the 1Password section of this guide to adjust the esp_tools.ps1 build script to not use the 1Password CLI. 


# Windows
Install Windows 10 or 11 using the following guides
- [Windows 10](https://www.microsoft.com/en-us/software-download/windows10%20)
- [Windows 11](https://support.microsoft.com/en-us/windows/ways-to-install-windows-11-e0edbbfb-cfc5-4011-868b-2ce77ac7c70e)

# PowerShell 7
Install the latest version of PowerShell using this guide
- [PowerShell](https://learn.microsoft.com/en-us/powershell/scripting/install/installing-powershell-on-windows?view=powershell-7.3)

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


## Source Code
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

## VSCode
In VSCode, search for the `Espressif IDF` extension and install. Then follow the setup steps in the extension.


# 1Password