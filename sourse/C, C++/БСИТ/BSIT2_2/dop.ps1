Import-Module ActiveDirectory

# Функция для установки атрибута AD через ADSI (обходит ограничения Set-ADUser)
function Set-sthADAttribute {
    param (
        [Parameter(Mandatory=$true)] [string]$Identity,
        [Parameter(Mandatory=$true)] [string]$Attribute,
        [Parameter(Mandatory=$true)] [byte[]]$Value
    )

    $userDN = (Get-ADUser -Identity $Identity).DistinguishedName
    if (-not $userDN) {
        Write-Warning "Пользователь $Identity не найден."
        return
    }

    $user = [ADSI]"LDAP://$userDN"

    if ($Value -eq $null) {
        $user.Properties[$Attribute].Clear()
    }
    else {
        # Преобразуем массив байт в COM-совместимый массив
        $byteArray = [System.Array]::CreateInstance([byte], $Value.Length)
        for ($i = 0; $i -lt $Value.Length; $i++) {
            $byteArray.SetValue($Value[$i], $i)
        }
        $user.Properties[$Attribute].Value = $byteArray
    }

    $user.SetInfo()
}

# Функция для создания битовой маски часов в одном 24-часовом дне (разбита на 3 байта по 8 часов)
function Get-HourMask {
    param (
        [int]$startHour,
        [int]$endHour
    )
    $mask = 0
    for ($h = $startHour; $h -lt $endHour; $h++) {
        $bitPosition = 7 - ($h % 8)  # Корректный порядок битов внутри байта
        $mask = $mask -bor (1 -shl $bitPosition)
    }
    return $mask
}

# Функция создания массива logonHours для всей недели (7 дней * 3 байта = 21 байт)
function New-LogonHours {
    param (
        [int[]]$AllowedHoursPerDay  # Массив из 7 элементов, каждый - 24-битовая маска для дня
    )
    $logonHours = New-Object byte[] 21
    for ($day = 0; $day -lt 7; $day++) {
        $hours = $AllowedHoursPerDay[$day]
        $logonHours[$day*3]   = ($hours -band 0xFF)
        $logonHours[$day*3+1] = (($hours -shr 8) -band 0xFF)
        $logonHours[$day*3+2] = (($hours -shr 16) -band 0xFF)
    }
    return $logonHours
}

# Функция сравнения двух массивов logonHours
function Compare-LogonHours {
    param (
        [byte[]]$CurrentHours,
        [byte[]]$NewHours
    )
    if ($null -eq $CurrentHours) { return $false }
    if ($CurrentHours.Length -ne $NewHours.Length) { return $false }
    
    for ($i = 0; $i -lt $CurrentHours.Length; $i++) {
        if ($CurrentHours[$i] -ne $NewHours[$i]) {
            return $false
        }
    }
    return $true
}

# Создаем маски для разных временных интервалов
$mask_8_22 = Get-HourMask -startHour 8 -endHour 22
$mask_0_24 = 0xFFFFFF  # 24 часа (3 байта с установленными битами)
$mask_9_18 = Get-HourMask -startHour 9 -endHour 18

# Определяем разрешенные часы для каждой группы (7 дней недели: Sunday=0 ... Saturday=6)
$allowedHours_AccountManagers = @($mask_8_22, $mask_8_22, $mask_8_22, $mask_8_22, $mask_8_22, $mask_8_22, $mask_8_22)
$allowedHours_GeneralAdmins = @($mask_0_24, $mask_0_24, $mask_0_24, $mask_0_24, $mask_0_24, $mask_0_24, $mask_0_24)
$allowedHours_Others = @(0, $mask_9_18, $mask_9_18, $mask_9_18, $mask_9_18, $mask_9_18, 0)  # Воскресенье и суббота - запрещено

# Генерируем массивы logonHours для групп
$logonHours_AccountManagers = New-LogonHours -AllowedHoursPerDay $allowedHours_AccountManagers
$logonHours_GeneralAdmins = New-LogonHours -AllowedHoursPerDay $allowedHours_GeneralAdmins
$logonHours_Others = New-LogonHours -AllowedHoursPerDay $allowedHours_Others

# Сопоставляем группы с их настройками времени
$groups = @{
    "Account Managers" = $logonHours_AccountManagers
    "Help Desk" = $logonHours_AccountManagers
    "Resource Admins" = $logonHours_AccountManagers
    "General Admins" = $logonHours_GeneralAdmins
}

$processedUsers = @()
$successCount = 0
$skipCount = 0
$errorCount = 0

foreach ($group in $groups.Keys) {
    try {
        Write-Host "`nОбработка группы: $group" -ForegroundColor Magenta
        $users = Get-ADGroupMember -Identity $group -Recursive | Where-Object { $_.objectClass -eq "user" }
        
        foreach ($user in $users) {
            if ($processedUsers -contains $user.SamAccountName) {
                Write-Host "Пользователь $($user.Name) уже обработан, пропускаем" -ForegroundColor DarkGray
                $skipCount++
                continue
            }

            try {
                # Получаем текущие настройки logonHours
                $currentHours = (Get-ADUser $user.SamAccountName -Properties logonHours).logonHours
                
                # Проверяем, нужно ли обновлять
                if (Compare-LogonHours -CurrentHours $currentHours -NewHours $groups[$group]) {
                    Write-Host "Настройки для $($user.Name) уже верные, пропускаем" -ForegroundColor DarkGray
                    $skipCount++
                }
                else {
                    # Устанавливаем новые значения через ADSI-функцию
                    Set-sthADAttribute -Identity $user.SamAccountName -Attribute "logonHours" -Value $groups[$group]
                    Write-Host "Установлены часы входа для $($user.Name)" -ForegroundColor Green
                    $successCount++
                    
                    # Проверяем результат
                    $newHours = (Get-ADUser $user.SamAccountName -Properties logonHours).logonHours
                    Write-Host "Новые настройки: $([BitConverter]::ToString($newHours))" -ForegroundColor DarkGray
                }
                
                $processedUsers += $user.SamAccountName
            }
            catch {
                Write-Warning "Ошибка обработки пользователя $($user.Name): $_"
                $errorCount++
            }
        }
    }
    catch {
        Write-Warning "Ошибка обработки группы $group : $_"
        $errorCount++
    }
}

# Обрабатываем всех остальных пользователей, не входящих в указанные группы
try {
    Write-Host "`nОбработка остальных пользователей..." -ForegroundColor Magenta
    $allUsers = Get-ADUser -Filter * -Properties MemberOf | Where-Object { $processedUsers -notcontains $_.SamAccountName }
    
    foreach ($user in $allUsers) {
        try {
            $currentHours = (Get-ADUser $user.SamAccountName -Properties logonHours).logonHours
        
            if (Compare-LogonHours -CurrentHours $currentHours -NewHours $logonHours_Others) {
                Write-Host "Настройки для $($user.Name) уже верные, пропускаем" -ForegroundColor DarkGray
                $skipCount++
                continue
            }
        
            if ($logonHours_Others -ne $null) {
                Set-sthADAttribute -Identity $user.SamAccountName -Attribute "logonHours" -Value $logonHours_Others
                Write-Host "Установлены часы по умолчанию для $($user.Name)" -ForegroundColor Cyan
                $successCount++
            
                $newHours = (Get-ADUser $user.SamAccountName -Properties logonHours).logonHours
                Write-Host "Новые настройки: $([BitConverter]::ToString($newHours))" -ForegroundColor DarkGray
            }
            else {
                Write-Warning "logonHours_Others равен null, пропускаем пользователя $($user.SamAccountName)"
            }
        }
        catch {
            Write-Warning "Ошибка обработки пользователя $($user.Name): $_"
            $errorCount++
        }
    }

}
catch {
    Write-Warning "Ошибка обработки остальных пользователей: $_"
    $errorCount++
}

Write-Host "`nПримеры настроек:" -ForegroundColor Yellow
Write-Host "Account Managers/Help Desk/Resource Admins: $([BitConverter]::ToString($logonHours_AccountManagers))"
Write-Host "General Admins: $([BitConverter]::ToString($logonHours_GeneralAdmins))"
Write-Host "Остальные пользователи: $([BitConverter]::ToString($logonHours_Others))`n"

Write-Host "Настройка времени входа завершена!" -ForegroundColor Green
