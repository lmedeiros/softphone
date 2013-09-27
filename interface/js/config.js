function updateSettings()
{
    var success;
    success = screenSettings.setAudioDevices(cboInputDevice.selectedData, cboOutputDevice.selectedData, cboRingingDevice.selectedData);

    if(success)
    {
        success = screenSettings.setSipPort(txtSipPort.m_text);
        if(success)
        {
            telapi.unregisterAccount();
            messageBox.m_color = "white"
            messageBox.state = "show";
            messageBox.text = "Settings updated";
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
