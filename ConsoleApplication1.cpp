#include <Windows.h>
#include <iostream>

int main() {
    // O caminho para o arquivo do driver (.sys) 
    LPCWSTR driverPath = L"C:\\Windows\\System32\\WinHttpDriver.sys";

    // Carregando o driver usando a API CreateService
    SC_HANDLE scManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (!scManager) {
        std::cerr << "Falha ao abrir o Gerenciador de Controle de Serviço. Erro: " << GetLastError() << std::endl;
        return 1;
    }

    SC_HANDLE service = CreateService(
        scManager,
        L"WinHttpDriver",    // Nome do serviço
        L"WinHttpDriver", // Display Name
        SERVICE_ALL_ACCESS,
        SERVICE_KERNEL_DRIVER,
        SERVICE_BOOT_START,
        SERVICE_ERROR_IGNORE,
        driverPath,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    if (!service) {
        std::cerr << "Falha ao criar o serviço. Erro: " << GetLastError() << std::endl;
        CloseServiceHandle(scManager);
        return 1;
    }

    std::cout << "Serviço criado com sucesso." << std::endl;

    // Iniciando o serviço
    if (!StartService(service, 0, nullptr)) {
        std::cerr << "Falha ao iniciar o serviço. Erro: " << GetLastError() << std::endl;
        CloseServiceHandle(service);
        CloseServiceHandle(scManager);
        return 1;
    }

    std::cout << "Serviço iniciado com sucesso." << std::endl;

    // Fechando as alças de serviço e Gerenciador de Controle de Serviço
    CloseServiceHandle(service);
    CloseServiceHandle(scManager);

    return 0;
}
