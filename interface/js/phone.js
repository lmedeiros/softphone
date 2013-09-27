function backspace(text)
{
    return text.slice(0,-1);
}

function secToTime(secs)
{
    if(secs<0) secs = 0;
    var sec_numb = parseInt(secs);
    var hours   = Math.floor(sec_numb / 3600);
    var minutes = Math.floor((sec_numb - (hours * 3600)) / 60);
    var seconds = sec_numb - (hours * 3600) - (minutes * 60);

    if (hours   < 10) {hours   = "0"+hours;}
    if (minutes < 10) {minutes = "0"+minutes;}
    if (seconds < 10) {seconds = "0"+seconds;}
    var time    = hours+':'+minutes+':'+seconds;
    return time;
}

function callAnswer()
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

    console.debug("trying to dial");

    if(screenPhone.active_channel===1)
    {
        console.debug("c1_call_id " + screenPhone.c1_call_id);
        if(screenPhone.c1_call_id < 0)
        {
            telapi.callTo(keypadText);
            return;
        }
    }

    if(screenPhone.active_channel===2)
    {
        if(screenPhone.c2_call_id < 0)
        {
            telapi.callTo(keypadText);
            return;
        }
    }
}

function hangup(channel)
{
    telapi.hangUp(channel);
}

function holdIcon()
{
    if(screenPhone.active_channel === 1)
    {
        return screenPhone.c1_state==="busy" ? "../../assets/ic_hold_off.png" : screenPhone.c1_state==="hold" ? "../../assets/ic_hold_on.png" : "../../assets/ic_hold_disabled.png"
    }
    else if(screenPhone.active_channel === 2)
    {
        return screenPhone.c2_state==="busy" ? "../../assets/ic_hold_off.png" : screenPhone.c2_state==="hold" ? "../../assets/ic_hold_on.png" : "../../assets/ic_hold_disabled.png"
    }
}

function transferIcon()
{
    if(screenPhone.active_channel === 1)
    {
        return screenPhone.c1_state === "busy" ? "../../assets/ic_transfer_enabled.png" : "../../assets/ic_transfer_disabled.png"
    }
    else if(screenPhone.active_channel === 2)
    {
        return screenPhone.c2_state === "busy" ? "../../assets/ic_transfer_enabled.png" : "../../assets/ic_transfer_disabled.png"
    }
}

function channelPress(channel)
{
    if(channel==="c1")
    {
        if(screenPhone.c2_state==="busy")
        {
            telapi.hold(screenPhone.c2_call_id);
        }
        if(screenPhone.c1_state==="hold")
        {
            telapi.unhold(screenPhone.c1_call_id);
        }
    }

    if(channel==="c2")
    {
        if(screenPhone.c1_state==="busy")
        {
            telapi.hold(screenPhone.c1_call_id);
        }
        if(screenPhone.c2_state==="hold")
        {
            telapi.unhold(screenPhone.c2_call_id);
        }
    }
    screenPhone.keypadEvent(channel);
}

function hold()
{
    if(screenPhone.active_channel===1)
    {
        if(screenPhone.c1_state==="hold")
        {
            telapi.unhold(screenPhone.c1_call_id);
        }
        else if(screenPhone.c1_state==="busy")
        {
            telapi.hold(screenPhone.c1_call_id);
        }
    }
    else if(screenPhone.active_channel===2)
    {
        if(screenPhone.c2_state==="hold")
        {
            telapi.unhold(screenPhone.c2_call_id);
        }
        else if(screenPhone.c2_state==="busy")
        {
            telapi.hold(screenPhone.c2_call_id);
        }
    }
}
