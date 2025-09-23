#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

class MOTORRAD{      // Klasse Motorrad 
    protected:
        string Modell;
        bool verfügbar;
    public:
        MOTORRAD(){};
        MOTORRAD(string mo, bool v): Modell(mo), verfügbar(v){};
        void set_verfügbar(bool v){
            verfügbar = v;
        }
        
        string getModell(){
            return Modell;
        }

        void setModell(string m){
            Modell = m;
        }

        bool get_verfügbar(){
            return verfügbar;
        }
};


class KUNDE{   //Klasse Kunde mit get und set Methoden, um außerhalb der Klasse auf die privaten Attribute zuzugreifen.
    private:
        string name;
        string addresse;
        int geburtsjahr;
        string telefonnr;
    public:
        KUNDE(string n, string a, int g, string t):name(n), addresse(a), geburtsjahr(g), telefonnr(t){};
        KUNDE(){};
        string getName(){
            return name;
        }

        void setname(string n){
            name = n;
        }

        string getTel(){
            return telefonnr;
        }

        void setTel(string t){
            telefonnr = t;
        }

        int getGeb(){
            return geburtsjahr;
        }

        void setGeb(int g){
            geburtsjahr = g;
        }

        string getAdresse(){
            return addresse;
        } 

        void setAdd(string add){
            addresse = add;
        }
};


class RESERVIERUNG{    // Klasse Reservierung durch friend class "verbunden" mit Klasse Reservierungsliste
    friend class ReservierungsListe;
    private:
        KUNDE customer;
        MOTORRAD* motptr;
        RESERVIERUNG* Nachfolger; // Zeiger Vorgänger und Nachfolger für verkettete Liste
        RESERVIERUNG* Vorgänger;
    public:
        RESERVIERUNG(KUNDE c, MOTORRAD* m): customer(c), motptr(m), Nachfolger(0){}
        RESERVIERUNG(){};

        KUNDE get_customer(){
            return customer;
        }

        MOTORRAD* get_motocycle(){
            return motptr;
        }

        RESERVIERUNG* get_nxt(){
            return Nachfolger;
        }

        void set_nxt(RESERVIERUNG* n){
            Nachfolger = n;
        }

        RESERVIERUNG* get_prv(){
            return Vorgänger;
        }

        void set_prv(RESERVIERUNG* p){
            Vorgänger = p;
        }

        friend ostream& operator<<(ostream& os, RESERVIERUNG& res); // Operatorüberladungen für shift-Operatoren in Kombination mit Objekten der Klasse Reservierung
        friend istream& operator>>(istream& is, RESERVIERUNG& res);
};

        ostream& operator<<(ostream& os, RESERVIERUNG& res){
            os<<res.customer.getName()<<"\t"<<res.customer.getGeb()<<"\t"<<res.customer.getAdresse()<<"\t"<<res.customer.getTel()<<endl;
            return os;
        }

        istream& operator>>(istream& is, RESERVIERUNG& res){
            string name, adresse, tel;
            int geb;
            is>>name>>geb>>adresse>>tel;
            res.customer.setname(name);
            res.customer.setGeb(geb);
            res.customer.setAdd(adresse);
            res.customer.setTel(tel);
            return is;
        }

class ReservierungsListe{    // Klasse Reservierungsliste als verkettete Liste mit Zeiger kopf auf Anfang der Liste
    private:
        RESERVIERUNG* kopf;
    public:
        ReservierungsListe():kopf(0){};

        bool empty() const{
            return kopf == 0;
        }

        RESERVIERUNG* Anfang() const{
            return kopf;
        }


        void reservierung_vornehmen(MOTORRAD (&M)[4], ofstream& file){  // Funktion zum Vornehmen einer Reservierung
            string name, adresse, tel, bike;
            int geb;
            char mot_license;
            cout<<"Hat der Kunde einen Führerschein der Klasse A? [j = Ja/n = Nein]"<<endl;
            cin>>mot_license;
            if(mot_license != 'j'){
                throw (string)"Kein gültiger Führerschein.";  // Hat der Kunde kein Motorradführerschein wird ein Fehler geworfen und das Programm beendet.
            return;
            }
            else{
                cout<<"Name des Kunden:[Vorname,Nachname]"<<endl;
                cin>>name;
                cout<<"Adresse des Kunden:[Strasse.Hausnummer]"<<endl;
                cin>>adresse;
                cout<<"Telefonnr des Kunden:"<<endl;
                cin>>tel;
                cout<<"Geburtsjahr des Kunden:"<<endl;
                cin>>geb;
                cout<<"Welches Motorrad ist gewünscht:(SuzukiGSX-8S, HondaTransalp, BMWF650GS, DucatiStreetfighterV4)"<<endl;
                cin>>bike;
                for(int i = 0; i<4; i++){
                    if(M[i].getModell() == bike){       // Wenn es das gewünschte Modell im Fuhrpark gibt wird geprüft ob es verfügbar ist
                        if(M[i].get_verfügbar() == true){
                            KUNDE* kd = new KUNDE(name, adresse, geb, tel);   // Kunde wird dynamisch mit eingegeben Daten angelegt, um es dann in nächster Zeile als Parameter für dynamisches Anlegen einer Reservierung zu nutzen.
                            RESERVIERUNG* res = new RESERVIERUNG(*kd, &M[i]);
                            res->Nachfolger = kopf;  // Reservierung wird verketteter Liste hinzugefügt
                            if(kopf!= 0){
                                kopf->Vorgänger = res;
                            }
                            kopf = res;
                            M[i].set_verfügbar(false);  // Motorrad kann nun nicht mehr vermietet werden.
                            cout<<"Reservierung für Motorrad "<<M[i].getModell()<<" war erfolgreich."<<endl;
                            file.open("reservierungen.txt", ios::app);  // Reservierung wird in Datei geschrieben.
                            if(file.good()){
                                file<<*res;
                                file.close();
                            }
                            else{
                                cout<<"Datei konnte nicht geöffnet werden."<<endl;
                            }
                            break;
                        }
                        else{
                            cout<<"Motorrad bereits vermietet."<<endl;
                        }
                    }
                }
            }
        }

        void Motorrad_abholen(ofstream& o_file, ifstream& i_file){
            string n, a, tel;
            int g;
            tel = "deleted";
            cout<<"Name des Kunden: [Vorname,Nachname]"<<endl;
            cin>>n;
            cout<<"Adresse des Kunden"<<endl;
            cin>>a;
            cout<<"Geburtsjahr des Kunden"<<endl;
            cin>>g;
            KUNDE* kd = new KUNDE(n, a, g, tel); //dynamisches Anlegen des zu löschenden Kunden
            RESERVIERUNG* temp = kopf;  // Zeiger auf Anfang der Liste wird initialisiert
            while((temp != 0) && (temp->customer.getName() != kd->getName() || temp->customer.getGeb() != kd->getGeb())){  // verkettete Liste wird durchgegangen bis entweder am Ende oder Reservierung mit zu löschendem Kunden gefunden
                temp = temp->Nachfolger;
            }
            if(temp == 0){  // die nächsten Zeilen decken alle Fälle ab. Falls Reservierung mit dem zu löschenden Kunden nicht gefunden wird, Reservierung mit zu löschendem Kunden am Anfang der Liste etc.
                cout<<"Kunde nicht in Datenbank"<<endl;
                return;
            }
            if(temp->Vorgänger != 0){
                temp->Vorgänger->Nachfolger = temp->Nachfolger;
            }
            else{
                kopf = temp->Nachfolger;
            }
            if(temp->Nachfolger != 0){
                temp->Nachfolger->Vorgänger = temp->Vorgänger;
            }
            cout<<"Abholung erfolgreich."<<endl;
            i_file.open("reservierungen.txt", ios::in);  // Reservierung mit zu löschendem Kunden, also zu löschende Reservierung wird aus Datei gelöscht 
            RESERVIERUNG res;
            int i = 0;
            int counter = 0;
            RESERVIERUNG reservierung[100];  // Objekte Reservierung aus ausgelesener Datei werden übergangsweise in Array gespeichert
            if(i_file.good()){
                while(i_file>>res){
                    reservierung[i] = res;
                    counter++; // counter zählt die Länge des Arrays also der Objekte in Datei
                    i++;
                }
                i_file.close();
            }
            else{
                cout<<"Datei konnte nicht geöffnet werden."<<endl;
            }
            o_file.open("reservierungen.txt", ios::out);  // Datei wird überschrieben mit allen Elemente aus Array ausser zu löschender Reservierung
            if(o_file.good()){
                for(int x = 0; x<counter; x++){
                    if(reservierung[x].customer.getName() != kd->getName()){
                        o_file<<reservierung[x];
                    }
                }
                o_file.close();
            }
            else{
                cout<<"Datei konnte nicht geöffnet werden."<<endl;
            }
            delete temp;
            delete kd;  // Zeiger auf Kopf der Liste und Kunde als Hilfsobjekt werden wieder gelöscht damit Speicher freigegeben wird.
        }

        void print_Reservierungen(){
            RESERVIERUNG* temp = kopf;    // verkettete Liste wird durchgegangen und Objekte der Klasse Reservierung ausgegeben
            while(temp != 0){
                cout<<"Kunde: "<<temp->customer.getName()<<"\t"<<temp->customer.getGeb()<<"\t"<<temp->customer.getAdresse()<<"\t"<<temp->customer.getTel()<<"\t";
                cout<<"Motorrad: "<<temp->motptr->getModell()<<endl;
                temp = temp->Nachfolger;
            }
        }

        void laden(ifstream& file){   // Einladen der Datensätze aus Datei zu Programmstart damit bei Programmende verloren gegangene Daten in verketteter Liste wieder auf Stand von Datei ist
            file.open("reservierungen.txt", ios::in);
            RESERVIERUNG res;
            if(file.good()){
                while(file>>res){
                    RESERVIERUNG* r = new RESERVIERUNG(res);
                    if(r->customer.getTel() != "deleted"){              // Datei wird ausgelesen und Objekte der Klasse Reservierung in verkettete Liste geladen
                        r->Nachfolger = kopf;
                        if(kopf != 0){
                            kopf->Vorgänger = r;
                        }
                        kopf = r;
                    }
                }
                file.close();
        }
            else{
                cout<<"Datei konnte nicht geöffnet werden."<<endl;
            }
        }

        void Mot_Rückgabe(MOTORRAD (&M)[4]){    // Motorrad wird bei Rückgabe wieder vefügbar/mietbar gemacht durch setzen von Attribut verfügbar auf true.
        string modell;
        cout<<"Welches Motorrad wurde zurückgebracht?"<<endl;
        cin>>modell;
        for(int i = 0; i<4; i++){
            if(M[i].getModell() == modell){
                M[i].set_verfügbar(true);
                cout<<"Rückgabe von Motorrad"<<M[i].getModell()<<" erfolgreich."<<endl;
            }
        }
        }
};


int main(){
    MOTORRAD fuhrpark[4] = {MOTORRAD("SuzukiGSX-8S", true), MOTORRAD("HondaTransalp", true), MOTORRAD("BMWF650GS", true), MOTORRAD("DucatiStreetfighterV4", true)}; //Fuhrpark
    ReservierungsListe liste; // Objekt der Klasse ReservierungsListe als die verkettete Liste
    ofstream einlesen_res;  //Objekt der Klasse ofstream zum Schreiben
    einlesen_res.open("reservierungen.txt", ios::app);
    if(einlesen_res.good()){
        einlesen_res<<"";
        einlesen_res.close();
    }
    ifstream auslesen_res;   //Objekt der Klasse ifstream zum Lesen
    liste.laden(auslesen_res);  //Laden der Reservierungen aus Datei in verkettete Liste.
    int auswahl;
    try{       // Fehlerbehandlung
        do{    // Menü
            cout<<"Herzlich Willkommen im Hauptmenü -- Wählen Sie aus einer der folgenden Möglichkeiten:"<<endl;
            cout<<"[1] -- Reservierung vornehmen"<<endl;
            cout<<"[2] -- Reservierungen einsehen"<<endl;
            cout<<"[3] -- Motorradabholung"<<endl;
            cout<<"[4] -- Motorradrückgabe"<<endl;
            cout<<"[0] -- Beenden"<<endl;
            cin>>auswahl;
            switch(auswahl){
                case 1:{
                    liste.reservierung_vornehmen(fuhrpark, einlesen_res);
                    break;
                }
                case 2:{
                    liste.print_Reservierungen();
                    break;
                }
                case 3:{
                    liste.Motorrad_abholen(einlesen_res, auslesen_res);
                    break;
                }
                case 4:{
                    liste.Mot_Rückgabe(fuhrpark);
                    break;
                }
                case 0:{
                    cout<<"Auf Wiedersehen!"<<endl<<endl;
                    break;
                }
                default:{
                    cout<<"Falsche Eingabe!"<<endl<<endl;
                    break;
                }
            }
        }
        while(auswahl!=0);
        }
    catch(string s){
        cerr<<s;
    }
    return 0;
}