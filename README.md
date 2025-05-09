# 🎨 Color Sorter

## Projekat iz predmeta Mikrokontrolerski sistemi

**ColorSorter** je pametni sistem za automatsko razvrstavanje pločica po bojama. Projekat je razvijen u okviru predmeta *Mikrokontrolerski sistemi* i koristi:
- senzor boja
- servo motor za upravljanje mehanizmom sortiranja
- Wi-Fi modul za bežičnu komunikaciju

## Cilj projekta

Cilj je omogućiti **brzo, precizno i pametno sortiranje pločica** uz real-time komunikaciju i nadzor. Sistem je osmišljen tako da može raditi autonomno, kao i pod nadzorom korisnika putem aplikacije. Sve zahtjeve možete pogledati na **Wiki** stranici - [O projektu](https://github.com/lukavidic/ColorSorter_A/wiki/2.-O-projektu) kao i [dostupne i potrebne komponente](https://github.com/lukavidic/ColorSorter_A/wiki/3.-Dostupne-i-potrebne-komponente).

---

## Realizacija projekta

Razvoj sistema je tekao kroz sledeće faze:

1.  **Kreiranje električne šeme**
   
    Na samom početku kreirana je električna šema, koja prikazuje sve povezane komponente i njihovu funkcionalnu ulogu u realizaciji ovog projekta.
    Detalje možete pronaći na **Wiki** stranici – [Projektovanje električne šeme u alatu KiCAD](https://github.com/lukavidic/ColorSorter_A/wiki/4.-Projektovanje-elektri%C4%8Dne-%C5%A1eme-u-alatu-KiCAD).

2.  **Izrada PCB-a**
   
    Na osnovu električne šeme, dizajnirana je štampana ploča (PCB) koja omogućava stabilno napajanje i povezivanje svih perifernih komponenti.
    PCB sadrži:

      - Mikrokontroler PIC24FJ64GA702 kao centralnu upravljačku jedinicu

      - Pasivne komponente (otpornici, kondenzatori)
  
      - Regulatore napona
  
      - Konektore i pinove za povezivanje sa eksternim uređajima kao što su senzor boja, servo motor te WiFi modul
    
    Više o dizajnu ploče dostupno je na **Wiki** stranici – [Projektovanje PCB-a u alatu KiCAD](https://github.com/lukavidic/ColorSorter_A/wiki/4.-Projektovanje-elektri%C4%8Dne-%C5%A1eme-u-alatu-KiCAD).

3.  **Programiranje mikrokontrolera**
   
    Nakon hardverske realizacije, implementiran je kod za rad uređaja – uključujući očitavanje boja, kontrolu motora i Wi-Fi komunikaciju, čiju dokumentaciju kompletnog koda možete pronaći na **Wiki** stranici - [Programiranje mikrokontrolera](https://github.com/lukavidic/ColorSorter_A/wiki/6.-Programiranje-mikrokontrolera).

4. **Izrada aplikacije**

Nakon uspješnog programiranja mikrokontrolerske pločice, pristupili smo razvoju desktop aplikacije koja omogućava nadzor i upravljanje procesom razvrstavanja boja. Aplikacija komunicira sa uređajem putem WiFi veze i omogućava korisniku da prati status sistema u realnom vremenu, šalje komande za pokretanje i resetovanje, te definiše pravila sortiranja po bojama. O zahtjevima aplikacije možete pogledati na **Wiki** stranici - [Firmware i aplikacija](https://github.com/lukavidic/ColorSorter_A/wiki/7.-Firmware-i-aplikacija).

5.  **Testiranje i kalibracija**
   
    Sistem je prolazio kroz više faza testiranja kako bi se obezbijedila tačnost u detekciji boja i pravilno sortiranje. Kratak pogled u testiranje komponenata možete pronaći na **Wiki** stranici - [Testiranje rada komponenata](https://github.com/lukavidic/ColorSorter_A/wiki/7.-Testiranje-rada-komponenata).



![colorsorter](https://github.com/user-attachments/assets/27c8654b-a978-4f76-9400-00ed7e32b33c)

---

## Autori
- Ana Vulin  
- Anđela Savičić  
- Luka Zeljko
