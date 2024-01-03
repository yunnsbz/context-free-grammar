#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

vector<pair<string,vector<string>>> convertExpressionToVector(string cfg_ifadesi) {
    vector<pair<string, vector<string>>> cfgVectors;
    pair<string,vector<string>> cfgPair;

    size_t arrowIndex = cfg_ifadesi.find("-->");

    //string::npos find fonksiyonu arad���n� bulamazsa d�nd�r�len bir sabittir. "-->" bulundu mu kontrol eder
    while (arrowIndex != string::npos) {

        //find fonksiyonu arrowIndex konumundan itibaren arama yapacak:
        size_t virgulIndex = cfg_ifadesi.find(",", arrowIndex);

        string nonTerminal = cfg_ifadesi.substr(arrowIndex - 1, arrowIndex); 

        //okun sa� taraf�n� al:
        string gramer = cfg_ifadesi.substr(arrowIndex + 3, virgulIndex - arrowIndex - 3); // �retim kurallar�
        
        // E�er virg�l bulunamazsa son k�sm� al:
        if (virgulIndex == string::npos) {
            gramer = cfg_ifadesi.substr(arrowIndex + 3);
        }

        //gramer'in ayra�lar ("|") ile ayr�lm�� hali
        vector<string> productionRules;
        
        size_t ayracIndex = gramer.find("|");

        while (ayracIndex != string::npos) {
            string kural = gramer.substr(0, ayracIndex); // Kurallar� ay�rma
            productionRules.push_back(kural);
            gramer = gramer.substr(ayracIndex + 1); // aktar�lan kural� ��kartma
            ayracIndex = gramer.find("|");
        }

        // Son kural� da ekle ("|" bulunamasa da i�eride bir kural daha var) (eklenen her kural gramer'den silinir)
        productionRules.push_back(gramer);

        //pair ekleme
        cfgPair = make_pair(nonTerminal, productionRules);

        // Vector'e ekleme
        cfgVectors.push_back(cfgPair);

        // Di�er non-terminal i�in indeks g�ncelleme (while d�ng�s�n�n par�as�)
        arrowIndex = cfg_ifadesi.find("-->", virgulIndex);

        
    }
    
    // Vector i�eri�ini yazd�rma
    for (const auto& pair : cfgVectors) {
        std::cout << pair.first << " -> ";
        for (const auto& rule : pair.second) {
            std::cout << rule << ", ";
        }
        std::cout << std::endl;
    }

    return cfgVectors;
}


// CFG �retim kurallar�n� depolamak i�in:
class ProductionRule {
public:
    char nonTerminal;
    vector<string> expansions;

    ProductionRule(char nt, vector<string> exps) : nonTerminal(nt), expansions(exps) {}
};


class ContextFreeGrammar {
public:
    vector<ProductionRule> kurallar;

    void kuralEkle(char nonTerminal, vector<string> expansions) {
        ProductionRule newRule(nonTerminal, expansions);
        kurallar.push_back(newRule);
    }
};

// Derinlik �ncelikli arama (DFS) ile dil a�ac�n� olu�turan fonksiyon
void generateLanguageTree(ContextFreeGrammar& cfg, string current, vector<string>& languageTree) {
    bool isTerminal = true;

    for (char& c : current) {
        if (isupper(c)) { // E�er karakter b�y�k harfse, nonTerminal
            isTerminal = false;
            for (auto& rule : cfg.kurallar) {
                if (rule.nonTerminal == c) {
                    for (auto& expansion : rule.expansions) {
                        string newString = current;
                        newString.replace(newString.find(c), 1, expansion);
                        generateLanguageTree(cfg, newString, languageTree);
                    }
                }
            }
        }
    }

    if (isTerminal) {
        languageTree.push_back(current);
    }
}

int main() {
    ContextFreeGrammar cfg;
    string cfg_ifade;
    //�rnek:  "S-->aa|bX|aXX, X-->ab|b"
    cout << "cfg ifadesini giriniz: ";
    getline(cin, cfg_ifade);

    auto cfgVectors = convertExpressionToVector(cfg_ifade);

    // CFG kurallar�n� tan�mlama
    for (auto& pair : cfgVectors)
    {
        cfg.kuralEkle(pair.first[0], pair.second);
    }
    
    //('X', { "ab", "b" });

    vector<string> languageTree;

    generateLanguageTree(cfg, "S", languageTree);

    set<string> uniqueWords;
    set<string> repetitiveWords;

    cout << "uretilen Kelimeler: ";
    for (auto& word : languageTree) {
        if (uniqueWords.insert(word).second) {
            cout << word << ", ";
        }
        else {
            repetitiveWords.insert(word);
        }
    }
    cout << endl;

    cout << "tekrarlanan kelimeler: ";
    for (auto& word : repetitiveWords) {
        cout << word << ", ";
    }
    cout << endl;

    return 0;
}

