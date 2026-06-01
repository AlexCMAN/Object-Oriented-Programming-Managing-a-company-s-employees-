#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

class DataSalariala {
private:
    char luna[20];
    double salariu;
    int zileConcediuRamase;
public:
    DataSalariala(const char* l = "", double s = 0.0, int z = 0) : salariu(s), zileConcediuRamase(z) {
        strncpy(luna, l, 19); luna[19] = '\0';
    }
    const char* getLuna() const { return luna; }
    double getSalariu() const { return salariu; }
    int getZileConcediuRamase() const { return zileConcediuRamase; }
    void afisare() const {
        cout << "Luna: " << luna << ", Salariu: " << salariu
             << " RON, Zile concediu: " << zileConcediuRamase << endl;
    }
};

class Angajat {
private:
    char nume[50], prenume[50], adresa[100], departament[50], functie[50];
    int varsta;
    static int totalAngajati;
public:
    Angajat(const char* n = "", const char* p = "", int v = 0,
            const char* a = "", const char* d = "", const char* f = "") : varsta(v) {
        strncpy(nume, n, 49); nume[49] = '\0';
        strncpy(prenume, p, 49); prenume[49] = '\0';
        strncpy(adresa, a, 99); adresa[99] = '\0';
        strncpy(departament, d, 49); departament[49] = '\0';
        strncpy(functie, f, 49); functie[49] = '\0';
        totalAngajati++;
    }
    virtual ~Angajat() { totalAngajati--; }
    const char* getNume() const { return nume; }
    const char* getDepartament() const { return departament; }
    virtual void afisare() const {
        cout << "Nume: " << nume << " " << prenume << ", Varsta: " << varsta
             << ", Departament: " << departament << ", Functie: " << functie << endl;
    }
    virtual double getMedieSalariu() const { return 0.0; }
    virtual int getZileConcediuCurente() const { return 0; }
    virtual void salvareInFisier(ofstream& fout) const {
        fout << nume << endl << prenume << endl << varsta << endl
             << adresa << endl << departament << endl << functie << endl;
    }
    virtual void incarcareDinFisier(ifstream& fin) {
        fin.getline(nume, 50);
        fin.getline(prenume, 50);
        fin >> varsta; fin.ignore();
        fin.getline(adresa, 100);
        fin.getline(departament, 50);
        fin.getline(functie, 50);
    }
};
int Angajat::totalAngajati = 0;

class AngajatComplet : public Angajat {
private:
    DataSalariala* istoricSalarii;
    int numarSalarii, capacitateSalarii;
public:
    AngajatComplet(const char* n = "", const char* p = "", int v = 0,
                   const char* a = "", const char* d = "", const char* f = "")
        : Angajat(n, p, v, a, d, f), istoricSalarii(0), numarSalarii(0), capacitateSalarii(0) {}
    ~AngajatComplet() { delete[] istoricSalarii; }
    void adaugaDateSalariale(const DataSalariala& data) {
        if (numarSalarii >= capacitateSalarii) {
            int newCap = (capacitateSalarii == 0) ? 2 : capacitateSalarii * 2;
            DataSalariala* nou = new DataSalariala[newCap];
            for (int i = 0; i < numarSalarii; i++) nou[i] = istoricSalarii[i];
            delete[] istoricSalarii;
            istoricSalarii = nou;
            capacitateSalarii = newCap;
        }
        istoricSalarii[numarSalarii++] = data;
    }
    void afisare() const {
        Angajat::afisare();
        cout << "   Medie salariu: " << getMedieSalariu()
             << " RON, Zile de concediu ramase: " << getZileConcediuCurente() << endl;
    }
    double getMedieSalariu() const {
        if (numarSalarii == 0) return 0.0;
        double suma = 0.0;
        for (int i = 0; i < numarSalarii; i++) suma += istoricSalarii[i].getSalariu();
        return suma / numarSalarii;
    }
    int getZileConcediuCurente() const {
        return (numarSalarii == 0) ? 0 : istoricSalarii[numarSalarii - 1].getZileConcediuRamase();
    }
    void salvareInFisier(ofstream& fout) const {
        Angajat::salvareInFisier(fout);
        fout << numarSalarii << endl;
        for (int i = 0; i < numarSalarii; i++) {
            fout << istoricSalarii[i].getLuna() << endl
                 << istoricSalarii[i].getSalariu() << endl
                 << istoricSalarii[i].getZileConcediuRamase() << endl;
        }
    }
    void incarcareDinFisier(ifstream& fin) {
        Angajat::incarcareDinFisier(fin);
        fin >> numarSalarii; fin.ignore();
        delete[] istoricSalarii;
        capacitateSalarii = numarSalarii;
        istoricSalarii = new DataSalariala[capacitateSalarii];
        for (int i = 0; i < numarSalarii; i++) {
            char luna[20]; double sal; int zile;
            fin.getline(luna, 20);
            fin >> sal >> zile; fin.ignore();
            istoricSalarii[i] = DataSalariala(luna, sal, zile);
        }
    }
};

template<typename T>
class ManagerColectie {
private:
    T** elemente;
    int numarElemente, capacitate;
public:
    ManagerColectie() : elemente(nullptr), numarElemente(0), capacitate(0) {}
    ~ManagerColectie() {
        for (int i = 0; i < numarElemente; i++) delete elemente[i];
        delete[] elemente;
    }
    void adaugaElement(T* elem) {
        if (numarElemente >= capacitate) {
            int newCap = (capacitate == 0) ? 2 : capacitate * 2;
            T** nou = new T*[newCap];
            for (int i = 0; i < numarElemente; i++) nou[i] = elemente[i];
            delete[] elemente;
            elemente = nou;
            capacitate = newCap;
        }
        elemente[numarElemente++] = elem;
    }
    void stergeElement(int index) {
        if (index >= 0 && index < numarElemente) {
            delete elemente[index];
            for (int i = index; i < numarElemente - 1; i++) elemente[i] = elemente[i + 1];
            numarElemente--;
        }
    }
    void stergeElementDupaNume(const char* nume) {
        for (int i = 0; i < numarElemente; i++) {
            if (strcmp(elemente[i]->getNume(), nume) == 0) {
                stergeElement(i);
                break;
            }
        }
    }
    T* getElement(int index) const { return (index >= 0 && index < numarElemente) ? elemente[index] : 0; }
    T* cautaDupaNume(const char* nume) const {
        for (int i = 0; i < numarElemente; i++)
            if (strcmp(elemente[i]->getNume(), nume) == 0) return elemente[i];
        return 0;
    }
    int getNumarElemente() const { return numarElemente; }
    void afisareToate() const {
        for (int i = 0; i < numarElemente; i++) {
            cout << i + 1 << ". ";
            elemente[i]->afisare();
        }
    }
    void sortareDupaNume() {
        for (int i = 0; i < numarElemente - 1; i++)
            for (int j = i + 1; j < numarElemente; j++)
                if (strcmp(elemente[i]->getNume(), elemente[j]->getNume()) > 0)
                    swap(elemente[i], elemente[j]);
    }
    void sortareDupaMedieSalariu() {
        for (int i = 0; i < numarElemente - 1; i++)
            for (int j = i + 1; j < numarElemente; j++)
                if (elemente[i]->getMedieSalariu() < elemente[j]->getMedieSalariu())
                    swap(elemente[i], elemente[j]);
    }
    void afisarePeDepartamentCuConcediu(const char* departament) const {
        cout << "\nAngajati din departamentul " << departament << " cu zile concediu" << endl;
        int found = 0;
        for (int i = 0; i < numarElemente; i++) {
            if (strcmp(elemente[i]->getDepartament(), departament) == 0 &&
                elemente[i]->getZileConcediuCurente() > 0) {
                cout << ++found << ". ";
                elemente[i]->afisare();
            }
        }
        if (found == 0) cout << "Nu exista angajati in acest departament cu zile de concediu ramase." << endl;
    }
    void salvareInFisier(const char* numeFisier) {
        ofstream fout(numeFisier);
        fout << numarElemente << endl;
        for (int i = 0; i < numarElemente; i++) elemente[i]->salvareInFisier(fout);
        fout.close();
    }
    void incarcareDinFisier(const char* numeFisier) {
        ifstream fin(numeFisier);
        if (!fin) return;
        int nr; fin >> nr; fin.ignore();
        for (int i = 0; i < nr; i++) {
            AngajatComplet* ang = new AngajatComplet();
            ang->incarcareDinFisier(fin);
            adaugaElement(ang);
        }
        fin.close();
    }
};

void afisareMeniu() {
    cout << "\nMENIU GESTIUNE ANGAJATI\n"
         << "1. Adaugare angajat nou\n"
         << "2. Stergere angajat\n"
         << "3. Adaugare salariu si zile concediu\n"
         << "4. Afisare angajati ordonati\n"
         << "5. Afisare angajati cu zile concediu pe departament\n"
         << "6. Salvare in fisier\n"
         << "7. Iesire\n"
         << "Alegeti optiunea: ";
}

void afisareSubmeniuSortare() {
    cout << "\nSORTARE ANGAJATI\na. Afisare ordonata dupa nume\nb. Afisare ordonata dupa medie salariu\nAlegeti optiunea: ";
}

int main() {
    ManagerColectie<AngajatComplet> manager;
    manager.incarcareDinFisier("angajati.txt");

    int optiune;
    do {
        afisareMeniu();
        cin >> optiune;
        cin.ignore();
        switch (optiune) {
            case 1: {
                char nume[50], prenume[50], adresa[100], departament[50], functie[50];
                int varsta;
                cout << "Nume: "; cin.getline(nume, 50);
                cout << "Prenume: "; cin.getline(prenume, 50);
                cout << "Varsta: "; cin >> varsta; cin.ignore();
                cout << "Adresa: "; cin.getline(adresa, 100);
                cout << "Departament: "; cin.getline(departament, 50);
                cout << "Functie: "; cin.getline(functie, 50);
                manager.adaugaElement(new AngajatComplet(nume, prenume, varsta, adresa, departament, functie));
                cout << "Angajat adaugat.\n";
                break;
            }
            case 2: {
                char nume[50];
                cout << "Numele angajatului de sters: ";
                cin.getline(nume, 50);
                manager.stergeElementDupaNume(nume);
                break;
            }
            case 3: {
                char nume[50], luna[20];
                double salariu; int zile;
                cout << "Nume angajat: "; cin.getline(nume, 50);
                AngajatComplet* ang = manager.cautaDupaNume(nume);
                if (ang) {
                    cout << "Luna: "; cin.getline(luna, 20);
                    cout << "Salariu: "; cin >> salariu;
                    cout << "Zile concediu ramase: "; cin >> zile; cin.ignore();
                    ang->adaugaDateSalariale(DataSalariala(luna, salariu, zile));
                    cout << "Date adaugate.\n";
                } else cout << "Angajat negasit.\n";
                break;
            }
            case 4: {
                char sub;
                afisareSubmeniuSortare(); cin >> sub; cin.ignore();
                if (sub == 'a') { manager.sortareDupaNume(); manager.afisareToate(); }
                else if (sub == 'b') { manager.sortareDupaMedieSalariu(); manager.afisareToate(); }
                else cout << "Optiune invalida.\n";
                break;
            }
            case 5: {
                char dep[50];
                cout << "Departament: "; cin.getline(dep, 50);
                manager.afisarePeDepartamentCuConcediu(dep);
                break;
            }
            case 6: {
                manager.salvareInFisier("angajati.txt");
                cout << "Date salvate in angajati.txt\n";
                break;
            }
            case 7: cout << "La revedere!\n"; break;
            default: cout << "Optiune invalida.\n";
        }
    } while (optiune != 7);
    return 0;
}
