/*
 * Sports Player Auction Management System (IPL-Style)
 * 
 * OOPS Concepts Implemented:
 * 1. Abstraction: Abstract base class 'Player' with pure virtual functions.
 * 2. Inheritance: Batsman, Bowler, AllRounder, WicketKeeper derived from Player.
 * 3. Encapsulation: Private/protected variables with getters and budget protection.
 * 4. Polymorphism: Virtual functions (overriding) and function overloading.
 * 5. Aggregation: Team class holds a list of Player pointers.
 */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Base Abstract Class
class Player {
protected:
    int id;
    string name;
    string role;
    double basePrice;
    double soldPrice;
    bool isSold;
    string teamName;

public:
    Player(int pId, string pName, string pRole, double bPrice) {
        id = pId;
        name = pName;
        role = pRole;
        basePrice = bPrice;
        soldPrice = 0.0;
        isSold = false;
        teamName = "None";
    }

    virtual ~Player() {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getRole() const { return role; }
    double getBasePrice() const { return basePrice; }
    double getSoldPrice() const { return soldPrice; }
    bool getIsSold() const { return isSold; }
    string getTeamName() const { return teamName; }

    void assignToTeam(string tName, double price) {
        teamName = tName;
        soldPrice = price;
        isSold = true;
    }

    // Pure virtual functions (Abstraction)
    virtual void displayDetails() const = 0;
    virtual double calculateRating() const = 0;
};

// Derived Class 1: Batsman
class Batsman : public Player {
private:
    int runs;
    double strikeRate;
    int centuries;

public:
    Batsman(int pId, string pName, double bPrice, int r, double sr, int cent)
        : Player(pId, pName, "Batsman", bPrice) {
        runs = r;
        strikeRate = sr;
        centuries = cent;
    }

    void displayDetails() const override {
        cout << id << "\t" << name << "\t" << role << "\tBase: Rs. " << basePrice 
             << " Cr\tRuns: " << runs << "\tSR: " << strikeRate << "\t100s: " << centuries;
    }

    double calculateRating() const override {
        return (runs / 50.0) + (strikeRate * 0.4) + (centuries * 5.0);
    }
};

// Derived Class 2: Bowler
class Bowler : public Player {
private:
    int wickets;
    double economy;

public:
    Bowler(int pId, string pName, double bPrice, int w, double eco)
        : Player(pId, pName, "Bowler", bPrice) {
        wickets = w;
        economy = eco;
    }

    void displayDetails() const override {
        cout << id << "\t" << name << "\t" << role << "\tBase: Rs. " << basePrice 
             << " Cr\tWickets: " << wickets << "\tEco: " << economy;
    }

    double calculateRating() const override {
        return (wickets * 1.5) + ((10.0 - economy) * 8.0);
    }
};

// Derived Class 3: All-Rounder
class AllRounder : public Player {
private:
    int runs;
    int wickets;
    double strikeRate;
    double economy;

public:
    AllRounder(int pId, string pName, double bPrice, int r, int w, double sr, double eco)
        : Player(pId, pName, "All-Rounder", bPrice) {
        runs = r;
        wickets = w;
        strikeRate = sr;
        economy = eco;
    }

    void displayDetails() const override {
        cout << id << "\t" << name << "\t" << role << "\tBase: Rs. " << basePrice 
             << " Cr\tRuns: " << runs << "\tWkts: " << wickets << "\tSR: " << strikeRate;
    }

    double calculateRating() const override {
        return (runs / 60.0) + (wickets * 1.2) + (strikeRate * 0.25) + ((10.0 - economy) * 4.0);
    }
};

// Derived Class 4: Wicket-Keeper
class WicketKeeper : public Player {
private:
    int runs;
    int dismissals;

public:
    WicketKeeper(int pId, string pName, double bPrice, int r, int dis)
        : Player(pId, pName, "Wicket-Keeper", bPrice) {
        runs = r;
        dismissals = dis;
    }

    void displayDetails() const override {
        cout << id << "\t" << name << "\t" << role << "\tBase: Rs. " << basePrice 
             << " Cr\tRuns: " << runs << "\tDismissals: " << dismissals;
    }

    double calculateRating() const override {
        return (runs / 50.0) + (dismissals * 2.0);
    }
};

// Team Class (Encapsulation and Aggregation)
class Team {
private:
    int id;
    string name;
    string mascot;
    double totalBudget;
    double remainingBudget;
    vector<Player*> squad;

public:
    Team(int tId, string tName, string tMascot, double budget = 100.0) {
        id = tId;
        name = tName;
        mascot = tMascot;
        totalBudget = budget;
        remainingBudget = budget;
    }

    int getId() const { return id; }
    string getName() const { return name; }
    string getMascot() const { return mascot; }
    double getRemainingBudget() const { return remainingBudget; }
    int getSquadSize() const { return squad.size(); }
    const vector<Player*>& getSquad() const { return squad; }

    bool canAfford(double amount) const {
        return remainingBudget >= amount;
    }

    bool deductBudget(double amount) {
        if (canAfford(amount)) {
            remainingBudget = remainingBudget - amount;
            return true;
        }
        return false;
    }

    void addPlayer(Player* p) {
        squad.push_back(p);
    }

    void displaySquad() const {
        cout << "\nTeam: " << name << " (" << mascot << ")\n";
        cout << "Remaining Budget: Rs. " << remainingBudget << " Cr / Rs. " << totalBudget << " Cr\n";
        cout << "Total Players: " << squad.size() << "\n";

        if (squad.empty()) {
            cout << "Squad is empty.\n";
            return;
        }

        int bats = 0, bowls = 0, ar = 0, wk = 0;
        for (size_t i = 0; i < squad.size(); i++) {
            cout << "- ";
            squad[i]->displayDetails();
            cout << " | Bought for: Rs. " << squad[i]->getSoldPrice() << " Cr\n";

            if (squad[i]->getRole() == "Batsman") bats++;
            else if (squad[i]->getRole() == "Bowler") bowls++;
            else if (squad[i]->getRole() == "All-Rounder") ar++;
            else if (squad[i]->getRole() == "Wicket-Keeper") wk++;
        }
        cout << "Role Counts: Batsmen: " << bats << ", Bowlers: " << bowls
             << ", All-Rounders: " << ar << ", Wicket-Keepers: " << wk << "\n";
    }
};

// Function Overloading (Polymorphism)
Player* findPlayer(const vector<Player*>& pool, int id) {
    for (size_t i = 0; i < pool.size(); i++) {
        if (pool[i]->getId() == id) return pool[i];
    }
    return nullptr;
}

Player* findPlayer(const vector<Player*>& pool, string name) {
    for (size_t i = 0; i < pool.size(); i++) {
        if (pool[i]->getName() == name) return pool[i];
    }
    return nullptr;
}

// Auction System Class
class AuctionSystem {
private:
    vector<Player*>& playerPool;
    vector<Team>& teams;

public:
    AuctionSystem(vector<Player*>& pool, vector<Team>& tList)
        : playerPool(pool), teams(tList) {}

    Player* getNextUnsoldPlayer() {
        for (size_t i = 0; i < playerPool.size(); i++) {
            if (!playerPool[i]->getIsSold()) return playerPool[i];
        }
        return nullptr;
    }

    void auctionPlayer(Player* p) {
        if (p == nullptr) {
            cout << "\nNo unsold players left in the pool.\n";
            return;
        }

        cout << "\n--- Player Up For Auction ---\n";
        p->displayDetails();
        cout << "\n";

        double currentBid = p->getBasePrice();
        int highestBidderIndex = -1;

        int choice = -1;
        while (choice != 0) {
            cout << "\nCurrent Highest Bid: Rs. " << currentBid << " Cr";
            if (highestBidderIndex != -1) {
                cout << " by " << teams[highestBidderIndex].getName();
            } else {
                cout << " (Base Price)";
            }
            cout << "\n";

            cout << "Choose a team to raise bid (+Rs. 0.25 Cr):\n";
            for (size_t i = 0; i < teams.size(); i++) {
                cout << (i + 1) << ". " << teams[i].getName() 
                     << " (Budget: Rs. " << teams[i].getRemainingBudget() << " Cr)\n";
            }
            cout << "0. Sell to current highest bidder / Pass\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice >= 1 && choice <= (int)teams.size()) {
                int teamIdx = choice - 1;
                double nextBid = (highestBidderIndex == -1) ? currentBid : (currentBid + 0.25);

                if (teams[teamIdx].canAfford(nextBid)) {
                    currentBid = nextBid;
                    highestBidderIndex = teamIdx;
                    cout << teams[teamIdx].getName() << " bid Rs. " << currentBid << " Cr!\n";
                } else {
                    cout << "Not enough budget in " << teams[teamIdx].getName() << "!\n";
                }
            } else if (choice != 0) {
                cout << "Invalid choice!\n";
            }
        }

        if (highestBidderIndex != -1) {
            Team& winner = teams[highestBidderIndex];
            winner.deductBudget(currentBid);
            p->assignToTeam(winner.getName(), currentBid);
            winner.addPlayer(p);
            cout << "\nSOLD: " << p->getName() << " sold to " 
                 << winner.getName() << " for Rs. " << currentBid << " Cr!\n";
        } else {
            cout << "\nUNSOLD: No bids for " << p->getName() << ".\n";
        }
    }
};

void seedInitialData(vector<Player*>& pool, vector<Team>& teams) {
    teams.push_back(Team(1, "Roaring Lions", "Lion", 100.0));
    teams.push_back(Team(2, "Fierce Tigers", "Tiger", 100.0));
    teams.push_back(Team(3, "Mighty Elephants", "Elephant", 100.0));
    teams.push_back(Team(4, "Swift Falcons", "Falcon", 100.0));

    // Batsmen
    pool.push_back(new Batsman(101, "Aarav Stone", 2.0, 3850, 142.5, 4));
    pool.push_back(new Batsman(102, "Leo Vance", 1.5, 2900, 135.2, 2));
    pool.push_back(new Batsman(103, "Maya Sterling", 2.0, 3400, 139.8, 3));
    pool.push_back(new Batsman(104, "Rohan Ray", 1.0, 2150, 128.6, 1));

    // Bowlers
    pool.push_back(new Bowler(201, "Shadow Bolt", 2.0, 95, 7.15));
    pool.push_back(new Bowler(202, "Spike Archer", 1.5, 82, 7.60));
    pool.push_back(new Bowler(203, "Tornado Thorne", 1.5, 88, 7.40));
    pool.push_back(new Bowler(204, "Axel Swift", 1.0, 64, 8.10));

    // All-Rounders
    pool.push_back(new AllRounder(301, "Blaze Rivera", 2.0, 2400, 68, 141.0, 7.80));
    pool.push_back(new AllRounder(302, "Hunter Cross", 1.5, 1850, 52, 133.5, 8.20));
    pool.push_back(new AllRounder(303, "Phoenix Drake", 2.0, 2600, 74, 144.2, 7.50));
    pool.push_back(new AllRounder(304, "Zenith Cole", 1.0, 1400, 40, 126.0, 8.50));

    // Wicket-Keepers
    pool.push_back(new WicketKeeper(401, "Dexter Fox", 1.5, 2750, 56));
    pool.push_back(new WicketKeeper(402, "Orion Peak", 1.5, 2400, 48));
    pool.push_back(new WicketKeeper(403, "Jasper Reed", 1.0, 1950, 39));
    pool.push_back(new WicketKeeper(404, "Milo Finch", 1.0, 1650, 31));
}

void cleanUpMemory(vector<Player*>& pool) {
    for (size_t i = 0; i < pool.size(); i++) {
        delete pool[i];
    }
    pool.clear();
}

int main() {
    vector<Player*> playerPool;
    vector<Team> teams;

    seedInitialData(playerPool, teams);
    AuctionSystem auction(playerPool, teams);

    int choice = -1;
    while (choice != 0) {
        cout << "\n--- Player Auction Management System ---\n";
        cout << "1. View Registered Player Pool\n";
        cout << "2. View Teams and Budgets\n";
        cout << "3. Search Player by ID or Name\n";
        cout << "4. Auction Next Unsold Player\n";
        cout << "5. View Team Squads\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "\n--- Registered Players ---\n";
                for (size_t i = 0; i < playerPool.size(); i++) {
                    playerPool[i]->displayDetails();
                    if (playerPool[i]->getIsSold()) {
                        cout << " | [Sold to: " << playerPool[i]->getTeamName() << " for Rs. " << playerPool[i]->getSoldPrice() << " Cr]";
                    } else {
                        cout << " | [Available]";
                    }
                    cout << "\n";
                }
                break;
            }

            case 2: {
                cout << "\n--- Teams and Budgets ---\n";
                for (size_t i = 0; i < teams.size(); i++) {
                    cout << teams[i].getName() << " (" << teams[i].getMascot() << ")"
                         << " | Budget Left: Rs. " << teams[i].getRemainingBudget() << " Cr"
                         << " | Players: " << teams[i].getSquadSize() << "\n";
                }
                break;
            }

            case 3: {
                cout << "\nSearch by: 1. ID  2. Name\nEnter choice: ";
                int subChoice;
                cin >> subChoice;
                if (subChoice == 1) {
                    cout << "Enter Player ID: ";
                    int pId;
                    cin >> pId;
                    Player* p = findPlayer(playerPool, pId);
                    if (p != nullptr) {
                        cout << "\nPlayer Found:\n";
                        p->displayDetails();
                        cout << "\nStatus: " << (p->getIsSold() ? ("Sold to " + p->getTeamName()) : "Available") << "\n";
                    } else {
                        cout << "No player found with ID " << pId << "!\n";
                    }
                } else if (subChoice == 2) {
                    cout << "Enter Player First Name: ";
                    string pName;
                    cin >> pName;
                    Player* found = nullptr;
                    for (size_t i = 0; i < playerPool.size(); i++) {
                        if (playerPool[i]->getName().find(pName) != string::npos) {
                            found = playerPool[i];
                            break;
                        }
                    }
                    if (found != nullptr) {
                        cout << "\nPlayer Found:\n";
                        found->displayDetails();
                        cout << "\nStatus: " << (found->getIsSold() ? ("Sold to " + found->getTeamName()) : "Available") << "\n";
                    } else {
                        cout << "No player found with Name: " << pName << "!\n";
                    }
                } else {
                    cout << "Invalid choice.\n";
                }
                break;
            }

            case 4: {
                Player* nextPlayer = auction.getNextUnsoldPlayer();
                if (nextPlayer != nullptr) {
                    auction.auctionPlayer(nextPlayer);
                } else {
                    cout << "\nAll players in the pool are already sold!\n";
                }
                break;
            }

            case 5: {
                for (size_t i = 0; i < teams.size(); i++) {
                    teams[i].displaySquad();
                }
                break;
            }

            case 0: {
                cout << "\nExiting Auction System. Goodbye!\n";
                break;
            }

            default: {
                cout << "Invalid choice! Please select 0 to 5.\n";
                break;
            }
        }
    }

    cleanUpMemory(playerPool);
    return 0;
}
