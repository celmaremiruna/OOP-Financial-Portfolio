#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

class Data {
private:
	int zi;
	int luna;
	int an;

	bool esteBisect(int an) {
		return (an % 4 == 0 and an % 100 != 0) or (an % 400 == 0);
	}

	int zileInLuna(int luna, int an) {
		if (luna == 2) {
			if (esteBisect(an))
				return 29;
			else
				return 28;
		}
		if (luna == 4 or luna == 6 or luna == 9 or luna == 11)
			return 30;
		return 31;
	}

public:
	Data() {
		zi = 1;
		luna = 1;
		an = 2026;
	}

	Data(int zi, int luna, int an) {
		if (an < 1900 or luna < 1 or luna>12 or zi<1 or zi>zileInLuna(luna, an)) {
			throw("data introdusa este invalida!");
		}
		else {
			this->zi = zi;
			this->luna = luna;
			this->an = an;
		}
	}

	int getZi() {
		return zi;
	}

	int getLuna() {
		return luna;
	}

	int getAn() {
		return an;
	}

	bool operator<(const Data& data) {
		if (an != data.an) return an < data.an;
		if (luna != data.luna) return luna < data.luna;
		return zi < data.zi;
	}

	static double diferentaInAni(const Data& inceput, const Data& sfarsit) {
		long zileInceput = (long)inceput.an * 365 + inceput.luna * 30 + inceput.zi;
		long zileSfarsit = (long)sfarsit.an * 365 + sfarsit.luna * 30 + sfarsit.zi;

		return (double)(zileSfarsit - zileInceput) / 365.0;
	}

	friend ostream& operator<<(ostream& out, const Data& d) {
		out << d.zi << "." << d.luna << d.an;
		return out;
	}
};

class InstrumentFinanciar {
protected:
	string nume;
	string simbol;
	double pretAchizitie;
	double cantitate;
	Data dataAchizitie;

public: 
	virtual double calculeazaValoareActuala(Data dataCurenta) = 0;
	virtual void afiseazaDetalii(ostream& os) = 0;

	InstrumentFinanciar(string nume, string simbol, double pretAchizitie, double cantitate, Data dataAchizitie) {
		this->nume = nume;
		this->simbol = simbol;
		this->pretAchizitie = pretAchizitie;
		this->cantitate = cantitate;
		this->dataAchizitie = dataAchizitie;
	}

	string getSimbol() {
		return this->simbol;
	}

	virtual ~InstrumentFinanciar(){}

	friend ostream& operator<<(ostream& out, InstrumentFinanciar& inf);
};
ostream& operator<<(ostream& out, InstrumentFinanciar& inf) {
	out << "\nNume: " << inf.nume;
	out << "\nSimbol: " << inf.simbol;
	out << "\nPret Achizitie: " << inf.pretAchizitie;
	out << "\nCantitate: " << inf.cantitate;
	out << "\nData Achizitie: " << inf.dataAchizitie;
	inf.afiseazaDetalii(out);
	return out;
}

class Stock : public InstrumentFinanciar {
protected:
	double pretActual;
	double dividendPerActiune;
	string sector;

public:
	Stock(string nume, string simbol, double pretAchizitie, double cantitate, Data dataAchizitie, double dividendPerActiune, string sector, double pretActual):
	InstrumentFinanciar(nume, simbol, pretAchizitie, cantitate, dataAchizitie){
		this->dividendPerActiune = dividendPerActiune;
		this->sector = sector;
		this->pretActual = pretActual;
	}

	double calculeazaValoareActuala(Data dataCurenta) override{
		double valoareActiva = cantitate * pretActual;
		double totalDividende = cantitate * dividendPerActiune;
		return valoareActiva + totalDividende;
	}

	void afiseazaDetalii(ostream& out) override{
		out << "\nDividend: " << this->dividendPerActiune;
		out << "\nSector: " << this->sector;
	}
};

class Bond :public InstrumentFinanciar {
protected:
	double dobandaFixa;
	Data dataMaturitate;
public:
	Bond(string nume, string simbol, double pretAchizitie, double cantitate, Data dataAchizitie, double dobandaFixa, Data dataMaturitate) :
	InstrumentFinanciar(nume, simbol, pretAchizitie, cantitate, dataAchizitie) {
		this->dobandaFixa = dobandaFixa;
		this->dataMaturitate = dataMaturitate;
	}

	void afiseazaDetalii(ostream& out) override{
		out << "\nDobanda Fixa: " << this->dobandaFixa;
		out << "\nData Maturitate: " << this->dataMaturitate;
	}

	double calculeazaValoareActuala(Data dataCurenta) override{
		Data dataLimita;
		if (dataCurenta < dataMaturitate)
			dataLimita = dataCurenta;
		else
			dataLimita = dataMaturitate;
		double ani = Data::diferentaInAni(dataAchizitie, dataLimita);
		return (cantitate * pretAchizitie) * (1 + dobandaFixa * ani);
	}
};

class DepozitBancar:public InstrumentFinanciar {
protected:
	double dobandaAnuala;
public:
	DepozitBancar(string nume, string simbol, double pretAchizitie, double cantitate, Data dataAchizitie, double dobandaAnuala) :
		InstrumentFinanciar(nume, simbol, pretAchizitie, cantitate, dataAchizitie) {
		this->dobandaAnuala = dobandaAnuala;
	}

	void afiseazaDetalii(ostream& out) override{
		out << "\nDobanda anuala: " << this->dobandaAnuala;
	}

	double calculeazaValoareActuala(Data dataCurenta) override{
		double rataLunara = dobandaAnuala / 12;
		double ani = Data::diferentaInAni(dataAchizitie, dataCurenta);
		double luniTrecute;
		if (ani > 0)
			luniTrecute = ani * 12;
		else
			luniTrecute = 0;
		double valoareFinala = (cantitate * pretAchizitie) * pow((1 + rataLunara), luniTrecute);
		return valoareFinala;
	}
};

class Portofoliu {
protected:
	vector<InstrumentFinanciar*> active{};
public:
	Portofoliu() {}

	Portofoliu(vector<InstrumentFinanciar*> active) {
		for (const auto& element : active) {
			this->active.push_back(element);
		}
	}

	~Portofoliu() {
		for (auto& element : active) {
			delete element;
		}
		active.clear();
	}

	void adaugaActiv(InstrumentFinanciar* activ) {
		this->active.push_back(activ);
	}

	void vindeActiv(string simbolCautat) {
		for (auto it = active.begin();it != active.end(); ) {
			if ((*it)->getSimbol() == simbolCautat) {
				cout <<(**it);
				delete* it;
				it = active.erase(it);
			}
			else {
				++it;
			}
		}
	}

	double calculeazaValoareTotala(Data dataCurenta) {
		double valTot=0;
		for (const auto& element : active) {
			valTot += element->calculeazaValoareActuala(dataCurenta);
		}
		return valTot;
	}

	void afiseazaPortofoliu() {
		for (const auto& element : active) {
			cout << *element;
			cout << "\n";
		}
	}

	template <typename tipActiv>
	double calculeazaTotalCategorie(Data dataCurenta) {
		double total = 0;
		for (auto activ : active) {
			tipActiv* casted = dynamic_cast<tipActiv*>(activ);
			if (casted != nullptr) {
				total += casted->calculeazaValoareActuala(dataCurenta);
			}
		}
		return total;
	}
};


int main() {
	Portofoliu portofoliu;

	Data dataAzi(22, 3, 2026);
	portofoliu.adaugaActiv(new Stock("Apple Inc.", "AAPL", 150.0, 10, Data(10, 1, 2025), 2.5, "IT", 175.0));
	portofoliu.adaugaActiv(new DepozitBancar("Depozit ING", "ING_DEP", 1000.0, 1, Data(15, 6, 2024), 0.07));
	portofoliu.adaugaActiv(new Bond("Titluri de Stat", "RO_BOND", 100.0, 50, Data(1, 1, 2023), 0.05, Data(1, 1, 2028)));

	int optiune = -1;
	while (optiune != 0) {
		cout << "\n\n  SIMULATOR PORTOFOLIU BURSAR   ";
		cout << "\n\n1. Afiseaza portofoliul detaliat";
		cout << "\n2. Calculeaza valoarea totala la zi";
		cout << "\n3. Vinde un instrument(dupa simbol)";
		cout << "\n4. Adauga un depozit bancar nou ";
		cout << "\n5. Adauga o obligatiune noua ";
		cout << "\n6. Adauga o actiune noua ";
		cout << "\n7. Calculeaza valoarea totala pe categorie ";
		cout << "\n0. Iesire din aplicatie";
		cout << "\n\n\nAlege o optiune: ";
		cin >> optiune;

		switch (optiune) {
		case 1: {
			cout << "\n---Activele tale---\n";
			portofoliu.afiseazaPortofoliu();
			break;
		}

		case 2: {
			cout << "\n---Valoarea Actuala---";
			cout<<portofoliu.calculeazaValoareTotala(dataAzi);
			break;
		}

		case 3: {
			string simbolSters;
			cout << "\nIntroduceti simbolul activului pe care vrei sa il vinzi: ";
			cin >> simbolSters;
			portofoliu.vindeActiv(simbolSters);
			cout << "\nActivul a fost sters cu succes!\n";
			break;
		}
		case 4: {
			string nume, simbol;
			double suma, dobanda;
			cout << "\nIntrodu numele bancii: ";
			cin >> nume;
			cout << "\nIntrodu simbolul: ";
			cin >> simbol;
			cout << "\nIntrodu suma depusa: ";
			cin >> suma;
			cout << "\nIntrodu dobanda anuala(ex: 0.05 pentru 5%): ";
			cin >> dobanda;

			portofoliu.adaugaActiv(new DepozitBancar(nume, simbol, suma, 1, dataAzi, dobanda));
			cout << "\nDepozit adaugat cu succes!\n";
			break;
		}
		case 5: {
			string nume, simbol;
			double dobanda, pret;
			int zi, luna, an;
			cout << "\nIntrodu numele obligatiunii: ";
			cin >> nume;
			cout << "\nIntrodu simbolul: ";
			cin >> simbol;
			cout << "\nIntrodu suma platita: ";
			cin >> pret;
			cout << "\nIntrodu dobanda anuala: ";
			cin >> dobanda;
			cout << "\nIntroduceti data la care va ajunge la maturitate dupa cum urmeaza: ";
			cout << "\nIntroduceti ziua: ";
			cin >> zi;
			cout << "\nIntroduceti luna: ";
			cin >> luna;
			cout << "\nIntroduceti anul: ";
			cin >> an;
			Data dataM(zi, luna, an);
			portofoliu.adaugaActiv(new Bond(nume, simbol, pret,1,dataAzi,dobanda,dataM));
			cout << "\nObligatiune adaugata cu succes!\n ";
			break;
		}
		case 6: {
			string nume, simbol, sector;
			double pret, dividende;
			cout << "\nIntrodu numele firmei: ";
			cin >> nume;
			cout << "\nIntrodu simbolul firmei: ";
			cin >> simbol;
			cout << "\nIntrodu sectorul de activitate al firmei: ";
			cin >> sector;
			cout << "\nIntrodu pretul de cumparare: ";
			cin >> pret;
			cout << "\nIntrodu numarul de dividende per actiune: ";
			cin >> dividende;
			portofoliu.adaugaActiv(new Stock(nume, simbol, pret, 1, dataAzi, dividende, sector, pret));
			cout << "\nActiune adaugata cu succes!\n";
			break;
		}
		case 7: {
			int alegere;
			cout << "\n   Calculeaza Total pe Categorie   ";
			cout << "\n1. Actiuni (Stocks)";
			cout << "\n2. Obligatiuni (Bonds)";
			cout << "\n3. Depozite Bancare";
			cout << "\nAlege categoria: ";
			cin >> alegere;

			double totalCategorie = 0;

			if (alegere == 1) {
				totalCategorie = portofoliu.calculeazaTotalCategorie<Stock>(dataAzi);
				cout << "\nValoarea totala a actiunilor este: " << totalCategorie << " RON\n";
			}
			else if (alegere == 2) {
				totalCategorie = portofoliu.calculeazaTotalCategorie<Bond>(dataAzi);
				cout << "\nValoarea totala a obligatiunilor este: " << totalCategorie << " RON\n";
			}
			else if (alegere == 3) {
				totalCategorie = portofoliu.calculeazaTotalCategorie<DepozitBancar>(dataAzi);
				cout << "\nValoarea totala a depozitelor este: " << totalCategorie << " RON\n";
			}
			else {
				cout << "\nCategorie invalida!\n";
			}
			break;
		}
		case 0: {
			cout << "\nIesire din simulator.";
			break;
		}
		default: {
			cout << "\nOptiune invalida, te rog sa incerci din nou.\n";
			break;
		}
		}
	}
	return 0;
}

