#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <set>

using namespace std;

class CartellaGenerator {
private:
    static const int RIGHE = 3;
    static const int COLONNE = 9;
    static const int NUMERI_PER_RIGA = 5;
    static const int TOTALE_NUMERI = 15;
    vector<vector<int>> cartella;
    vector<vector<bool>> numeriBarrati;  // Nuova matrice per tenere traccia dei numeri barrati

    int NumeroCasuale(int min, int max) {
        return min + rand() % (max - min + 1);
    }

    vector<int> distribuisciNumeriPerColonna() {
        vector<int> numeriPerColonna(COLONNE, 0);
        int numeriRimanenti = TOTALE_NUMERI;
        
        while (numeriRimanenti > 0) {
            for (int c = 0; c < COLONNE && numeriRimanenti > 0; c++) {
                if (numeriPerColonna[c] < 2) {
                    numeriPerColonna[c]++;
                    numeriRimanenti--;
                }
            }
        }
        
        for (int i = 0; i < COLONNE - 1; i++) {
            int j = i + rand() % (COLONNE - i);
            swap(numeriPerColonna[i], numeriPerColonna[j]);
        }
        
        return numeriPerColonna;
    }

    vector<int> generaNumeriColonna(int colonna, int quantita) {
        set<int> numeri;
        int min = (colonna == 0) ? 1 : colonna * 10;
        int max = (colonna == 8) ? 90 : (colonna + 1) * 10 - 1;
        
        while (numeri.size() < quantita) {
            numeri.insert(NumeroCasuale(min, max));
        }
        
        vector<int> risultato(numeri.begin(), numeri.end());
        sort(risultato.begin(), risultato.end());
        return risultato;
    }

    bool rigaValida(int riga) {
        int count = 0;
        for (int c = 0; c < COLONNE; c++) {
            if (cartella[riga][c] != 0) count++;
        }
        return count == NUMERI_PER_RIGA;
    }

public:
    CartellaGenerator() : cartella(RIGHE, vector<int>(COLONNE, 0)),
                         numeriBarrati(RIGHE, vector<bool>(COLONNE, false)) {
        srand(time(NULL));
    }

    void genera() {
        do {
            for (auto& riga : cartella) {
                fill(riga.begin(), riga.end(), 0);
            }

            vector<int> numeriPerColonna = distribuisciNumeriPerColonna();
            
            for (int c = 0; c < COLONNE; c++) {
                if (numeriPerColonna[c] > 0) {
                    vector<int> numeriColonna = generaNumeriColonna(c, numeriPerColonna[c]);
                    vector<int> posizioniDisponibili = {0, 1, 2};
                    
                    for (int num : numeriColonna) {
                        int posIndex = rand() % posizioniDisponibili.size();
                        int riga = posizioniDisponibili[posIndex];
                        posizioniDisponibili.erase(posizioniDisponibili.begin() + posIndex);
                        cartella[riga][c] = num;
                    }
                }
            }

            vector<int> numeriPerRiga(RIGHE, 0);
            for (int r = 0; r < RIGHE; r++) {
                numeriPerRiga[r] = count_if(cartella[r].begin(), cartella[r].end(), 
                                          [](int n) { return n != 0; });
            }

            bool modificato;
            do {
                modificato = false;
                for (int r = 0; r < RIGHE; r++) {
                    while (numeriPerRiga[r] > NUMERI_PER_RIGA) {
                        for (int c = 0; c < COLONNE && numeriPerRiga[r] > NUMERI_PER_RIGA; c++) {
                            if (cartella[r][c] != 0) {
                                for (int r2 = 0; r2 < RIGHE; r2++) {
                                    if (r2 != r && numeriPerRiga[r2] < NUMERI_PER_RIGA && cartella[r2][c] == 0) {
                                        cartella[r2][c] = cartella[r][c];
                                        cartella[r][c] = 0;
                                        numeriPerRiga[r]--;
                                        numeriPerRiga[r2]++;
                                        modificato = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            } while (modificato);

        } while (!rigaValida(0) || !rigaValida(1) || !rigaValida(2));

        // Resetta la matrice dei numeri barrati
        for (auto& riga : numeriBarrati) {
            fill(riga.begin(), riga.end(), false);
        }
    }

    void stampa() const {
        for (int r = 0; r < RIGHE; r++) {
            for (int c = 0; c < COLONNE; c++) {
                if (cartella[r][c] == 0) {
                    cout << setw(5) << "X";
                } else if (numeriBarrati[r][c]) {
                    cout << setw(4) << cartella[r][c] << "*";  // Aggiungi un asterisco per i numeri barrati
                } else {
                    cout << setw(5) << cartella[r][c];
                }
            }
            cout << endl;
        }
    }

    // Nuova funzione per gestire i numeri estratti
    bool barraNumero(int numero) {
        bool trovato = false;
        for (int r = 0; r < RIGHE; r++) {
            for (int c = 0; c < COLONNE; c++) {
                if (cartella[r][c] == numero) {
                    numeriBarrati[r][c] = true;
                    trovato = true;
                }
            }
        }
        return trovato;
    }
};

int main() {
    CartellaGenerator gen;
    gen.genera();
    gen.stampa();

    int numero;
    cout << "\nInserisci un numero (0 per uscire): ";
    
    while (cin >> numero && numero != 0) {
        if (numero < 1 || numero > 90) {
            cout << "Numero non valido. Inserisci un numero tra 1 e 90." << endl;
        } else {
            if (gen.barraNumero(numero)) {
                cout << "Numero trovato e barrato!" << endl;
            } else {
                cout << "Numero non presente nella cartella." << endl;
            }
            gen.stampa();
        }
        cout << "\nInserisci un numero (0 per uscire): ";
    }

    return 0;
}