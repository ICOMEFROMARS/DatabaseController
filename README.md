* Alper abinin isteği üzerine arayüz başlangıçta seçilen .db'ye göre şekillenebilir hale getirildi.
* Arayüz başlarken kullanabilmek için iki adet .db yüklendi, bunlardan biri veya başka bir .db seçilerek deneme yapılabilir.
* Thread ile çalıştığını gösterebilmek için insert_data fonksiyonuna sleep eklendi.(DatabaseController.cpp / 56)
* Add butonu ile insert_data aktif edilir, 10 saniyelik ekleme süreci devam ederken kullanıcı diğer tablo elemanları için delete veya update yapabilir.
