function saveAudioSettings()
{
    var success;
    success = screenSettings.setAudioDevices(cboInputDevice.selectedData, cboOutputDevice.selectedData);

    if(success)
    {
        success = screenSettings.setSipPort(txtSipPort.m_text);
        if(success)
        {
            if(screenPhone.is_registered === 1)
            {
                telapi.unregisterAccount();
                telapi.registerAccount(screenSettings.getActiveAccount())
            }
            messageBox.m_color = "white"
            messageBox.state = "show";
            messageBox.text = "Audio Settings updated";
        }
        else
        {
            messageBox.m_color = "red"
            messageBox.state = "show";
            messageBox.text = "Error setting Sip Port";
        }
    }
    else
    {
        messageBox.m_color = "red"
        messageBox.state = "show";
        messageBox.text = "Error setting audio devices";
    }
}

function saveVideoSettings()
{
    var success;

    var devtmp = cboVideoDevice.selectedData.split("::::");
    var device_id = devtmp[1];
    var device_name = devtmp[0];

    success = screenSettings.setVideoDevice(device_id, device_name);

    if(success)
    {
        if(screenPhone.is_registered === 1)
        {
            telapi.unregisterAccount();
            telapi.registerAccount(screenSettings.getActiveAccount())
        }
        messageBox.m_color = "white"
        messageBox.state = "show";
        messageBox.text = "Video Settings updated";
    }
    else
    {
        messageBox.m_color = "red"
        messageBox.state = "show";
        messageBox.text = "Error setting video device";
    }
}
