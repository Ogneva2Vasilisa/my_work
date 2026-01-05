param(
[int]$expDays=100
)

$Users = Get-ADUser -filter {Enabled -eq $True -and PasswordNeverExpires -eq $False} -Properties msDS-UserPasswordExpiryTimeComputed
$day = Get-Date
foreach($user in $Users)
{
    $expire = [datetime]::FromFileTime($user."msDS-UserPasswordExpiryTimeComputed") - $day
    if ( $expire.Days -le $expDays)
    {
        "[" + $user.Name + "]: " + $expire.Days + " days left until the password expires"
    }

}