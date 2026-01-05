# Дать группе HelpDesk право на запись lockoutTime
$userOU = "OU=Users,DC=domain,DC=com"
$group = "HelpDesk"
$rights = "WriteProperty"
$attr = "lockoutTime"

Add-ADPermission -Identity $userOU -User $group -AccessRights $rights -Properties $attr