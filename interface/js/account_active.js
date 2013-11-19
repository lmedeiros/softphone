function validateAccount(accountId)
{
    if(accountId>0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

function activateAccount(accountId)
{
    if(validateAccount(accountId))
    {
        if(screenSettings.setActiveAccount(accountId))
        {
            messageBox.text = "Account set, register status: \n" + screenPhone.register_reason;
            messageBox.state = "show";
            messageBox.m_color = "white";
            m_activeScreen = "Accounts";
        }
        else
        {
            messageBox.text = "Error, fail to activate account " + accountId;
            messageBox.state = "show";
            messageBox.m_color = "red";
        }
    }
    else
    {
        messageBox.state = "show";
        messageBox.text = "Please select an account";
    }
}
