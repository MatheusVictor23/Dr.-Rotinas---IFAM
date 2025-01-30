# Script para compilar o projeto
Write-Host "Compilando o projeto..."

gcc -Wall -g -O2 -Iinclude src/main/*.c src/admin/*.c src/financeiro/*.c src/login/*.c src/medico/*.c src/recepcionista/*.c -o output/clinica.exe

if ($?) {
    Write-Host "Compilação concluída com sucesso! Executável gerado: output/clinica.exe"
} else {
    Write-Host "Erro durante a compilação."
}

# CASO HAJA RESTRIÇÃO DE EXECUÇÃO DE SCRIPTS NO POWERSHELL, EXECUTE O COMANDO ABAIXO
# COPIAR COMANDO APÓS O "#" E COLAR NO POWERSHELL

# powershell -ExecutionPolicy Bypass -File .\compilar.ps1