#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm> 

/**
 * \brief Produktrepräsentation
 *
 * Klasse, die ein Produkt mit Name, Ablaufdatum und Menge repräsentiert.
 * 
 * @author [Autorname]
 */
class Product {
public:
    std::string name;               // Name des Produkts
    std::string expirydate;         // Ablaufdatum des Produkts
    int quantity;                   // Menge des Produkts

    /**
     * \brief Parameterisierter Konstruktor
     *
     * Erzeugt ein Produkt mit gegebenem Namen, Ablaufdatum und Menge.
     *
     * @param n Name des Produkts
     * @param exp Ablaufdatum des Produkts
     * @param qty Menge des Produkts
     */
    Product(std::string n, std::string exp, int qty) : name(n), expirydate(exp), quantity(qty) {}

    /**
     * \brief Standardkonstruktor
     *
     * Erzeugt ein Produkt mit leeren Werten.
     */
    Product() : name(""), expirydate(""), quantity(0) {}

    /**
     * \brief Liest Produktinformationen aus einem Datenstrom
     *
     * @param stream Datenstrom, aus dem die Informationen gelesen werden
     */
    void read(std::istream& stream) {
        std::getline(stream, name);
        std::getline(stream, expirydate);
        stream >> quantity;
    }

    /**
     * \brief Statische Funktion zum Lesen von Produktinformationen aus einem Datenstrom
     *
     * @param stream Datenstrom, aus dem die Informationen gelesen werden
     * @return Gelesene Produktinformationen
     */
    static Product readStatic(std::istream& stream) {
        Product data;
        std::getline(stream >> std::ws, data.name, ','); // Lese bis ',' und beachte führende Leerzeichen
        std::getline(stream >> std::ws, data.expirydate, ','); // Lese bis ',' und beachte führende Leerzeichen
        stream >> data.quantity;
        return data;
    }

    /**
     * \brief Schreibt Produktinformationen in einen Datenstrom
     *
     * @param out Datenstrom, in den die Informationen geschrieben werden
     */
    void write(std::ostream& out) const {
        out << name << ',' << expirydate << ',' << quantity << '\n';
    }
};

/**
 * \brief Vorratsverwalter
 *
 * Klasse, die eine Vorratskammer mit Produkten verwaltet.
 * 
 * @author [Autorname]
 */
class PantryManager {
public:
    std::vector<Product> pantry;    // Container für Vorratsprodukte
};

/**
 * \brief Menüfunktionen und Hauptprogramm
 *
 * Funktionen zur Anzeige des Menüs, Hinzufügen, Bearbeiten, Löschen, Speichern und Laden von Produkten.
 * 
 * @author [Autorname]
 */
void printMenu() {
    std::cout << "\n"
              << "\n"
              << "Menü: \n"
              << "1. Alle Produkte anzeigen\n"
              << "2. Neues Produkt hinzufügen\n"
              << "3. Produkt bearbeiten\n"
              << "4. Produkt entfernen\n"
              << "5. Alle Daten speichern\n"
              << "6. Produkte mit Ablaufdatum oder bereits abgelaufen speichern\n"
              << "7. Produkte mit Menge von 5 oder weniger speichern\n"
              << "8. Daten laden\n"
              << "9. Daten von Ablaufdatum oder bereits abgelaufenen Produkten laden\n"
              << "10. Daten von Produkten mit geringer Menge laden\n"
              << "11. Beenden\n"
              << "\n"
              << "\n";
}

/**
 * \brief Anzeige der Produkte in der Vorratskammer
 *
 * Zeigt alle Produkte in der Vorratskammer mit ihren Details an.
 *
 * @param pantry Vorratskammer mit Produkten
 */
void displayProducts(const std::vector<Product>& pantry) {
    std::cout << "Produkte in der Vorratskammer: \n";
    for (size_t i = 0; i < pantry.size(); ++i) {
        std::cout << i + 1 << ". " << "Name: " << pantry[i].name
                  << ", Ablaufdatum: " << pantry[i].expirydate
                  << ", Menge: " << pantry[i].quantity << std::endl;
    }
}

/**
 * \brief Hinzufügen eines neuen Produkt-Eintrags
 *
 * Fragt den Benutzer nach den Informationen für ein neues Produkt und fügt es zur Vorratskammer hinzu.
 *
 * @param pantry Vorratskammer, zu der das Produkt hinzugefügt wird
 */
void addProduct(std::vector<Product>& pantry) {
    std::string name, expirydate;
    int quantity;

    std::cout << "Neuer Produkteintrag \n";
    
    // Produktname abfragen
    std::cout << "Geben Sie das Produkt ein (oder 'cancel' zum Abbrechen): \n";
    std::cin >> name;

    // Auf Unterbrechung prüfen
    if (name == "cancel") {
        std::cout << "Prozess wurde abgebrochen.\n";
        return;
    }

    // Ablaufdatum abfragen
    std::cout << "Geben Sie das Ablaufdatum ein (oder 'cancel' zum Abbrechen): \n";
    std::cin >> expirydate;

    // Auf Unterbrechung prüfen
    if (expirydate == "cancel") {
        std::cout << "Prozess wurde abgebrochen.\n";
        return;
    }

    // Menge abfragen
    std::cout << "Geben Sie die Menge ein (oder 'cancel' zum Abbrechen): \n";
    std::cin >> quantity;

    // Auf Unterbrechung prüfen
    if (std::cin.fail() || quantity < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ungültige Eingabe für Menge. Prozess wurde abgebrochen.\n";
        return;
    }

    std::cout << "Neues Produkt hinzugefügt \n";
    pantry.push_back(Product(name, expirydate, quantity));
}

/**
 * \brief Bearbeiten eines Produkt-Eintrags (nur Menge ändern)
 *
 * Fragt den Benutzer nach der Nummer des zu bearbeitenden Produkts und ändert die Menge.
 *
 * @param pantry Vorratskammer, in der das Produkt bearbeitet wird
 */
void editProduct(std::vector<Product>& pantry) {
    if (pantry.empty()) {
        std::cout << "Die Vorratskammer ist leer. Keine Produkte zum Bearbeiten. \n";
        return;
    }

    std::cout << "Produkte in der Vorratskammer: \n";
    for (size_t i = 0; i < pantry.size(); ++i) {
        std::cout << i + 1 << ". " << "Name: " << pantry[i].name << ", Ablaufdatum: "
                  << pantry[i].expirydate << ", Menge: " << pantry[i].quantity << std::endl;
    }

    int indexToEdit;
    std::cout << "Geben Sie die Nummer des zu bearbeitenden Produkts ein: \n";
    std::cin >> indexToEdit;

    if (indexToEdit >= 1 && static_cast<size_t>(indexToEdit) <= pantry.size()) {
        // Aktuelle Produktinformationen anzeigen
        std::cout << "Aktuelle Menge für Produkt " << indexToEdit << ":\n";
        std::cout << "Menge: " << pantry[indexToEdit - 1].quantity << "\n";

        // Neue Menge vom Benutzer abfragen
        std::cout << "Neue Menge eingeben:\n";
        std::cin >> pantry[indexToEdit - 1].quantity;

        std::cout << "Menge erfolgreich bearbeitet.\n";
    } else {
        std::cout << "Ungültige Auswahl. Produkt wurde nicht bearbeitet.\n";
    }
}

/**
 * \brief Entfernen von Produkt-Einträgen
 *
 * Fragt den Benutzer nach den Nummern der zu entfernenden Produkte und entfernt sie aus der Vorratskammer.
 *
 * @param pantry Vorratskammer, aus der Produkte entfernt werden
 */
void removeProduct(std::vector<Product>& pantry) {
    if (pantry.empty()) {
        std::cout << "Die Vorratskammer ist leer. Keine Produkte zum Entfernen. \n";
        return;
    }

    std::cout << "Produkte in der Vorratskammer: \n";
    for (size_t i = 0; i < pantry.size(); ++i) {
        std::cout << i + 1 << ". " << "Name: " << pantry[i].name << ", Ablaufdatum: "
                  << pantry[i].expirydate << ", Menge: " << pantry[i].quantity << std::endl;
    }

    std::cout << "Geben Sie die Nummern der zu entfernenden Produkte ein (getrennt durch Komma und Leerzeichen), dann Enter drücken: \n";
    std::string indicesToRemove;
    std::getline(std::cin >> std::ws, indicesToRemove);  // Ganze Zeile mit Leerzeichen einlesen

    std::istringstream iss(indicesToRemove);
    int indexToRemove;

    while (iss >> indexToRemove) {
        if (indexToRemove >= 1 && static_cast<size_t>(indexToRemove) <= pantry.size()) {
            pantry.erase(pantry.begin() + indexToRemove - 1);
        } else {
            std::cout << "Ungültige Auswahl: " << indexToRemove << ". Bitte erneut versuchen.\n";
        }
    }

    std::cout << "Produkte erfolgreich entfernt.\n";
}

/**
 * \brief Speichern von Daten in eine Datei
 *
 * Speichert die Produktinformationen in eine Textdatei.
 *
 * @param pantry Vorratskammer mit Produkten
 * @param filename Dateiname, in den die Daten gespeichert werden
 */
void saveData(const std::vector<Product>& pantry, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& product : pantry) {
            product.write(file);
        }
        std::cout << "Daten erfolgreich gespeichert. \n";
    } else {
        std::cerr << "Fehler beim Öffnen der Datei. \n";
    }
}

/**
 * \brief Speichern von Produkten mit Ablaufdatum oder bereits abgelaufen in eine Datei
 *
 * Speichert die Produktinformationen von Produkten mit Ablaufdatum oder bereits abgelaufen in eine Textdatei.
 *
 * @param pantry Vorratskammer mit Produkten
 * @param filename Dateiname, in den die Daten gespeichert werden
 */
void saveExpiringAndExpiredProducts(const std::vector<Product>& pantry, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        auto now = std::chrono::system_clock::now();

        for (const auto& product : pantry) {
            // Überprüfen, ob das Produkt ein gültiges Ablaufdatum hat und nicht "keins" ist
            if (!product.expirydate.empty() && product.expirydate != "keines") {
                // Ablaufdatum in einen Zeitpunkt umwandeln
                std::tm expirydate_tm = {};
                std::istringstream expirydate_stream(product.expirydate);
                expirydate_stream >> std::get_time(&expirydate_tm, "%d.%m.%Y");

                auto expirydate_time = std::chrono::system_clock::from_time_t(std::mktime(&expirydate_tm));

                // Überprüfen, ob das Produkt abgelaufen ist oder innerhalb der nächsten 7 Tage abläuft
                if (expirydate_time <= now + std::chrono::hours(7 * 24)) {
                    file << product.name << ',' << product.expirydate << ',' << product.quantity << '\n';
                }
            }
        }
        std::cout << "Produkte mit Ablaufdatum in den nächsten 7 Tagen oder bereits abgelaufen erfolgreich gespeichert.\n";
    } else {
        std::cerr << "Fehler beim Öffnen der Datei.\n";
    }
}

/**
 * \brief Speichern von Produkten mit geringer Menge in eine Datei
 *
 * Speichert die Produktinformationen von Produkten mit einer Menge von 5 oder weniger in eine Textdatei.
 *
 * @param pantry Vorratskammer mit Produkten
 * @param filename Dateiname, in den die Daten gespeichert werden
 */
void saveLowQuantityProducts(const std::vector<Product>& pantry, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        bool hasLowQuantityProducts = false;

        for (const auto& product : pantry) {
            // Überprüfen, ob die Menge kleiner oder gleich 5 ist
            if (product.quantity <= 5) {
                file << product.name << ',' << product.expirydate << ',' << product.quantity << '\n';
                hasLowQuantityProducts = true;
            }
        }

        if (hasLowQuantityProducts) {
            file << "Warnung: Alles nur 5 oder weniger\n";
        }

        std::cout << "Produkte mit Menge von 5 oder weniger erfolgreich gespeichert.\n";
    } else {
        std::cerr << "Fehler beim Öffnen der Datei.\n";
    }
}

/**
 * \brief Laden von Daten aus einer Datei
 *
 * Lädt Produktinformationen aus einer Textdatei.
 *
 * @param pantry Vorratskammer, in die die Daten geladen werden
 * @param filename Dateiname, aus dem die Daten geladen werden
 */
void loadData(std::vector<Product>& pantry, const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        pantry.clear();
        std::string line;
        while (std::getline(file, line)) {
            // Überprüfen, ob die Zeile mit dem Warnzeichen beginnt
            if (line.find("Warnung:") != 0) {
                std::istringstream iss(line);
                pantry.push_back(Product::readStatic(iss));
            }
        }
        std::cout << "Daten erfolgreich geladen.\n";
    } else {
        std::cerr << "Fehler beim Öffnen der Datei.\n";
    }
}

/**
 * \brief Behandelt die Menüoptionen
 *
 * Menü zur Verwaltung der Vorratskammer mit verschiedenen Optionen.
 *
 * @param pantryManager Vorratsverwalter
 */
void handleMenu(PantryManager& pantryManager) {
    int choice;

    do {
        printMenu();
        std::cout << "Ihre Auswahl: ";
        std::cin >> choice;

        switch (choice) {

            case 1:
                displayProducts(pantryManager.pantry);
                break;
            case 2:
                addProduct(pantryManager.pantry);
                break;
            case 3:
                editProduct(pantryManager.pantry);
                break;
            case 4:
                removeProduct(pantryManager.pantry);
                break;
            case 5:
                saveData(pantryManager.pantry, "pantry_data.txt");
                break;
            case 6:
                saveExpiringAndExpiredProducts(pantryManager.pantry, "expiring_and_expired_products.txt");
                break;
            case 7:
                saveLowQuantityProducts(pantryManager.pantry, "low_quantity_products.txt");
                break;
            case 8:
                loadData(pantryManager.pantry, "pantry_data.txt");
                break;           
            case 9:
                loadData(pantryManager.pantry, "expiring_and_expired_products.txt");
                break;
            case 10:
                loadData(pantryManager.pantry, "low_quantity_products.txt");
                break;
            case 11:
                std::cout << "Programm wird beendet. \n";
                break;
            default:
                std::cout << "Ungültige Auswahl. Bitte erneut versuchen.\n";
        }

    } while (choice != 11);
}

/**
 * \brief Hauptfunktion des Programms
 *
 * Erstellt einen Vorratsverwalter und behandelt das Hauptmenü.
 *
 * @return Rückgabewert des Programms
 */
int main() {
    PantryManager pantryManager;
    handleMenu(pantryManager);

    return 0;
}
