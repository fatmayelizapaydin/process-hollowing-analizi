Adım 1: Kurulum ve Kaynak Kod Analizi (Reverse Engineering)

Analiz: Projenin C++ kaynak kodları, Windows.h kütüphanesi ve ntdll.dll üzerindeki düşük seviyeli API çağrıları (NtUnmapViewOfSection vb.) üzerinden analiz edilmiştir.

Kritik Soru: Kodun derlenme aşamasında statik olarak bağlandığı kütüphanelerin güvenilirliği ve PE (Portable Executable) yapısı üzerindeki etkileri incelenmiştir.

Adım 2: İzolasyon ve İz Bırakmadan Temizlik (Forensics)

Analiz: Geliştirilen araç, "Fileless Malware" (Dosyasız Zararlı) mantığında çalıştığı için diskte kalıcı bir dosya bırakmaz.

İspat: Analiz tamamlandığında, enjekte edilen sürecin (Örn: notepad.exe) bellek alanı (RAM) temizlenerek sistemin orijinal haline döndüğü Process Hacker ve Process Monitor araçlarıyla doğrulanmıştır.

Adım 3: İş Akışları ve CI/CD Yapısı

Analiz: Projenin GitHub üzerindeki ana dizini ve dosya yapısı incelenmiş, kodun bütünlüğü commit hash takibiyle sağlanmıştır. Geliştirme sürecinde güvenli kod yazım standartları (Secure Coding) uygulanmıştır.

Adım 4: Docker Mimarisi ve Sanallaştırma Güvenliği

Analiz: Bu tip "Memory Injection" tekniklerinin analizi, ana sistemi tehlikeye atmamak adına izole bir sanal makinede (VM) veya güvenli bir sanal ortamda gerçekleştirilmiştir. Konteyner güvenliği açısından, sürecin sistem yetkileri (Privileges) sınırlandırılmıştır.

Adım 5: Tehdit Modelleme ve Akış Analizi (Threat Modeling)

Analiz: Uygulamanın başlangıç noktası (Entry Point) manipüle edilerek, meşru bir sürecin kimliği arkasına gizlenme (Masquerading) tekniği simüle edilmiştir.

Kritik Soru: Bir saldırgan bu tekniği kullanarak Antivirüs (AV) ve EDR çözümlerini nasıl atlatabilir? Bu sorunun cevabı, WriteProcessMemory ve ResumeThread fonksiyonlarının çalışma mantığıyla teknik olarak açıklanmıştır.
