function updateAccount()
{
    if(validateAccount())
    {
        if(screenAccounts.editAccount(currentAccDesc, txtUsername.m_text, txtSecret.m_text, ((currentAccId === "1") ? cboServer.selectedData : txtServer.m_text), currentAccId, cboFirstCodec.selectedData, cboSecCodec.selectedData, cboThirdCodec.selectedData, cboSRTP.selectedData, cboTLS.selectedData, txtMailbox.m_text))
        {
            if(activeAccount == currentAccId)
            {
                telapi.unregisterAccount();
                telapi.registerAccount(activeAccount)
            }

            messageBox.state = "show";
            messageBox.m_color = "white";
            messageBox.text = "Account data edited";
            m_activeScreen = "Accounts";
        }
        else
        {
            messageBox.state = "show";
            messageBox.m_color = "red";
            messageBox.text = "Error when editing the account, internal error";
        }
    }
    else
    {
        messageBox.state = "show";
        messageBox.m_color = "red";
        messageBox.text = "Error when editing the account, please fill all fields";
    }
}

function validateAccount()
{
    return true;
}
