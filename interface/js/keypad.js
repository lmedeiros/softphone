function callAnswer(has_video)
{
    if(screenPhone.is_registered===0)
    {
        messageBox.m_color = "lightred"
        messageBox.text = "Error: Account is not registered with server"
        messageBox.state = "show"
        return;
    }

    if(keypadText==="")
    {
        if(lastDial!=="")
        {
            screenPhone.keypadEvent(lastDial);
            return;
        }
        else
        {
            return;
        }
    }

    if(screenPhone.active_channel===1)
    {
        if(screenPhone.c1_call_id < 0)
        {
            telapi.callTo(keypadText, has_video);
        }
    }

    if(screenPhone.active_channel===2)
    {
        if(screenPhone.c2_call_id < 0)
        {
            telapi.callTo(keypadText, has_video);
        }
    }
}
