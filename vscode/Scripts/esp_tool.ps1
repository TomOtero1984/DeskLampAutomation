param([switch] $help = $False,
      [string] $port = "COM8",
      [switch] $do_build=$False, 
      [switch] $do_clean=$False,
      [switch] $do_rebuild=$False,
      [switch] $do_flash=$False,
      [switch] $do_monitor=$False,
      [switch] $do_export=$False,
      [switch] $do_test=$False)


$idf_py_path = "$env:IDF_PATH\tools\idf.py"
$proj_dir = "E:\Projects\home_automation\DeskLampAutomation\vscode"
$config_original = Get-Content "$proj_dir\sdkconfig"
$network_name, $wireless_password


function ESP-Write-Host {
    param(
        $msg
    )
    Write-Host "[ESP_TOOLS]" -ForegroundColor Cyan -NoNewline
    Write-Host " " $msg
}

function ESP_Export{
    Invoke-Expression "$env:IDF_PATH\export.ps1"
}

function ESP_Activate {
    ESP-Write-Host("Activating build environment")
    Invoke-Expression "$env:IDF_PYTHON_PATH\Scripts\activate.ps1"
    
    ESP-Write-Host("Checking Python path...")
    if (-not ($(Get-Command python).Path.Contains($env:IDF_PYTHON_PATH))){
        throw "Invalid Python Path. Python path does not contain '$env:IDF_PYTHON_PATH'."
    }
    ESP-Write-Host("Python path is valid"   )
}

function ESP_Deactivate {
    ESP-Write-Host("Deactivating build environment")
    deactivate
}

function ESP_Build {
    ESP-Write-Host("Starting build...")
    $py_args = "build"
    & {
        Set-Location $proj_dir 
        python $idf_py_path $py_args
    }
}

function ESP_Clean {
    # Remove-Item -Force -Recurse -Path "$proj_dir\build"
    ESP-Write-Host("Starting full clean...")
    $py_args = "fullclean"
    & {
        Set-Location $proj_dir 
        python $idf_py_path $py_args
    }
}

function ESP_Flash {
    ESP-Write-Host("Starting flash...")
    & {
        Set-Location $proj_dir
        python "$env:IDF_PATH\tools\idf.py" -p $port flash
    }
}

function ESP_Monitor {
    ESP-Write-Host("Starting monitor...")
    & {
        Set-Location $proj_dir 
        python "$env:IDF_PATH\tools\idf_monitor.py" -p $port -b 115200 --toolchain-prefix xtensa-esp32-elf- --target esp32 "$proj_dir\build\vscode.elf"
    }
}

function ESP_Rebuild {
    ESP-Write-Host("Starting rebuild...")
    ESP_Clean
    ESP_Build
}

function _set_sdk_wifi_password {
    $config = $config_original.Replace("WIFI_SSID_PLACEHOLDER", $network_name)
    $config = $config.Replace("WIFI_PASSWORD_PLACEHOLDER", $wireless_password)
    Out-File -FilePath "$proj_dir\sdkconfig" -InputObject $config
}

function _set_sdk_wifi_placeholder {
    Out-File -FilePath "$proj_dir\sdkconfig" -InputObject $config_original
}

function GetOnePasswordInfo {
    $res = op item get $env:OP_WIFI_ID --format json | ConvertFrom-Json
    $network_name, $wireless_password
    for ($i=0; $i -lt $res.fields.length; $i++){
        if ($res.fields[$i].id -eq "network_name") {
            $script:network_name = $res.fields[$i].value
        }
        elseif ($res.fields[$i].id -eq "wireless_password") {
            $script:wireless_password =$res.fields[$i].value
        }
    }
}

function WriteHelp{
    ESP-Write-Host("
    *****************
    *** Help Menu ***
    *****************
    Description
    -----------
    ESP_TOOL is a wrapper for the ESP idf.py tool.
    
    -port       : Defines the port to use 
    -do_build   : Runs the build
    -do_clean   : Cleans the build dir
    -do_flash   : Flashes the device defined in esp_tool.ps1
    -do_monitor : Monitors the ESP device through serial
    -do_all     : Cleans, builds, flashes, monitors 
    -do_test    : Test command for script
    ")
}



if($do_export){
    ESP_Export
}
ESP_Activate
try {
    if($help){
        WriteHelp
    }
    elseif ($do_build){
        ESP_Build
    }
    elseif ($do_clean){
        ESP_Clean
    }
    elseif ($do_rebuild){
        ESP_Rebuild
    }
    elseif ($do_flash){
        GetOnePasswordInfo
        _set_sdk_wifi_password
        ESP_Flash
        _set_sdk_wifi_placeholder
    }
    elseif ($do_monitor){
        ESP_Monitor
    }
    elseif ($do_all){
        ESP_Clean
        ESP_Build
        ESP_Flash
        ESP_Monitor
    }
    elseif ($do_test){
        $PSCommandPath
    }
    else{
        WriteHelp
    }
}
catch {
    ESP-Write-Host("An error occurred:")
    Write-Host $_.ScriptStackTrace
}
finally{
    ESP_Deactivate
}
