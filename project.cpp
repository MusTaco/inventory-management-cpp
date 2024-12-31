#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

// to make it look pretty
#include <iomanip>
// used to overwrite data.txt with temp.txt
#include <cstdio>
using namespace std;

void exit_program();
bool check_exists(string product, bool check_stock = false, int required_qty = 0);
void input_items();
void update_stocks();
void current_stock();
void generate_invoice();
void search_client_record();

int main() {

    char option;

    cout << "\033[2J\033[H";
    
    cout << "--------------------" << endl;
    cout << "INVENTORY MANAGEMENT" << endl;
    cout << "--------------------" << endl; 
    cout << endl << endl;

    cout << "1. Add new items\n" << "2. Update stocks\n" << "3. View stock\n" << "4. Generate Invoice\n" << "5. Search client's record\n" << "6. Exit\n";
    cout << ">> ";
    cin >> option;
    cin.ignore();

    switch (option) {
        case '1':
            input_items();
            break;
        case '2':
            update_stocks();
            break;
        case '3':
            current_stock();
            break;
        case '4':
            generate_invoice();
            break;
        case '5':
            search_client_record();
            break;
        case '6':
            return 0;
            break;
        default:
            cout << "Incorrect option!\n";
    }
    
    exit_program();
 
    return 0;
}

void exit_program() {
    char exit;
    cout << "Enter 1 to exit(any key to continue): ";
    cin >> exit;
    switch (exit) {
        case '1':
            break;
        default:
            main();
    }
}


bool check_exists(string product, bool check_stock, int required_qty) {
    string line;
    string remaining_stock;
    ifstream dataFile("text_files/data.txt");
    while (getline(dataFile, line)) {
        if (line == product) {
            if (check_stock) {
                getline(dataFile, remaining_stock);
                if (stoi(remaining_stock) < required_qty) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

void input_items() {
    int numOfItems;
    cout << "Enter number of items: ";
    cin >> numOfItems;
    cin.ignore();

    // Open file in append
    ofstream dataFile("text_files/data.txt", ios_base::app);

    int itemStock;
    string itemName;

    for (int i = 0; i < numOfItems; i++) {
        cout << "Enter name of item: ";
        getline(cin, itemName);
        cout << itemName << endl;
        dataFile << itemName << '\n';

        if (check_exists(itemName)) {
            cout << "Cannot add item since it already exists!\n";
            continue;
        }

        cout << "Enter stock for " << itemName << ": ";
        cin >> itemStock;
        cout << itemStock << endl;
        cin.ignore();
        dataFile << itemStock << '\n';
    }
    dataFile.close();
}

void update_stocks() {

    string itemName, itemStock, option;
    bool is_found = false;

    ofstream writeTempFile("text_files/temp.txt");
    ifstream readDataFile("text_files/data.txt", ios_base::app);

    cout << "\n--- Update Stocks ---" << endl;

    current_stock();

    cout << "Enter product name(type all to edit all): ";
    getline(cin, option);

    while (getline(readDataFile, itemName)) {
        int purchased = 0, sold = 0, updatedAmount = 0;
        writeTempFile << itemName << endl;
        if (itemName == option || option == "all") {
            is_found = true;
            cout << "For   " << itemName << ":" << endl;
            cout << "Enter quantity purchased: ";
            cin >> purchased;
            cout << "Enter quantity sold: ";
            cin >> sold;
            cin.ignore();
        }

        getline(readDataFile, itemStock);

        int prevAmount = stoi(itemStock);
        updatedAmount = prevAmount + purchased - sold;

        writeTempFile << updatedAmount << endl;
    }

    writeTempFile.close();
    readDataFile.close();

    remove("text_files/data.txt");
    rename("text_files/temp.txt", "text_files/data.txt");
    if (!is_found) {
        cout << "(" << option << ")No such product exists in the current inventory.\n";
    }
}


void current_stock() {

    string itemName;
    string itemStock;
    ifstream dataFile("text_files/data.txt");

    cout << "\n--- Current Stock ---" << endl;
    cout << setw(20) << left << "Item" << " | " << setw(20) << left << "Stock" << endl; 
    cout << "----------------------------------------\n";
    while (getline(dataFile, itemName))
    {
        getline(dataFile, itemStock);
        cout << setw(20) << left << itemName << " | " << setw(20) << left << itemStock << "\n"; 
    }
    
}

int get_client_id() {
    int client_id = 1;
    string line;
    ifstream readId("text_files/client_id.txt");
    while (getline(readId, line)) {
        client_id++;
    }
    readId.close();

    ofstream writeId("text_files/client_id.txt", ios_base::app);
    writeId << to_string(client_id) << '\n';
    writeId.close();

    return client_id;
}

void store_client_record(string telephone, string date, int invoice_id, string name, int total_items, int sub_total) {
    ofstream clientRecord("text_files/client_record.txt", ios_base::app);
    clientRecord << telephone << "\n" << date << "\n" << to_string(invoice_id) << "\n" << name << "\n" << to_string(total_items) << "\n" << to_string(sub_total) << "\n";
    clientRecord.close();
}

void generate_invoice() {
    string name, telephone, date;
    int numOfitems, client_id, total, actuallyPurchased = 0;
    client_id = get_client_id();
    string invoice_name = "clients/client_id_" + to_string(client_id) + ".csv";

    int subTotal = 0;

    name = "very very big dummy name";
    telephone = "111111111111";

    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    date = to_string(localTime->tm_year + 1900) + "-"   // Year
              + to_string(localTime->tm_mon + 1) + "-"       // Month (0-based, so +1)
              + to_string(localTime->tm_mday);                // Day

    ofstream client_invoice;
    client_invoice.open(invoice_name, ios::out);

    client_invoice << "\n\n,,----------------------------------" << endl;
    client_invoice << ",,-----11111's Inventory-----" << endl;
    client_invoice << ",,Islamabad | 03235235677" << endl;;
    client_invoice << ",,----------------------------------\n\n" << endl;

    client_invoice << "Date: " << date << "," << " Invoice_id: " << client_id << endl;

    while (name.length() > 20) {
        cout << "Enter your name(20 characters max): ";
        getline(cin, name);
    }

    while (telephone.length() != 11) {
        cout << "Enter contact number(11 digits): ";
        cin >> telephone;
    }

    client_invoice << "Name: " << name << "," << " Telephone: " << telephone << endl;

    cout << "Enter number of items to purchase: ";
    cin >> numOfitems;
    cin.ignore();

    client_invoice << "Item Name" << "," << "Qty" << "," << "Unit price" << "," << "Total" << endl;

    for (int i = 0; i<numOfitems; i++) {
        string itemName;
        int qty, unit_price;

        cout << "Item: ";
        getline(cin, itemName);

        cout << "Quantity: ";
        cin >> qty;

        if (!check_exists(itemName, true, qty)) {
            cout << "Cannot add item since it is out of stock!\n";
            cin.ignore();
            continue;
        }

        cout << "Enter unit price: ";
        cin >> unit_price;


        int totalPrice = unit_price * qty;
        subTotal += totalPrice;
        cin.ignore();
        client_invoice  << itemName << "," << qty << "," << unit_price << "," << totalPrice << "\n";
        actuallyPurchased += 1;
    }
    client_invoice << "," << "," << "Sub total: " << "," << subTotal << endl;
    client_invoice << "Thank you for purchasing!" << endl << endl;

    store_client_record(telephone, date, client_id, name, numOfitems, subTotal);


}

void search_client_record() {
    string telephone, date, client_id, name, numOfitems, subTotal, line;
    bool is_found = false;
    cout << "Enter contact number: ";
    cin >> telephone;
    

    ifstream clientRecord("text_files/client_record.txt");
    cout << telephone << endl;
    
    while(getline(clientRecord, line)) {
        getline(clientRecord, date);
        getline(clientRecord, client_id);
        getline(clientRecord, name);
        getline(clientRecord, numOfitems);
        getline(clientRecord, subTotal);

        if (line == telephone) {
            if (!is_found) {
                cout << left << setw(20) << "Invoice ID " << setw(20) << "Date " << setw(20) << "Contact No. " << setw(20) << "Name " << setw(20) << "Total Items " << setw(20) << "Total " << "\n";
                is_found = true;
            }
            

            cout << left << setw(20) << client_id << setw(20) << date << setw(20) << telephone << setw(20) << name << setw(20) << numOfitems << setw(20) << subTotal << "\n";
        }
    }
    if (!is_found) {
        cout << "No records found for " << telephone << endl;
    }

}