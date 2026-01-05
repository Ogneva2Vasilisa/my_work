# Параметры
param (
    [Parameter(Mandatory=$true)]
    [string]$UserName

)
$UserPassword = ConvertTo-SecureString "User1234" -AsPlainText -Force
$UserOU = "OU=Developers,OU=testMyCompany,DC=mydomain,DC=com"
$FileServerPath = "C:\Sharing\Users"
$FileServerShare = "\\Controller\"

# Создание пользователя
New-ADUser -Name $UserName -SamAccountName $UserName -UserPrincipalName "$UserName@mydomain.com" -AccountPassword $UserPassword -Enabled $true -Path $UserOU


# Создание папки пользователя
$UserFolderPath = Join-Path -Path $FileServerPath -ChildPath $UserName
New-Item -Path $UserFolderPath -ItemType Directory

# Настройка общего доступа
$ShareName = "$UserName$"

New-SmbShare -Name $ShareName -Path $UserFolderPath -ChangeAccess "$UserName@mydomain.com"

# Настройка разрешений NTFS
$Acl = Get-Acl -Path $UserFolderPath
$User = [System.Security.Principal.NTAccount]::new("MYDOMAIN", $UserName)
$AccessRule = [System.Security.AccessControl.FileSystemAccessRule]::new($User, "Modify", "ContainerInherit,ObjectInherit", "None", "Allow")
$Acl.SetAccessRule($AccessRule)
Set-Acl -Path $UserFolderPath -AclObject $Acl

Write-Host "User $UserName was created, folder configured."