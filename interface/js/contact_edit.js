function addNumber()
{
    if(validateNumber(phoneNumber.m_text))
    {
        if(screenContacts.addNumber(capitaliseFirstLetter(phoneDescription.m_text), phoneNumber.m_text, selectedContact))
        {
            messageBox.m_color = "white"
            messageBox.text = "Number (" + phoneDescription.m_text + ") " + phoneNumber.m_text + " has been added"
            messageBox.state = "show"
            screenContacts.getNewNumbers(selectedContact);
            clearPhone();
        }
        else
        {
            messageBox.m_color = "red"
            messageBox.text = "Error when adding the number"
            messageBox.state = "show"
        }
    }
}

function validateNumber(value)
{
    if(value==="" || value.indexOf(" ")>0)
    {
        messageBox.m_color = "red"
        messageBox.text = "Invalid Number"
        messageBox.state = "show"
        return false;
    }

    return true;
}

function clearPhone()
{
    phoneNumber.m_input.color = "#BBB"
    phoneNumber.m_text = "Phone Number"
    phoneDescription.m_input.color = "#BBB"
    phoneDescription.m_text = "Description"
    phoneDescription.forceActiveFocus();
}

function editContact()
{
    if(validateContact())
    {
        if(screenContacts.editContact(txtName.m_text, txtSurname.m_text, txtPicPath.source, selectedContact))
        {
            for(var i = 0 ; listViewNumbers.count > i ; i++)
            {
                console.debug("edit contact i: " + listViewNumbers.model.data(0,0));
            }

            messageBox.m_color = "white"
            messageBox.text = "Contact " + txtName.m_text + " has been edited"
            messageBox.state = "show"
            m_loader_screen = ""
            waiterAddContact.running = true
        }
        else
        {
            messageBox.m_color = "red"
            messageBox.text = "Error when editing the contact"
            messageBox.state = "show"
        }
    }
}

function validateContact()
{
    if(txtName.m_text!=="")
    {
        if(txtPicPath.m_text===" ")
        {
            txtPicPath.m_text="";
        }

        return true;
    }
    else
    {
        messageBox.m_color = "red"
        messageBox.text = "Error when editing contact, please fill all fields"
        messageBox.state = "show"
    }
}


function capitaliseFirstLetter(string)
{
    return string.charAt(0).toUpperCase() + string.slice(1);
}
