param (
    [Parameter(Mandatory=$true)]
    [string]$UserName
)

$dcName="mydomain"
$dcComName="com"
$serverPCName="Controller"

try {
    $user = Get-ADUser -Identity $UserName -Properties LockedOut -ErrorAction Stop
    
    if ($user.LockedOut) {
        Unlock-ADAccount -Identity $UserName
        Write-Host "Пользователь $UserName успешно разблокирован." -ForegroundColor Green
    }
    else {
        Write-Host "Пользователь $UserName не заблокирован." -ForegroundColor Yellow
    }
}
catch {
    Write-Error "Ошибка: $_"
    Write-Host "Пользователь $UserName не найден или произошла ошибка." -ForegroundColor Red
}