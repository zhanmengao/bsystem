    $LocalHostName = [System.Net.DNS]::GetHostByName('') | ForEach-Object { $_.HostName }
    $SessionHost = $LocalHostName
    $CollectionName = "QuickCollection"
    New-RDSessionCollection `
        -CollectionName $CollectionName `
        -SessionHost $SessionHost `
        -CollectionDescription $CollectionDescription `
        -ErrorAction SilentlyContinue