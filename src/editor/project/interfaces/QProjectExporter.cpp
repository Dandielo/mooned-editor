////// ConsoleApplication2.cpp : Defines the entry point for the console application.
//////
////
////#include <iostream>
////#include <vector>
////#include <string>
////#include <cassert>
////
////using namespace std;
////
////namespace obliczenia
////{
////
////int roznica_godzinowa(int od_godz, int do_godz)
////{
////    int wynik;
////    if (od_godz > do_godz) // Zakładamy że przekroczyliśmy 24h i jesteśmy w kolejnym dniu
////    {
////        // Suma (od 'od_godz' do 24.00) oraz (od 0.00 do 'do_godz')
////        wynik = (24 - od_godz) + (0 + do_godz);
////    }
////    else
////    {
////        // Roznica 'do_godz' oraz 'od_godz' to nasz wynik
////        wynik = do_godz - od_godz;
////    }
////    return wynik;
////}
////
////int dodaj_godziny(int godz_startowa, int przesuniencie)
////{
////    int wynik = godz_startowa + przesuniencie;
////    return wynik % 24; // Wystarczy modulo żeby porawić wynik jeżeli wyjdziemy poza dobę
////}
////
////bool zawiera_sie_w_godzinach(int godzina_aktualna, int zawiera_od, int zawiera_do)
////{
////    bool wynik;
////
////    assert(godzina_aktualna <= 24); // Akceptujemy tylko wartości max do 24
////
////    // Jezeli mamy godziny miedzy kolejnymi dniami to sprawdzamy inaczej niz miedzy godzinamy w tym samym dniu
////    if (zawiera_od > zawiera_do) // np. (22 wieczorem) > (6 rano)
////    {
////        wynik = (godzina_aktualna >= zawiera_od) || (godzina_aktualna <= zawiera_do);
////    }
////    else
////    {
////        wynik = (godzina_aktualna >= zawiera_od) && (godzina_aktualna <= zawiera_do);
////    }
////
////    return wynik;
////}
////
////}
////
////class GodzinyPracyUrzadzenia;
////
////class Taryfa
////{
////public:
////    enum class Typ { JednoStrefowa, DwuStrefowa, Weekendowa };
////
////    Taryfa(Typ typ, float stawka_1, float stawka_2)
////        : _typ{ typ }
////        , _stawka_1{ stawka_1 }
////        , _stawka_2{ stawka_2 }
////    { }
////
////    float daj_stawke(int t) const
////    {
////        if (obliczenia::zawiera_sie_w_godzinach(t, 23, 6) || obliczenia::zawiera_sie_w_godzinach(t, 14, 14))
////        {
////            return _stawka_2;
////        }
////        else
////        {
////            return _stawka_1;
////        }
////    }
////
////    float oblicz_koszt(const GodzinyPracyUrzadzenia& godziny_pracy) const;
////
////private:
////    float _stawka_1; //powinno byc w private ale trzeba zrobic metode do uzywania
////    float _stawka_2;
////    Typ _typ;
////};
////
////
////class Dostawca
////{
////public:
////    Dostawca(std::string nazwa)
////        : _nazwa{ std::move(nazwa) }
////    { };
////
////    void dodaj_taryfe(Taryfa taryfa)
////    {
////        _taryfy.push_back(std::move(taryfa));
////    }
////
////    //const std::vector<Taryfa>& wszystkie_taryfy() const
////    auto wszystkie_taryfy() const -> const std::vector<Taryfa>&
////    {
////        return _taryfy;
////    }
////
////private:
////    std::string _nazwa;
////    std::vector<Taryfa> _taryfy;
////};
////
////class Urzadzenie
////{
////public:
////    Urzadzenie(std::string nazwa, float pobierana_moc)
////        : _nazwa{ std::move(nazwa) }
////        , _pobierana_moc{ pobierana_moc }
////    { }
////
////    float pobierana_moc() const
////    {
////        return _pobierana_moc;
////    }
////
////private:
////    std::string _nazwa;
////    float _pobierana_moc;
////};
////
////class GodzinyPracyUrzadzenia
////{
////public:
////    GodzinyPracyUrzadzenia(Urzadzenie& urzadzenie, int czas_pracy, int praca_od = 0)
////        : _urzadzenie{ urzadzenie }
////        , _praca_od{ praca_od }
////        , _praca_do{ obliczenia::dodaj_godziny(_praca_od, czas_pracy) } // Uzywamy funkcji tak że czas pracy do 22.00 o dlugosci 4h zwroci wynik 'praca_do' 2.00 a nie 26.00
////    { }
////
////    int ustaw_godzine_rozpoczecia(int praca_od)
////    {
////        // Zapamietajmy aktualna dlugosc pracy
////        int dlugosc_pracy = ile_pracuje();
////
////        _praca_od = praca_od;
////        _praca_do = obliczenia::dodaj_godziny(_praca_od, dlugosc_pracy); // Uzywamy funkcji tak że czas pracy do 22.00 o dlugosci 4h zwroci wynik 'praca_do' 2.00 a nie 26.00
////    }
////
////    int godzina_rozpoczecia() const
////    {
////        return _praca_od;
////    }
////
////    int godzina_zakonczenia() const
////    {
////        return _praca_do;
////    }
////
////    int ile_pracuje() const
////    {
////        return obliczenia::roznica_godzinowa(_praca_od, _praca_do);
////    }
////
////    Urzadzenie& urzadzenie() const
////    {
////        return _urzadzenie;
////    }
////
////private:
////    Urzadzenie& _urzadzenie;
////    int _praca_od;
////    int _praca_do;
////};
////
////// Definicja funkcji dopiero pod klasą 'GodzinyPracyUrzadzenia' ponieważ jest ona nam potrzebna
////float Taryfa::oblicz_koszt(const GodzinyPracyUrzadzenia& godziny_pracy) const
////{
////    int praca_od = godziny_pracy.godzina_rozpoczecia();
////    int praca_do = godziny_pracy.godzina_zakonczenia();
////
////    float wynik = 0.0f;
////    for (; praca_od < praca_do; ++praca_od)
////    {
////        wynik += daj_stawke(praca_od) *
////    }
////
////    return daj_stawke(godziny_pracy.godzina_rozpoczecia()) * godziny_pracy.urzadzenie().pobierana_moc();
////}
////
//////class Urzadzenie
//////{
//////private:
//////
//////public:
//////	string _nazwa;
//////	int _ile_pracuje;
//////	int _pobrana_moc;
//////	//wybrane godziny
//////	int _praca_od;
//////	int _praca_do;
//////
//////	Urzadzenie()
//////	{
//////		_ile_pracuje = 0;
//////		_pobrana_moc = 0;
//////		_praca_do = 0;
//////		_praca_od = 0;
//////	}
//////
//////	Urzadzenie(string nazwa, float ile_pracuje, int pobrana_moc);
//////	void ustaw_godziny(int _praca_od2, int _praca_do2)
//////	{
//////		_praca_od = _praca_od2;
//////		_praca_do = _praca_do2; //jak to ustawić??
//////	}
//////
//////};
////
////struct RoczneKoszta
////{
////public:
////    Dostawca* dostawca;
////    Taryfa* taryfa;
////    double koszty;
////    //RoczneKoszta(Dostawca dostawca = NULL,Taryfa& taryfa = NULL,double  koszty = 0.0);
////
////    RoczneKoszta(Dostawca* d, Taryfa* t, double k)
////    {
////        dostawca = d;
////        taryfa = t;
////        koszty = k;
////    }
////};
////
////class Uzytkownik
////{
////private:
////    string _imie;
////    int _praca_od;
////    int _praca_do;
////    int _sen_od;
////    int _sen_do;
////
////    std::vector<GodzinyPracyUrzadzenia> _urzadzenia;
////    std::vector<RoczneKoszta> _roczneKoszta;
////
////public:
////    Uzytkownik()
////    {
////        _imie = "domyslne";
////        _praca_do = 16;
////        _praca_od = 8;
////        _sen_do = 7;
////        _sen_od = 1;
////    }
////    //ustawienie parametrow odbiorcy
////    Uzytkownik(string imie, int praca_od_godz, int praca_do_godz, int sen_od, int sen_do);
////    void setUzytkownik(string imie, int praca_od_godz, int praca_do_godz, int sen_od, int sen_do)
////    {
////        _imie = imie;
////        _praca_do = praca_do_godz;
////        _praca_od = praca_od_godz;
////        _sen_do = sen_do;
////        _sen_od = sen_od;
////    }
////    void setImie(string imie)
////    {
////        _imie = imie;
////    }
////    void dodajUrzadzenie(Urzadzenie& urzadzenie, int czas_pracy)
////    {
////        _urzadzenia.push_back(GodzinyPracyUrzadzenia{ urzadzenie, czas_pracy, 0 /* nie wiemy jeszcze kiedy najlepiej ustawić czas pracy */ });
////    }
////
////
////    //funkcja zwracajaca ROCZNY koszt energii przy korzystaniu z podanych urządzeń
////    //w najtańszych godzinach oraz wybranego dostawce i taryfę
////    std::vector<RoczneKoszta> sprawdz_taryfy(Dostawca& dostawca)
////    {
////        //RoczneKoszta koszta =
////        float min = 0;
////        int od_h;
////        int do_h;
////        float wynik = 0;
////
////
////        //wyszukanie najtanszej taryfy podanego dostawcy
////        std::vector<RoczneKoszta> wyniki;
////
////        //zakłądam ze urzadzenia pracuja tylko ,gdy odbiorca jest w domu i nie spi xd
////        //bo wtedy praca tv, grzejnika i pralki ma sens
////        //+ jest bardziej ograniczony zakres pracy
////        for (auto& taryfa : dostawca.wszystkie_taryfy())
////        {
////            double koszt_urzadzen = 0;
////
////
////            //chce sprawdzić wszystkie możliwe okresy działania urządzenia
////            //gdy np. uzytkownik spi do 6 a prace zaczyna od 12 i urządzenie pracuje 2 godziny to:
////            // cena za prace od 6 do 8, cena za 7-9, 8-10 itd. i wybranie najmniejszej z nich
////            for (auto& urzadzenie : _urzadzenia)
////            {
////                //sprawdzenie okresu przed pojsciem do pracy
////                for (int l = _sen_do; l < _praca_od; l++)
////                {
////
////                    if (_sen_do + urzadzenie.ile_pracuje() <= _praca_od)
////                    {
////                        wynik += taryfa.oblicz_koszt(urzadzenie);
////
////                        //przypisanie poczatkowej wartosci minimalnej
////                        min = wynik;
////                        wynik = 0;
////                        od_h = _sen_do;
////                        do_h = _sen_do + urzadzenie.ile_pracuje();
////                    }
////                }
////                //dalsze sprawdzanie okresu przed pojsciem do pracy
////                for (int l = _sen_do + 1; l < _praca_od; l++)
////                {
////                    //warunek po to by praca urządzenia nie konczyla sie jak go nie ma w domu
////                    //zeby tv sie samo nie ogladało i pranie nie gniło w pralce xd
////                    if (l + u->_ile_pracuje <= _praca_od)
////                    {
////                        for (int k = l; k < (l + u->_ile_pracuje); k++)
////                            wynik = wynik + t->_pobierz_stawke(k)*u->_pobrana_moc;
////
////                        if (wynik < min)
////                        {
////                            min = wynik;
////                            od_h = _sen_do + 1;
////                            do_h = od_h + u->_ile_pracuje;
////                        }
////
////                    }
////                    for (int l = _praca_do; l < _sen_od; l++)
////                    {
////
////                        if (l + u->_ile_pracuje <= _sen_od)
////                        {
////                            for (int k = l; k < (l + u->_ile_pracuje); k++)
////                            {
////                                wynik = wynik + t->_pobierz_stawke(k)*u->_pobrana_moc;
////                            }
////
////                            if (wynik < min)
////                            {
////                                min = wynik;
////                                od_h = _sen_do;
////                                do_h = od_h + u->_ile_pracuje;
////                            }
////                        }
////                    }
////                    //wybranie czasu działania urządzenia i zapisanie go w klasie
////                    u->ustaw_godziny(od_h, do_h);
////                }
////                //zsumowanie kosztów urządzenia w zadanej taryfie
////                koszt_urzadzen += min;
////                //trzeba przeliczyć na rok w zaleznosci od tego
////                // ile razy w tygodniu urzadzenie pracuje
////
////            }
////            //stworzenie odpowiedzi dla danego dostawcy
////            RoczneKoszta * test = new RoczneKoszta(d, d->_pobierz_taryfe(i), koszt_urzadzen);
////            _roczneKoszta.push_back(test);
////        }
////        //zwrócenie odp:
////        return _roczneKoszta;
////
////    }
////
////
////};
////
////int _tmain(int argc, _TCHAR* argv[])
////{
////    vector<Dostawca*> dostawcy;
////    Dostawca* dostawca = new Dostawca(string("TPSA"));
////    Taryfa* taryfa_test = new Taryfa();
////    taryfa_test->ustawTaryfe(Taryfa::Typ::JednoStrefowa, 0.2931, 0.2931);
////
////    //stworzenie dostawców:
////    Dostawca* enea = new Dostawca(string("Enea S.A."));
////    Taryfa* jednostrefowa = new Taryfa();
////    jednostrefowa->ustawTaryfe(Taryfa::Typ::JednoStrefowa, 0.2931, 0.2931);
////    Taryfa* dwustrefowa = new Taryfa();
////    dwustrefowa->ustawTaryfe(Taryfa::Typ::DwuStrefowa, 0.3667, 0.1696);
////    Taryfa* weekendowa = new Taryfa();
////    weekendowa->ustawTaryfe(Taryfa::Typ::Weekendowa, 0.4209, 0.1765);
////    enea->dodaj_taryfe(jednostrefowa);
////    enea->dodaj_taryfe(dwustrefowa);
////    enea->dodaj_taryfe(weekendowa);
////    dostawcy.push_back(enea);
////
////    Urzadzenie grzejnik{ "grzejnik", 2250 };
////    Urzadzenie telewizor{ "telewizor", 224.475 };
////    grzejnik->_ile_pracuje = 6;
////    telewizor->_ile_pracuje = 4;
////    //Urzadzenie* pralka = new Urzadzenie();
////    //Urzadzenie* lodowka = new Urzadzenie();
////    //Urzadzenie* odkurzacz = new Urzadzenie();
////
////    //dostawca->dodaj_taryfe(new Taryfa( Taryfa::Typ::JednoStrefowa, 0.2931, 0.2931 ));
////
////    dostawcy.push_back(dostawca);
////    //nie wiem czemu tak nie działa:
////    //Uzytkownik janusz(string("janusz"), 6, 16, 1, 5 );
////    //janusz->setImie("janusz");
////    //Uzytkownik* janusz = new Uzytkownik(imie, 6, 16, 1, 5);
////
////    std::string imie = "Janusz";
////    Uzytkownik* janusz = new Uzytkownik();
////    janusz->setUzytkownik("Janusz", 6, 16, 1, 5);
////    janusz->dodajUrzadzenie(grzejnik, 6);
////    janusz->dodajUrzadzenie(telewizor, 4);
////
////    std::string imie = "Teresa";
////    Uzytkownik* janusz = new Uzytkownik();
////    janusz->setUzytkownik("Teresa", 14, 23, 1, 5);
////    janusz->dodajUrzadzenie(grzejnik, 6);
////    janusz->dodajUrzadzenie(telewizor, 4);
////
////    std::vector<RoczneKoszta> wyniki;
////    for (int i = 0; i < dostawcy.size(); i++)
////    {
////        auto wynik = janusz->sprawdz_taryfy(dostawcy.at(i));
////    }
////
////    //wybierz najlepszy wynik z nich
////    delete grzejnik;
////    delete telewizor;
////    //delete zmywarka;
////    //delete pralka;
////    //delete lodowka;
////    //delete odkurzacz;
////    delete dostawca;
////    delete janusz;
////
////    return 0;
////}
////
//// ConsoleApplication2.cpp : Defines the entry point for the console application.
////
//// ConsoleApplication2.cpp : Defines the entry point for the console application.
////
//
//#include <iostream>
//#include <vector>
//#include <string>
//
//using namespace std;
//
//class Taryfa
//{
//public:
//    enum Typ { JednoStrefowa, DwuStrefowa, Weekendowa };
//
//    // Tworzymy poprawny konstruktor
//    Taryfa(Typ typ, double stawka_1, double stawka_2)
//        : _typ{ typ }
//        , _stawka_1{ stawka_1 }
//        , _stawka_2{ stawka_2 }
//    {
//    }
//
//    int typTaryfy()
//    {
//        return _typ;
//    }
//
//    double _pobierz_stawke(int t)
//    {
//        // Można by to np. opisać tak (zdefiniowałem sobie funkcję)
//        if ((t < 7 || t>22) || (t > 13 && t < 15))
//            return _stawka_2;
//        else
//            return _stawka_1;
//    }
//
//private:
//    double _stawka_1; //powinno byc w private ale trzeba zrobic metode do uzywania
//    double _stawka_2;
//
//    // int _typ; // Ten int został wymuszony bo nie wiedział jeszcze nic o 'enum Typ' więc przenieśmy tą sekcję niżej
//    Typ _typ; // Tak teraz zadziała
//};
//
//
//class Dostawca
//{
//private:
//    std::string _nazwa;
//    double koszt_podpisania;
//    std::vector<Taryfa> _nowe_taryfy; // nie trzymamy wskaźnika
//
//public:
//    std::vector<Taryfa> _stare_taryfy; // Ten vector z tego co widziałem dostaje 'nowe' obiekty o których później zapominamy, więc można by je tutaj usuwać, lub tworzyć dopiero w funkcji 'dodaj_taryfe'
//
//    // pokażę opcję nr2.
//
//    Dostawca()
//    {
//        _nazwa = string("brak");
//        koszt_podpisania = 0;
//    };
//
//    Dostawca(std::string nazwa)
//    {
//        _nazwa = nazwa;
//    };
//
//    void setKosztPodpisania(double i)
//    {
//        koszt_podpisania = i;
//    }
//
//    double getKosztPodpisania()
//    {
//        return koszt_podpisania * 12;
//    }
//
//    string getName()
//    {
//        return _nazwa;
//    }
//
//    void dodaj_taryfe(Taryfa::Typ typ, float stawka_1, float stawka_2)
//    {
//        _nowe_taryfy.push_back(Taryfa{ typ, stawka_1, stawka_2 });
//    }
//
//    // Ta funkcja jest tak naprawdę do usunięcia
//    //  Taryfa* _pobierz_taryfe(int n)
//    // {
//    //        return &_taryfy[n]; // żeby nie psuć reszty użyć, to zwracamy wciąż wskaźnik
//    // }
//
//    // Jeszcze stworzymy funkcję do zwracania taryf bo widze ze potrzebna
//    std::vector<Taryfa> wszystkie_taryfy() const
//    {
//        return _nowe_taryfy;
//    }
//};
//
//
//class Urzadzenie
//{
//private:
//
//public:
//    double _koszt_dzienny;
//    string _nazwa;
//    int _ile_pracuje;
//    double _pobrana_moc;
//    //wybrane godziny
//    int _praca_od;
//    int _praca_do;
//    int _dni_w_roku;
//
//    Urzadzenie()
//    {
//        _ile_pracuje = 0;
//        _pobrana_moc = 0;
//        _praca_do = 0;
//        _praca_od = 0;
//        _dni_w_roku = 365;
//    }
//
//    Urzadzenie(string nazwa, float ile_pracuje, int pobrana_moc);
//    void ustaw_godziny(int _praca_od2, int _praca_do2)
//    {
//        _praca_od = _praca_od2;
//        _praca_do = _praca_do2; //jak to ustawić??
//    }
//
//};
//
//struct RoczneKoszta
//{
//public:
//    Dostawca* dostawca;
//    Taryfa* taryfa;
//    double koszty;
//    //RoczneKoszta(Dostawca dostawca = NULL,Taryfa& taryfa = NULL,double  koszty = 0.0);
//
//    RoczneKoszta(Dostawca* d, Taryfa* t, double k)
//    {
//        dostawca = d;
//        taryfa = t;
//        koszty = k;
//    }
//};
//
//class Uzytkownik
//{
//private:
//    string _imie;
//    int _praca_od;
//    int _praca_do;
//    int _sen_od;
//    int _sen_do;
//
//    std::vector<Urzadzenie *> _urzadzenia;
//    std::vector<RoczneKoszta *> _roczneKoszta;
//
//public:
//    Uzytkownik()
//    {
//        _imie = "domyslne";
//        _praca_do = 16;
//        _praca_od = 8;
//        _sen_do = 7;
//        _sen_od = 1;
//    }
//    //ustawienie parametrow odbiorcy
//    Uzytkownik(string imie, int praca_od_godz, int praca_do_godz, int sen_od, int sen_do);
//    void setUzytkownik(string imie, int praca_od_godz, int praca_do_godz, int sen_od, int sen_do)
//    {
//        _imie = imie;
//        _praca_do = praca_do_godz;
//        _praca_od = praca_od_godz;
//        _sen_do = sen_do;
//        _sen_od = sen_od;
//    }
//
//    void setImie(string imie)
//    {
//        _imie = imie;
//    }
//
//    void pokazWynik()
//    {
//        int min = 0;
//        for (int i = 0; i < _roczneKoszta.size(); i++)
//        {
//            if (_roczneKoszta.at(i)->koszty < _roczneKoszta.at(min)->koszty)
//            {
//                min = i;
//            }
//        }
//        cout << "Najmniejszy koszt dla uzytkownika ";
//        cout << _imie << endl;
//        cout << " jest u dostawcy " << _roczneKoszta.at(min)->dostawca->getName() << endl;
//        cout << ", przy taryfie ";
//        switch (_roczneKoszta.at(min)->taryfa->typTaryfy())
//        {
//        case 0:
//            cout << "JednoStrefowa" << endl;
//            break;
//        case 1:
//            cout << "DwuStrefowa" << endl;
//            break;
//        case 2:
//            cout << "Weekendowa" << endl;
//            break;
//        default:
//            break;
//        }
//        cout << ", koszta wynosza " << _roczneKoszta.at(min)->koszty << endl;
//        cout << ", jednorazowy koszt podpisania umowy: " << _roczneKoszta.at(min)->dostawca->getKosztPodpisania() << endl;
//
//    }
//
//    void dodajUrzadzenie(Urzadzenie* urza)
//    {
//        _urzadzenia.push_back(urza);
//    }
//
//    std::vector<RoczneKoszta *> sprawdz_taryfy(Dostawca* d)
//    {
//        //RoczneKoszta koszta =
//        double min = 999999;
//        int od_h = _sen_do;
//        int do_h = _sen_od;
//        double wynik = 0;
//        //int zmiennazmetodypodajstawkeodk;
//        //wyszukanie najtanszej taryfy podanego dostawcy
//        std::vector<RoczneKoszta> wyniki;
//        //d = d;
//
//
//        //for (Dostawca& d : d._taryfy) //przerobic na it w wektorze
//        // For each dla taryf
//        for (Taryfa& taryfa : d->wszystkie_taryfy())
//        {
//            //koszt każdego urządzenia w zadanej taryfie
//            Taryfa* t = &taryfa; // No i reszta kodu powinna śmigać
//            double koszt_urzadzen = 0;
//            double koszt_urzadzenia_dzienny = 0;
//            for (Urzadzenie* u : _urzadzenia)
//            {
//                wynik = 0;
//                for (int l = 0; l < 1; l++)
//                {
//
//                    if (_sen_do + u->_ile_pracuje <= _praca_od)
//                    {
//                        for (int k = _sen_do; k != _sen_do + u->_ile_pracuje; k++)
//                        {
//                            wynik = wynik + t->_pobierz_stawke(k)*u->_pobrana_moc;//*u._pobrana_moc;
//                        }
//
//                        min = wynik;
//                        wynik = 0;
//                        od_h = _sen_do;
//                        do_h = _sen_do + u->_ile_pracuje;
//                    }
//                }
//
//                for (int l = _sen_do; l < _praca_od; l++)
//                {
//                    if (l + u->_ile_pracuje <= _praca_od)
//                    {
//                        for (int k = l; k < (l + u->_ile_pracuje); k++)
//                            wynik = wynik + t->_pobierz_stawke(k)*u->_pobrana_moc;
//
//                        if (wynik < min)
//                        {
//                            min = wynik;
//                            od_h = _sen_do + 1;
//                            do_h = od_h + u->_ile_pracuje;
//                        }
//
//                    }
//                }
//                for (int l = _praca_do; l < _sen_od; l++)
//                {
//
//                    if (l + u->_ile_pracuje <= _sen_od)
//                    {
//                        for (int k = l; k < (l + u->_ile_pracuje); k++)
//                        {
//                            wynik = wynik + t->_pobierz_stawke(l)*u->_pobrana_moc;
//                        }
//
//                        if (wynik < min)
//                        {
//                            min = wynik;
//                            od_h = _sen_do;
//                            do_h = od_h + u->_ile_pracuje;
//                        }
//                    }
//                }
//                u->ustaw_godziny(od_h, do_h);
//                u->_koszt_dzienny = min;
//            }
//            koszt_urzadzen = 0;
//            for (Urzadzenie* u : _urzadzenia)
//            {
//                int koszt_jednego_urzadzenia_w_roku = 0;
//                if (taryfa.typTaryfy() == 2)//tutaj taryfa weekendowa
//                {
//                    koszt_jednego_urzadzenia_w_roku += u->_koszt_dzienny * 5;
//                    koszt_jednego_urzadzenia_w_roku += u->_ile_pracuje*taryfa._pobierz_stawke(6) * 2;
//                    koszt_urzadzen += 52 * koszt_jednego_urzadzenia_w_roku;
//                }
//                else
//                {
//                    koszt_urzadzen += u->_koszt_dzienny*u->_dni_w_roku;
//                }
//            }
//            //_roczneKoszta.push_back(min);
//            RoczneKoszta * test = new RoczneKoszta(d, &taryfa, koszt_urzadzen);
//            _roczneKoszta.push_back(test);
//        }
//        return _roczneKoszta;
//
//    }
//
//
//};
//
//int _tmain(int argc, char* argv[])
//{
//    vector<Dostawca*> dostawcy;
//    Dostawca* dostawca = new Dostawca(string("TPSA"));
//
//    Dostawca* enea = new Dostawca(string("Enea S.A."));
//    enea->setKosztPodpisania(0.0);
//    enea->dodaj_taryfe(Taryfa::Typ::JednoStrefowa, 0.2931, 0.2931);
//    enea->dodaj_taryfe(Taryfa::Typ::DwuStrefowa, 0.3667, 0.1696);
//    enea->dodaj_taryfe(Taryfa::Typ::Weekendowa, 0.4209, 0.1765);
//    dostawcy.push_back(enea);
//
//    Urzadzenie* grzejnik = new Urzadzenie();
//    grzejnik->_nazwa = "grzejnik";
//    grzejnik->_pobrana_moc = 2.50;
//    grzejnik->_ile_pracuje = 6;
//    grzejnik->_dni_w_roku = 150;
//    /*Urzadzenie* zmywarka = new Urzadzenie();
//    zmywarka->_nazwa = "zmywarka";
//    zmywarka->_pobrana_moc = 2250;
//    zmywarka->_ile_pracuje = 6;*/
//    Urzadzenie* telewizor = new Urzadzenie();
//    telewizor->_nazwa = "telewizor";
//    telewizor->_pobrana_moc = 0.205;
//    telewizor->_ile_pracuje = 4;
//    telewizor->_dni_w_roku = 365;
//    /*Urzadzenie* pralka = new Urzadzenie();
//    pralka->_nazwa = "pralka";
//    pralka->_pobrana_moc = 224.475;
//    pralka->_ile_pracuje = 4;*/
//    /*Urzadzenie* lodowka = new Urzadzenie();
//    lodowka->_nazwa = "lodowka";
//    lodowka->_pobrana_moc = 224.475;
//    lodowka->_ile_pracuje = 4;
//    */
//    /*Urzadzenie* odkurzacz = new Urzadzenie();
//    odkurzacz->_nazwa = "odkurzacz";
//    odkurzacz->_pobrana_moc = 224.475;
//    odkurzacz->_ile_pracuje = 4;*/
//
//    //dostawca->dodaj_taryfe(new Taryfa( Taryfa::Typ::JednoStrefowa, 0.2931, 0.2931 ));
//
//    dostawcy.push_back(dostawca);
//    //Uzytkownik janusz(string("janusz"), 6, 16, 1, 5 );
//    //janusz->setImie("janusz");
//    //Uzytkownik* janusz = new Uzytkownik(imie, 6, 16, 1, 5);
//
//    std::string imie = "Janusz";
//    Uzytkownik* janusz = new Uzytkownik();
//    janusz->setUzytkownik("Janusz", 6, 16, 23, 5);
//    janusz->dodajUrzadzenie(grzejnik);
//    janusz->dodajUrzadzenie(telewizor);
//    Uzytkownik* grazyna = new Uzytkownik();
//    grazyna->setUzytkownik("Grazyna", 14, 23, 24, 5);
//    grazyna->dodajUrzadzenie(grzejnik);
//    grazyna->dodajUrzadzenie(telewizor);
//
//    std::vector<RoczneKoszta> wyniki;
//    for (int i = 0; i < dostawcy.size(); i++)
//    {
//        auto wynik = janusz->sprawdz_taryfy(dostawcy.at(i));
//        auto wynik2 = grazyna->sprawdz_taryfy(dostawcy.at(i));
//    }
//
//    janusz->pokazWynik();
//
//    //wybierz najlepszy wynik z nich
//    delete grzejnik;
//    delete telewizor;
//    //delete zmywarka;
//    //delete pralka;
//    //delete lodowka;
//    //delete odkurzacz;
//    delete dostawca;
//    delete janusz;
//    return 0;
//}
