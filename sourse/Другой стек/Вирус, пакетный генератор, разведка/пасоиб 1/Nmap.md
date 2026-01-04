└─$ # Базовое сканирование с определением версий сервисов
nmap -sV -sC www.unimas.my
nmap -sV -sC 49.50.236.20

```
# Стандартное сканирование всех TCP-портов (медленное, но полное)
nmap -p- 49.50.236.20

# Быстрое сканирование самых популярных портов
nmap -F www.unimas.my

# Сканирование с определением ОС (требует прав root)
sudo nmap -O 49.50.236.20

# Проверка на уязвимости с использованием скриптов NSE
nmap --script vuln 49.50.236.20
nmap --script http-sql-injection www.unimas.my

Starting Nmap 7.95 ( https://nmap.org ) at 2025-11-20 13:12 EST
Nmap scan report for www.unimas.my (49.50.236.20)
Host is up (0.23s latency).
rDNS record for 49.50.236.20: 20-subnet236.unimas.my
Not shown: 995 closed tcp ports (reset)
PORT    STATE    SERVICE      VERSION
80/tcp  open     http
| fingerprint-strings: 
|   GetRequest, HTTPOptions: 
|     HTTP/1.1 302 Found 
|     Connection: close
|     Content-Type: text/html
|     X-Frame-Options: SAMEORIGIN
|     X-XSS-Protection: 1; mode=block
|     X-Content-Type-Options: nosniff
|     Location: https://10.224.202.195/
|     Content-Length: 169
|     <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
|     <HTML><HEAD>
|     <TITLE>302 Found </TITLE>
|     </HEAD><BODY>
|     <H1>The Document has moved</H1></BODY></HTML>
|   RTSPRequest: 
|     HTTP/1.1 400 Invalid Request
|     Date: Thu, 20 Nov 2025 18:13:25 GMT
|     X-Frame-Options: SAMEORIGIN
|     X-XSS-Protection: 1; mode=block
|     X-Content-Type-Options: nosniff
|     Connection: close
|     Content-Length: 87
|     Content-Type: text/html
|     <html><head><title>400 Invalid Request</title></head><body>Invalid Request: ??</body>
|   SIPOptions: 
|     HTTP/1.1 400 Invalid Request
|     Date: Thu, 20 Nov 2025 18:14:42 GMT
|     X-Frame-Options: SAMEORIGIN
|     X-XSS-Protection: 1; mode=block
|     X-Content-Type-Options: nosniff
|     Connection: close
|     Content-Length: 87
|     Content-Type: text/html
|_    <html><head><title>400 Invalid Request</title></head><body>Invalid Request: ??</body>
135/tcp filtered msrpc
139/tcp filtered netbios-ssn
443/tcp open     ssl/http     nginx 1.18.0 (Ubuntu)
| ssl-cert: Subject: commonName=*.unimas.my
| Subject Alternative Name: DNS:*.unimas.my, DNS:unimas.my
| Not valid before: 2025-07-22T00:00:00
|_Not valid after:  2026-08-22T23:59:59
|_http-server-header: nginx/1.18.0 (Ubuntu)
|_ssl-date: 2025-11-20T18:16:12+00:00; 0s from scanner time.
445/tcp filtered microsoft-ds
1 service unrecognized despite returning data. If you know the service/version, please submit the following fingerprint at https://nmap.org/cgi-bin/submit.cgi?new-service :
SF-Port80-TCP:V=7.95%I=7%D=11/20%Time=691F5A44%P=x86_64-pc-linux-gnu%r(Get
SF:Request,183,"HTTP/1\.1\x20302\x20Found\x20\r\nConnection:\x20close\r\nC
SF:ontent-Type:\x20text/html\r\nX-Frame-Options:\x20SAMEORIGIN\r\nX-XSS-Pr
SF:otection:\x201;\x20mode=block\r\nX-Content-Type-Options:\x20nosniff\r\n
SF:Location:\x20https://10\.224\.202\.195/\r\nContent-Length:\x20169\r\n\r
SF:\n<!DOCTYPE\x20HTML\x20PUBLIC\x20\"-//W3C//DTD\x20HTML\x204\.0\x20Trans
SF:itional//EN\">\r\n<HTML><HEAD>\r\n<TITLE>302\x20Found\x20</TITLE>\r\n</
SF:HEAD><BODY>\r\n<H1>The\x20Document\x20has\x20moved</H1></BODY></HTML>\r
SF:\n\r\n")%r(HTTPOptions,183,"HTTP/1\.1\x20302\x20Found\x20\r\nConnection
SF::\x20close\r\nContent-Type:\x20text/html\r\nX-Frame-Options:\x20SAMEORI
SF:GIN\r\nX-XSS-Protection:\x201;\x20mode=block\r\nX-Content-Type-Options:
SF:\x20nosniff\r\nLocation:\x20https://10\.224\.202\.195/\r\nContent-Lengt
SF:h:\x20169\r\n\r\n<!DOCTYPE\x20HTML\x20PUBLIC\x20\"-//W3C//DTD\x20HTML\x
SF:204\.0\x20Transitional//EN\">\r\n<HTML><HEAD>\r\n<TITLE>302\x20Found\x2
SF:0</TITLE>\r\n</HEAD><BODY>\r\n<H1>The\x20Document\x20has\x20moved</H1><
SF:/BODY></HTML>\r\n\r\n")%r(RTSPRequest,13B,"HTTP/1\.1\x20400\x20Invalid\
SF:x20Request\r\nDate:\x20Thu,\x2020\x20Nov\x202025\x2018:13:25\x20GMT\r\n
SF:X-Frame-Options:\x20SAMEORIGIN\r\nX-XSS-Protection:\x201;\x20mode=block
SF:\r\nX-Content-Type-Options:\x20nosniff\r\nConnection:\x20close\r\nConte
SF:nt-Length:\x2087\r\nContent-Type:\x20text/html\r\n\r\n<html><head><titl
SF:e>400\x20Invalid\x20Request</title></head><body>Invalid\x20Request:\x20
SF:\?\?</body>\r\n")%r(SIPOptions,13B,"HTTP/1\.1\x20400\x20Invalid\x20Requ
SF:est\r\nDate:\x20Thu,\x2020\x20Nov\x202025\x2018:14:42\x20GMT\r\nX-Frame
SF:-Options:\x20SAMEORIGIN\r\nX-XSS-Protection:\x201;\x20mode=block\r\nX-C
SF:ontent-Type-Options:\x20nosniff\r\nConnection:\x20close\r\nContent-Leng
SF:th:\x2087\r\nContent-Type:\x20text/html\r\n\r\n<html><head><title>400\x
SF:20Invalid\x20Request</title></head><body>Invalid\x20Request:\x20\?\?</b
SF:ody>\r\n");
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 194.49 seconds
Starting Nmap 7.95 ( https://nmap.org ) at 2025-11-20 13:16 EST
Nmap scan report for 20-subnet236.unimas.my (49.50.236.20)
Host is up (0.23s latency).
Not shown: 995 closed tcp ports (reset)
PORT    STATE    SERVICE      VERSION
80/tcp  open     http
| fingerprint-strings: 
|   GetRequest, HTTPOptions: 
|     HTTP/1.1 302 Found 
|     Connection: close
|     Content-Type: text/html
|     X-Frame-Options: SAMEORIGIN
|     X-XSS-Protection: 1; mode=block
|     X-Content-Type-Options: nosniff
|     Location: https://10.224.202.195/
|     Content-Length: 169
|     <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
|     <HTML><HEAD>
|     <TITLE>302 Found </TITLE>
|     </HEAD><BODY>
|     <H1>The Document has moved</H1></BODY></HTML>
|   RTSPRequest: 
|     HTTP/1.1 400 Invalid Request
|     Date: Thu, 20 Nov 2025 18:16:39 GMT
|     X-Frame-Options: SAMEORIGIN
|     X-XSS-Protection: 1; mode=block
|     X-Content-Type-Options: nosniff
|     Connection: close
|     Content-Length: 87
|     Content-Type: text/html
|     <html><head><title>400 Invalid Request</title></head><body>Invalid Request: ??</body>
|   SIPOptions: 
|     HTTP/1.1 400 Invalid Request
|     Date: Thu, 20 Nov 2025 18:17:56 GMT
|     X-Frame-Options: SAMEORIGIN
|     X-XSS-Protection: 1; mode=block
|     X-Content-Type-Options: nosniff
|     Connection: close
|     Content-Length: 87
|     Content-Type: text/html
|_    <html><head><title>400 Invalid Request</title></head><body>Invalid Request: ??</body>
135/tcp filtered msrpc
139/tcp filtered netbios-ssn
443/tcp open     ssl/http     nginx 1.18.0 (Ubuntu)
|_http-server-header: nginx/1.18.0 (Ubuntu)
|_ssl-date: 2025-11-20T18:19:26+00:00; 0s from scanner time.
| ssl-cert: Subject: commonName=*.unimas.my
| Subject Alternative Name: DNS:*.unimas.my, DNS:unimas.my
| Not valid before: 2025-07-22T00:00:00
|_Not valid after:  2026-08-22T23:59:59
445/tcp filtered microsoft-ds
1 service unrecognized despite returning data. If you know the service/version, please submit the following fingerprint at https://nmap.org/cgi-bin/submit.cgi?new-service :
SF-Port80-TCP:V=7.95%I=7%D=11/20%Time=691F5B06%P=x86_64-pc-linux-gnu%r(Get
SF:Request,183,"HTTP/1\.1\x20302\x20Found\x20\r\nConnection:\x20close\r\nC
SF:ontent-Type:\x20text/html\r\nX-Frame-Options:\x20SAMEORIGIN\r\nX-XSS-Pr
SF:otection:\x201;\x20mode=block\r\nX-Content-Type-Options:\x20nosniff\r\n
SF:Location:\x20https://10\.224\.202\.195/\r\nContent-Length:\x20169\r\n\r
SF:\n<!DOCTYPE\x20HTML\x20PUBLIC\x20\"-//W3C//DTD\x20HTML\x204\.0\x20Trans
SF:itional//EN\">\r\n<HTML><HEAD>\r\n<TITLE>302\x20Found\x20</TITLE>\r\n</
SF:HEAD><BODY>\r\n<H1>The\x20Document\x20has\x20moved</H1></BODY></HTML>\r
SF:\n\r\n")%r(HTTPOptions,183,"HTTP/1\.1\x20302\x20Found\x20\r\nConnection
SF::\x20close\r\nContent-Type:\x20text/html\r\nX-Frame-Options:\x20SAMEORI
SF:GIN\r\nX-XSS-Protection:\x201;\x20mode=block\r\nX-Content-Type-Options:
SF:\x20nosniff\r\nLocation:\x20https://10\.224\.202\.195/\r\nContent-Lengt
SF:h:\x20169\r\n\r\n<!DOCTYPE\x20HTML\x20PUBLIC\x20\"-//W3C//DTD\x20HTML\x
SF:204\.0\x20Transitional//EN\">\r\n<HTML><HEAD>\r\n<TITLE>302\x20Found\x2
SF:0</TITLE>\r\n</HEAD><BODY>\r\n<H1>The\x20Document\x20has\x20moved</H1><
SF:/BODY></HTML>\r\n\r\n")%r(RTSPRequest,13B,"HTTP/1\.1\x20400\x20Invalid\
SF:x20Request\r\nDate:\x20Thu,\x2020\x20Nov\x202025\x2018:16:39\x20GMT\r\n
SF:X-Frame-Options:\x20SAMEORIGIN\r\nX-XSS-Protection:\x201;\x20mode=block
SF:\r\nX-Content-Type-Options:\x20nosniff\r\nConnection:\x20close\r\nConte
SF:nt-Length:\x2087\r\nContent-Type:\x20text/html\r\n\r\n<html><head><titl
SF:e>400\x20Invalid\x20Request</title></head><body>Invalid\x20Request:\x20
SF:\?\?</body>\r\n")%r(SIPOptions,13B,"HTTP/1\.1\x20400\x20Invalid\x20Requ
SF:est\r\nDate:\x20Thu,\x2020\x20Nov\x202025\x2018:17:56\x20GMT\r\nX-Frame
SF:-Options:\x20SAMEORIGIN\r\nX-XSS-Protection:\x201;\x20mode=block\r\nX-C
SF:ontent-Type-Options:\x20nosniff\r\nConnection:\x20close\r\nContent-Leng
SF:th:\x2087\r\nContent-Type:\x20text/html\r\n\r\n<html><head><title>400\x
SF:20Invalid\x20Request</title></head><body>Invalid\x20Request:\x20\?\?</b
SF:ody>\r\n");
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 193.71 seconds
Starting Nmap 7.95 ( https://nmap.org ) at 2025-11-20 13:19 EST
Stats: 0:01:37 elapsed; 0 hosts completed (1 up), 1 undergoing SYN Stealth Scan
SYN Stealth Scan Timing: About 8.29% done; ETC: 13:38 (0:17:53 remaining)
Stats: 0:01:39 elapsed; 0 hosts completed (1 up), 1 undergoing SYN Stealth Scan
SYN Stealth Scan Timing: About 8.41% done; ETC: 13:39 (0:17:58 remaining)
Stats: 0:01:40 elapsed; 0 hosts completed (1 up), 1 undergoing SYN Stealth Scan
SYN Stealth Scan Timing: About 8.47% done; ETC: 13:39 (0:18:00 remaining)
Stats: 0:17:51 elapsed; 0 hosts completed (1 up), 1 undergoing SYN Stealth Scan
SYN Stealth Scan Timing: About 99.99% done; ETC: 13:37 (0:00:00 remaining)
Nmap scan report for 20-subnet236.unimas.my (49.50.236.20)
Host is up (0.25s latency).
Not shown: 65525 closed tcp ports (reset)
PORT     STATE    SERVICE
80/tcp   open     http
135/tcp  filtered msrpc
137/tcp  filtered netbios-ns
138/tcp  filtered netbios-dgm
139/tcp  filtered netbios-ssn
442/tcp  filtered cvc_hostd
443/tcp  open     https
445/tcp  filtered microsoft-ds
830/tcp  filtered netconf-ssh
4430/tcp open     rsqlserver

Nmap done: 1 IP address (1 host up) scanned in 1089.91 seconds
Starting Nmap 7.95 ( https://nmap.org ) at 2025-11-20 13:37 EST
Nmap scan report for www.unimas.my (49.50.236.20)
Host is up (0.26s latency).
rDNS record for 49.50.236.20: 20-subnet236.unimas.my
Not shown: 95 closed tcp ports (reset)
PORT    STATE    SERVICE
80/tcp  open     http
135/tcp filtered msrpc
139/tcp filtered netbios-ssn
443/tcp open     https
445/tcp filtered microsoft-ds

Nmap done: 1 IP address (1 host up) scanned in 6.18 seconds
[sudo] password for kali: 
Starting Nmap 7.95 ( https://nmap.org ) at 2025-11-20 13:37 EST
Stats: 0:00:20 elapsed; 0 hosts completed (1 up), 1 undergoing SYN Stealth Scan
SYN Stealth Scan Timing: About 52.85% done; ETC: 13:38 (0:00:19 remaining)
Nmap scan report for 20-subnet236.unimas.my (49.50.236.20)
Host is up (0.25s latency).
Not shown: 995 closed tcp ports (reset)
PORT    STATE    SERVICE
80/tcp  open     http
135/tcp filtered msrpc
139/tcp filtered netbios-ssn
443/tcp open     https
445/tcp filtered microsoft-ds
OS fingerprint not ideal because: Didn't receive UDP response. Please try again with -sSU
No OS matches for host

OS detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 44.99 seconds
Starting Nmap 7.95 ( https://nmap.org ) at 2025-11-20 13:38 EST
Pre-scan script results:
| broadcast-avahi-dos: 
|   Discovered hosts:
|     224.0.0.251
|   After NULL UDP avahi packet DoS (CVE-2011-1002).
|_  Hosts are all up (not vulnerable).
Nmap scan report for 20-subnet236.unimas.my (49.50.236.20)
Host is up (0.25s latency).
Not shown: 995 closed tcp ports (reset)
PORT    STATE    SERVICE
80/tcp  open     http
|_http-aspnet-debug: ERROR: Script execution failed (use -d to debug)
|_http-csrf: Couldn't find any CSRF vulnerabilities.
|_http-stored-xss: Couldn't find any stored XSS vulnerabilities.
|_http-dombased-xss: Couldn't find any DOM based XSS.
|_http-vuln-cve2014-3704: ERROR: Script execution failed (use -d to debug)
135/tcp filtered msrpc
139/tcp filtered netbios-ssn
443/tcp open     https
|_http-dombased-xss: Couldn't find any DOM based XSS.
|_http-vuln-cve2014-3704: ERROR: Script execution failed (use -d to debug)
|_http-stored-xss: Couldn't find any stored XSS vulnerabilities.
|_http-aspnet-debug: ERROR: Script execution failed (use -d to debug)
|_http-csrf: Couldn't find any CSRF vulnerabilities.
445/tcp filtered microsoft-ds

Nmap done: 1 IP address (1 host up) scanned in 102.95 seconds
Starting Nmap 7.95 ( https://nmap.org ) at 2025-11-20 13:40 EST
Nmap scan report for www.unimas.my (49.50.236.20)
Host is up (0.25s latency).
rDNS record for 49.50.236.20: 20-subnet236.unimas.my
Not shown: 997 closed tcp ports (reset)
PORT    STATE    SERVICE
135/tcp filtered msrpc
139/tcp filtered netbios-ssn
445/tcp filtered microsoft-ds




nmap -T4 -n -Pn -sS -p- 49.50.236.20   
Starting Nmap 7.95 ( https://nmap.org ) at 2025-11-20 16:26 EST
Warning: 49.50.236.20 giving up on port because retransmission cap hit (6).
Nmap scan report for 49.50.236.20
Host is up (0.25s latency).
Not shown: 65525 closed tcp ports (reset)
PORT     STATE    SERVICE
80/tcp   open     http
135/tcp  filtered msrpc
137/tcp  filtered netbios-ns
138/tcp  filtered netbios-dgm
139/tcp  filtered netbios-ssn
442/tcp  filtered cvc_hostd
443/tcp  open     https
445/tcp  filtered microsoft-ds
830/tcp  filtered netconf-ssh
4430/tcp open     rsqlserver

Nmap done: 1 IP address (1 host up) scanned in 609.86 seconds

```