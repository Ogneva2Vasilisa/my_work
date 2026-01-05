# Пример: Дать группе "Resource Admins" полный доступ к папке D:\Shared
$folderPath = "D:\Shared"
$acl = Get-Acl $folderPath
$rule = New-Object System.Security.AccessControl.FileSystemAccessRule(
    "DOMAIN\Resource Admins",  # Группа
    "Modify",                  # Права (Read, Write, Modify, FullControl)
    "ContainerInherit, ObjectInherit",  # Наследование
    "None",                    # No propagation
    "Allow"                    # Тип доступа
)
$acl.AddAccessRule($rule)
Set-Acl -Path $folderPath -AclObject $acl