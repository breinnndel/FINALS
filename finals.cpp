#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

// Helper function for safe integer input
int getValidatedInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a number.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard any extra input
            return value;
        }
    }
}

// Helper function for safe double input
double getValidatedDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a number.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard any extra input
            return value;
        }
    }
}

// Product class
class Product {
private:
    int id;
    string name;
    double price;
    int stock;

public:
    Product(int pid, string pname, double pprice, int pstock) {
        if (pprice < 0 || pstock < 0) {
            cout << "Invalid price or stock.\n";
            price = 0;
            stock = 0;
        } else {
            price = pprice;
            stock = pstock;
        }
        id = pid;
        name = pname;
    }

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    bool reduceStock(int qty) {
        if (qty > stock) {
            cout << "Insufficient stock.\n";
            return false;
        }
        stock -= qty;
        return true;
    }

    void increaseStock(int qty) {
        stock += qty;
    }

    void setPrice(double newPrice) {
        if (newPrice >= 0) price = newPrice;
    }

    void display() const {
        cout << setw(4) << id << " | " << setw(15) << name
             << " | P" << fixed << setprecision(2) << price
             << " | Stock: " << stock << "\n";
    }
};

// CartItem class
class CartItem {
private:
    Product* product;
    int quantity;

public:
    CartItem(Product* p, int qty) {
        product = p;
        quantity = 0;
        if (qty > 0 && product->reduceStock(qty)) {
            quantity = qty;
        }
    }

    int getProductId() const { return product->getId(); }
    double getSubtotal() const { return quantity * product->getPrice(); }

    bool updateQuantity(int newQty) {
        product->increaseStock(quantity);
        if (newQty <= 0 || !product->reduceStock(newQty)) {
            cout << "Invalid quantity.\n";
            return false;
        }
        quantity = newQty;
        return true;
    }

    void restoreStock() {
        product->increaseStock(quantity);
    }

    void display() const {
        cout << product->getName() << " | Qty: " << quantity
             << " | Subtotal: P" << fixed << setprecision(2) << getSubtotal() << "\n";
    }
};

// Payment Strategy
class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentStrategy() {}
};

class CreditCardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid P" << fixed << setprecision(2) << amount << " using Credit Card.\n";
    }
};

class gCashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid P" << fixed << setprecision(2) << amount << " using GCASH.\n";
    }
};

// Shopping Cart
class ShoppingCart {
private:
    vector<CartItem> items;

public:
    void addItem(Product* prod, int qty) {
        if (qty <= 0) {
            cout << "Quantity must be positive.\n";
            return;
        }

        for (int i = 0; i < (int)items.size(); i++) {
            if (items[i].getProductId() == prod->getId()) {
                if (items[i].updateQuantity(qty)) {
                    cout << "Updated quantity.\n";
                }
                return;
            }
        }

        CartItem item(prod, qty);
        items.push_back(item);
        cout << "Item added to cart.\n";
    }

    void removeItem(int prodId) {
        for (int i = 0; i < (int)items.size(); i++) {
            if (items[i].getProductId() == prodId) {
                items[i].restoreStock();
                items.erase(items.begin() + i);
                cout << "Item removed.\n";
                return;
            }
        }
        cout << "Item not found in cart.\n";
    }

    void viewCart() const {
        if (items.empty()) {
            cout << "Cart is empty.\n";
            return;
        }

        cout << "\n--- Cart Contents ---\n";
        for (const auto& item : items)
            item.display();
        cout << "Total: P" << fixed << setprecision(2) << getTotal() << "\n";
    }

    double getTotal() const {
        double total = 0;
        for (const auto& item : items)
            total += item.getSubtotal();
        return total;
    }

    void generateReceipt() const {
        cout << "\n--- Receipt ---\n";
        for (const auto& item : items)
            item.display();
        cout << "Total Paid: P" << fixed << setprecision(2) << getTotal() << "\nThank you!\n";
    }

    void clearCart() {
        items.clear();
    }
};

// User Management
vector<string> usernames = {"admin", "user", "seller"};
vector<string> passwords = {"1234", "pass", "sell"};
vector<string> roles = {"admin", "buyer", "seller"};

string login() {
    string u, p;
    cout << "Username: "; cin >> u;
    cout << "Password: "; cin >> p;

    for (int i = 0; i < usernames.size(); i++) {
        if (usernames[i] == u && passwords[i] == p) {
            cout << "--------------------------\nLogin successful.\n--------------------------\nWelcome, " << u << "!\n";
            return roles[i];
        }
    }
    cout << "Login failed.\n";
    return "";
}

bool deleteProduct(vector<Product>& products, int id) {
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->getId() == id) {
            products.erase(it);
            cout << "Product deleted successfully.\n";
            return true;
        }
    }
    cout << "Product not found.\n";
    return false;
}

bool deleteUser(vector<string>& usernames, vector<string>& passwords, vector<string>& roles, string username) {
    for (int i = 0; i < usernames.size(); i++) {
        if (usernames[i] == username && roles[i] != "admin") {
            usernames.erase(usernames.begin() + i);
            passwords.erase(passwords.begin() + i);
            roles.erase(roles.begin() + i);
            cout << "User deleted successfully.\n";
            return true;
        }
    }
    cout << "User not found or cannot delete admin.\n";
    return false;
}

// Function to generate next available product ID
int getNextProductId(const vector<Product>& products) {
    int maxId = 0;
    for (const auto& product : products) {
        if (product.getId() > maxId) {
            maxId = product.getId();
        }
    }
    return maxId + 1;
}

// Function to add a new product
void addProduct(vector<Product>& products) {
    cout << "\n--- Add New Product ---\n";
    
    string productName;
    cout << "Enter product name: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, productName);
    
    if (productName.empty()) {
        cout << "Product name cannot be empty.\n";
        return;
    }
    
    double price = getValidatedDouble("Enter product price: P");
    if (price < 0) {
        cout << "Price cannot be negative.\n";
        return;
    }
    
    int stock = getValidatedInt("Enter initial stock quantity: ");
    if (stock < 0) {
        cout << "Stock cannot be negative.\n";
        return;
    }
    
    int newId = getNextProductId(products);
    
    Product newProduct(newId, productName, price, stock);
    products.push_back(newProduct);
    
    cout << "Product added successfully!\n";
    cout << "Product ID: " << newId << "\n";
    cout << "Name: " << productName << "\n";
    cout << "Price: P" << fixed << setprecision(2) << price << "\n";
    cout << "Stock: " << stock << "\n";
}

int main() {
    vector<Product> products = {
        Product(1, "Budai Chocolate", 2500.00, 5),
        Product(2, "Mouse", 350.00, 10),
        Product(3, "Keyboard", 800.00, 6),
        Product(4, "Hopia", 56.00, 3),
        Product(5, "Tube Top", 135.00, 8),
        Product(6, "Mouse Pad", 40.00, 4),
        Product(7, "DJ Pad", 7550.00, 1),
        Product(8, "Headphones", 750.00, 3),
        Product(9, "DripNBites", 130.00, 0),
        Product(10, "Konu Mini Crunch", 89.00, 7)
    };

    while (true) {
        cout << "\n===== Welcome to Brein's Shopping System =====\n";
        int entryChoice = getValidatedInt("1. Login\n0. Exit\nChoice: ");

        if (entryChoice == 0) {
            cout << "Thank you for shopping!\n";
            break;
        } else if (entryChoice == 1) {
            string role = login();
            if (role == "") continue;

            if (role == "admin") {
                int choice;
                do {
                    choice = getValidatedInt("\n--- Admin Menu ---\n1. View Products\n2. View Users\n3. View Sellers\n4. View Buyers\n5. Delete User\n6. Delete Product\n7. Logout\nChoice: ");

                    if (choice == 1) {
                        for (const auto& p : products) p.display();
                    } else if (choice == 2) {
                        for (int i = 0; i < usernames.size(); i++)
                            cout << "Username: " << usernames[i] << " | Role: " << roles[i] << "\n";
                    } else if (choice == 3 || choice == 4) {
                        string targetRole = (choice == 3) ? "seller" : "buyer";
                        for (int i = 0; i < usernames.size(); i++) {
                            if (roles[i] == targetRole)
                                cout << "Username: " << usernames[i] << "\n";
                        }
                    } else if (choice == 5) {
                        string uname;
                        cout << "Enter username to delete: ";
                        cin >> uname;
                        deleteUser(usernames, passwords, roles, uname);
                    } else if (choice == 6) {
                        int pid = getValidatedInt("Enter product ID to delete: ");
                        deleteProduct(products, pid);
                    }
                } while (choice != 7);
            } else if (role == "buyer") {
                ShoppingCart cart;
                int choice;
                do {
                    choice = getValidatedInt("\n1. View Products\n2. Add to Cart\n3. Remove from Cart\n4. View Cart\n5. Checkout\n6. Logout\nChoice: ");

                    if (choice == 1) {
                        for (const auto& p : products) p.display();
                    } else if (choice == 2) {
                        int id = getValidatedInt("Enter Product ID: ");
                        int qty = getValidatedInt("Enter Quantity: ");
                        Product* prod = nullptr;
                        for (auto& p : products) if (p.getId() == id) prod = &p;
                        if (prod) cart.addItem(prod, qty);
                        else cout << "Product not found.\n";
                    } else if (choice == 3) {
                        int id = getValidatedInt("Enter Product ID to remove: ");
                        cart.removeItem(id);
                    } else if (choice == 4) {
                        cart.viewCart();
                    } else if (choice == 5) {
                        int payChoice = getValidatedInt("\nSelect Payment Method:\n1. Credit Card\n2. GCASH\nChoice: ");
                        PaymentStrategy* payment = nullptr;

                        if (payChoice == 1) payment = new CreditCardPayment();
                        else if (payChoice == 2) payment = new gCashPayment();
                        else {
                            cout << "Invalid option.\n";
                            continue;
                        }

                        double total = cart.getTotal();
                        if (total > 0) {
                            payment->pay(total);
                            cart.generateReceipt();
                            cart.clearCart();
                        } else {
                            cout << "Your cart is empty.\n";
                        }
                        delete payment;
                    }
                } while (choice != 6);

            } else if (role == "seller") {
                int choice;
                do {
                    choice = getValidatedInt("\n--- Seller Menu ---\n1. View Products\n2. Add Product\n3. Restock Product\n4. Delete Product\n5. Logout\nChoice: ");

                    if (choice == 1) {
                        for (const auto& p : products) p.display();
                    } else if (choice == 2) {
                        addProduct(products);
                    } else if (choice == 3) {
                        int id = getValidatedInt("Enter Product ID to restock: ");
                        int qty = getValidatedInt("Enter quantity to add: ");
                        bool found = false;
                        for (auto& p : products) {
                            if (p.getId() == id) {
                                p.increaseStock(qty);
                                cout << "Product restocked.\n";
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Product not found.\n";
                        }
                    } else if (choice == 4) {
                        int pid = getValidatedInt("Enter product ID to delete: ");
                        deleteProduct(products, pid);
                    }
                } while (choice != 5);
            }
        }
    }

    return 0;
}