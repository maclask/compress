//POP_2017-01-23_projekt_2_Laskowski_Maciej_EIT_3_165650
//Code::Blocks 16.01
#include <iostream>
#include<fstream>

using namespace std;

const int MAX_S = 4;
const int MAX_W = 4;
const int MAX_N=20;

char zamiana(char wartosc);
char zamiana_dekompresja(char wartosc);
void kompresja(char slownik[], char wejscie[]);
void dekompresja();
int main()
{
    char slownik[MAX_S];
    char wejscie[MAX_W];
    int wybor;
    cout<<"Aby dokonac kompresji wybierz 1, aby dokonac dekompresji wybierz 2"<<endl;
    cin>>wybor;
    if(wybor==1)
        kompresja(slownik, wejscie);
    else if(wybor==2)
        dekompresja();
    else
        cout<<"Nie wybrales destepnej opcji. Nastapi zakonczenie programu."<<endl;
    return 0;
}

void kompresja(char slownik[], char wejscie[])
{
    char plik_we[MAX_N], plik_wy[MAX_N];
    bool koniec=false;
    bool znaleziono=false;
    cout<<"Podaj nazwe pliku wejsciowego: "<<endl;
    cin >> plik_we;
    cout<<"Podaj nazwe pliku wyjsciowego: "<<endl;
    cin >> plik_wy;
    ofstream out(plik_wy, ios::binary);
    ifstream in(plik_we, ios::binary);
    if(in)
    {
        for(int i=0; i<MAX_W; i++)
        {
            wejscie[i] = in.get();
            wejscie[i] = zamiana(wejscie[i]);
        }
        for(int i=0; i<MAX_S; i++) slownik[i] = wejscie[0];

        while(!koniec)
        {
            if(wejscie[0]==100)koniec=true; //liczba 100 jako znak konca pliku
            int licznik=2;
            int licznik2=1;
            for(int i=0; i<MAX_S-1; i++)
            {
                if(slownik[i]==wejscie[0] && slownik[i+1]==wejscie[1] && wejscie[0]!=100 && wejscie[1]!=100)
                {
                    znaleziono=true;
                    for(int j=2; j+i<MAX_S; j++)
                    {
                            if(slownik[i+j]==wejscie[j] && wejscie[j]!=100)
                            {
                                licznik++;
                                licznik2 = MAX_S-1-i;
                            }
                    }
                    break;
                }
            }
            if(znaleziono)
            {
                for(int i=0; i<MAX_S-licznik; i++) slownik[i]=slownik[licznik+i];

                for(int i=0; i<licznik; i++) slownik[MAX_S-licznik+i] = wejscie[i];

                for(int i=0; i<MAX_W-licznik; i++) wejscie[i]=wejscie[licznik+i];
                for(int i=0; i<licznik; i++)
                {wejscie[MAX_W-licznik+i] = in.get();
                wejscie[MAX_W-licznik+i]=zamiana(wejscie[MAX_W-licznik+i]);
                if(in.eof())
                    wejscie[MAX_W-licznik+i]=100;
                }
                licznik +=72;
                out.write(reinterpret_cast <char *>(&licznik),sizeof(char));
                out.write(reinterpret_cast <char *>(&licznik2),sizeof(char));
            }
            else
            {
                if(wejscie[0]!=100)out.write(reinterpret_cast <char *>(&wejscie[0]),sizeof(char));
                for(int i=0; i<MAX_S-1; i++) slownik[i]=slownik[i+1];
                slownik[MAX_S-1]=wejscie[0];
                for(int i=0; i<MAX_W-1; i++) wejscie[i]=wejscie[i+1];
                wejscie[MAX_W-1]=in.get();
                wejscie[MAX_W-1]=zamiana(wejscie[MAX_W-1]);
                if(in.eof())
                        wejscie[MAX_W-1]=100;
            }
            znaleziono=false;
        }
        cout<<"Kompresja przebiegla pomyslnie";

    }
    else
        cout<<"Nie ma takiego pliku wejsciowego lub nie mozna go otworzyc";
    out.close();
    in.close();
}

void dekompresja()
{
    char plik_we[MAX_N], plik_wy[MAX_N];
    cout<<"Podaj nazwe pliku wejsciowego: "<<endl;
    cin >> plik_we;
    cout<<"Podaj nazwe pliku wyjsciowego: "<<endl;
    cin >> plik_wy;
    ofstream out2(plik_wy);//by utworzyc plik jezeli nie istnieje
    out2.close();
    fstream out(plik_wy, ios::binary | ios::in | ios::out); //plik do odczytu i zapisu
    ifstream in(plik_we, ios::binary);


    if(in)
    {
        in.seekg(0,ios::end);
        long poloz = in.tellg();
        in.seekg(0);
        char buf, buf2, wartosc;
        for(int i=0; i<poloz; i++)
        {
            buf = in.get();
            if(buf<74)
            {
                buf=zamiana_dekompresja(buf);
                out.write(reinterpret_cast <char*>(&buf),sizeof(char));
            }
            else
            {
                buf2 = in.get();
                char ilosc_kopiowanych = buf-72;
                char cofniecie = 0-buf2-1;
                long poloz2 = out.tellg();
                out.seekg(cofniecie,ios::cur);
                for(int i=0; i<ilosc_kopiowanych; i++)
                {
                    out.read(reinterpret_cast<char*>(&wartosc),sizeof(char));
                    out.seekg(poloz2+i);
                    out.write(reinterpret_cast<char*>(&wartosc),sizeof(char));
                    out.seekg(cofniecie,ios::cur);
                }
                out.seekg(poloz2+ilosc_kopiowanych);
                i++;
            }
        }
        cout<<"Dekompresja przebiegla pomyslnie";
    }
    else
        cout<<"Nie ma takiego pliku wejsciowego lub nie mozna go otworzyc";
    out.close();
    in.close();
}

char zamiana(char wartosc)
{
    char nowa_wartosc;

    if(wartosc>='a' && wartosc<='z')nowa_wartosc=wartosc-97;

    else if(wartosc>='A' && wartosc<='Z')nowa_wartosc=wartosc-39;

    else if(wartosc>'(' && wartosc<'9')nowa_wartosc=wartosc+12;

    else if(wartosc==32)nowa_wartosc=70; //spacja

    else if(wartosc==10)nowa_wartosc=71; //nowa linia

    else if(wartosc==61)nowa_wartosc=72; //=

    else nowa_wartosc=73;

    return nowa_wartosc;

}

char zamiana_dekompresja(char wartosc)
{
    char nowa_wartosc;

    if(wartosc>=0 && wartosc<=25)nowa_wartosc=wartosc+97; //a-z

    else if(wartosc>=26 && wartosc<=51)nowa_wartosc=wartosc+39; //A-Z

    else if(wartosc>52 && wartosc<69)nowa_wartosc=wartosc-12; //( - 9

    else if(wartosc==70)nowa_wartosc=32; //spacja

    else if(wartosc==71)nowa_wartosc=10; //nowa linia

    else if(wartosc==72)nowa_wartosc=61; //=

    else nowa_wartosc=35; //#

    return nowa_wartosc;

}
