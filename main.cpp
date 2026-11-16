/*
 * =====================================================================================
 *  PROJECT: Sports Tournament & Player Auction Management System (IPL-Style)
 *  LANGUAGE: C++ (Standard C++)
 *  
 *  HOW TO COMPILE & RUN:
 *      g++ main.cpp -o main.exe
 *      .\main.exe
 *
 *  -----------------------------------------------------------------------------------
 *  QUICK REFERENCE GUIDE FOR VIVA / LAB EVALUATION:
 *  -----------------------------------------------------------------------------------
 *  1. ABSTRACTION:
 *     - Abstract base class 'Player' has pure virtual functions:
 *       virtual void displayDetails() const = 0;
 *       virtual double calculateRating() const = 0;
 *
 *  2. INHERITANCE:
 *     - Subclasses inherit from Player:
 *       Batsman, Bowler, AllRounder, WicketKeeper.
 *
 *  3. ENCAPSULATION:
 *     - Data members in 'Team' and 'Player' are private/protected.
 *     - Purse is safely deducted using deductBudget(amount).
 *
 *  4. RUNTIME POLYMORPHISM (OVERRIDING):
 *     - Storing derived objects in 'vector<Player*> squad'.
 *     - Calling player->displayDetails() and player->calculateRating() calls the
 *       correct derived class method at runtime.
 *
 *  5. COMPILE-TIME POLYMORPHISM (FUNCTION OVERLOADING):
 *     - Overloaded functions: findPlayer(pool, id) and findPlayer(pool, name).
 *
 *  6. COMPOSITION & AGGREGATION:
 *     - 'Team' contains a squad of Player pointers (vector<Player*>).
 *     - 'Tournament' manages Team references and match simulations.
 * =====================================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// =====================================================================================
// 1. OOPS CONCEPT: ABSTRACTION & BASE CLASS
// =====================================================================================
// 'Player' is an abstract base class with pure virtual functions (= 0).
class Player {
protected:
    int id;
    string name;
    string role;
    double basePrice;   // in Crores (e.g., 2.0 = Rs. 2.00 Cr)
    double soldPrice;
    bool isSold;
    string teamName;

public:
    // Parameterized Constructor
    Player(int pId, string pName, string pRole, double bPrice) {
        id = pId;
        name = pName;
        role = pRole;
        basePrice = bPrice;
        soldPrice = 0.0;
        isSold = false;
        teamName = "None";
    }

    // Virtual Destructor
    virtual ~Player() {}

    // Getters (Encapsulation)
    int getId() const { return id; }
    string getName() const { return name; }
    string getRole() const { return role; }
    double getBasePrice() const { return basePrice; }
    double getSoldPrice() const { return soldPrice; }
    bool getIsSold() const { return isSold; }
    string getTeamName() const { return teamName; }

    // Assign player to winning franchise
    void assignToTeam(string tName, double price) {
        teamName = tName;
        soldPrice = price;
        isSold = true;
    }

    // Pure Virtual Functions (Abstraction)
    virtual void displayDetails() const = 0;
    virtual double calculateRating() const = 0;
};

// =====================================================================================
// 2. OOPS CONCEPT: INHERITANCE & POLYMORPHISM (DERIVED CLASSES)
// =====================================================================================

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

    // Function Overriding
    void displayDetails() const override {
        cout << left << setw(4) << id
             << setw(18) << name
             << setw(15) << role
             << "Rs. " << fixed << setprecision(2) << setw(5) << basePrice << " Cr  "
             << "Runs: " << setw(5) << runs
             << "  SR: " << setw(6) << strikeRate
             << "  100s: " << centuries;
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
        cout << left << setw(4) << id
             << setw(18) << name
             << setw(15) << role
             << "Rs. " << fixed << setprecision(2) << setw(5) << basePrice << " Cr  "
             << "Wkts: " << setw(5) << wickets
             << "  Eco: " << setw(6) << economy;
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
        cout << left << setw(4) << id
             << setw(18) << name
             << setw(15) << role
             << "Rs. " << fixed << setprecision(2) << setw(5) << basePrice << " Cr  "
             << "Runs: " << setw(5) << runs
             << "  Wkts: " << setw(3) << wickets
             << "  SR: " << strikeRate;
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
        cout << left << setw(4) << id
             << setw(18) << name
             << setw(15) << role
             << "Rs. " << fixed << setprecision(2) << setw(5) << basePrice << " Cr  "
             << "Runs: " << setw(5) << runs
             << "  Dismissals: " << dismissals;
    }

    double calculateRating() const override {
        return (runs / 50.0) + (dismissals * 2.0);
    }
};

// =====================================================================================
// 3. OOPS CONCEPT: ENCAPSULATION & AGGREGATION ('Team' Class)
// =====================================================================================
class Team {
private:
    int id;
    string name;
    string mascot;
    double totalBudget;     // Rs. 100.0 Cr
    double remainingBudget; // Purse left
    vector<Player*> squad;  // Aggregation: Team has players

public:
    Team(int tId, string tName, string tMascot, double budget = 100.0) {
        id = tId;
        name = tName;
        mascot = tMascot;
        totalBudget = budget;
        remainingBudget = budget;
    }

    // Getters
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
        cout << "\n==============================================================================\n";
        cout << " [Franchise] " << name << " (" << mascot << ")\n";
        cout << " Remaining Purse: Rs. " << fixed << setprecision(2) << remainingBudget << " Cr / Rs. " << totalBudget << " Cr\n";
        cout << " Total Players: " << squad.size() << "\n";
        cout << "------------------------------------------------------------------------------\n";

        if (squad.empty()) {
            cout << " (Squad is currently empty. Buy players in auction!)\n";
            return;
        }

        int bats = 0, bowls = 0, ar = 0, wk = 0;
        for (size_t i = 0; i < squad.size(); i++) {
            cout << " - ";
            squad[i]->displayDetails();
            cout << " | Sold: Rs. " << fixed << setprecision(2) << squad[i]->getSoldPrice() << " Cr\n";

            if (squad[i]->getRole() == "Batsman") bats++;
            else if (squad[i]->getRole() == "Bowler") bowls++;
            else if (squad[i]->getRole() == "All-Rounder") ar++;
            else if (squad[i]->getRole() == "Wicket-Keeper") wk++;
        }
        cout << "------------------------------------------------------------------------------\n";
        cout << " Squad Composition: [Batsmen: " << bats << " | Bowlers: " << bowls
             << " | All-Rounders: " << ar << " | Wicket-Keepers: " << wk << "]\n";
        cout << "==============================================================================\n";
    }

    double getOverallTeamStrength() const {
        if (squad.empty()) return 50.0;
        double totalRating = 0.0;
        for (size_t i = 0; i < squad.size(); i++) {
            totalRating += squad[i]->calculateRating();
        }
        return totalRating / squad.size();
    }
};

// =====================================================================================
// 4. OOPS CONCEPT: FUNCTION OVERLOADING
// =====================================================================================
// Overload 1: Find by ID
Player* findPlayer(const vector<Player*>& pool, int id) {
    for (size_t i = 0; i < pool.size(); i++) {
        if (pool[i]->getId() == id) return pool[i];
    }
    return nullptr;
}

// Overload 2: Find by Name
Player* findPlayer(const vector<Player*>& pool, string name) {
    for (size_t i = 0; i < pool.size(); i++) {
        if (pool[i]->getName() == name) return pool[i];
    }
    return nullptr;
}

// =====================================================================================
// 5. AUCTION SYSTEM MODULE
// =====================================================================================
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

    void auctionSinglePlayer(Player* p, bool autoMode = false) {
        if (p == nullptr) {
            cout << "\n[!] No unsold player found!\n";
            return;
        }

        cout << "\n------------------------------------------------------------------------------\n";
        cout << " >>> UP FOR BIDDING: ";
        p->displayDetails();
        cout << "\n------------------------------------------------------------------------------\n";

        double currentBid = p->getBasePrice();
        int highestBidderIndex = -1;

        if (!autoMode) {
            // Interactive Bidding
            int choice = -1;
            while (choice != 0) {
                cout << "\nCurrent Highest Bid: Rs. " << fixed << setprecision(2) << currentBid << " Cr";
                if (highestBidderIndex != -1) {
                    cout << " by " << teams[highestBidderIndex].getName();
                } else {
                    cout << " (Base Price)";
                }
                cout << "\n";

                cout << "Select Franchise to raise bid (+Rs. 0.25 Cr):\n";
                for (size_t i = 0; i < teams.size(); i++) {
                    cout << "  " << (i + 1) << ". " << teams[i].getName()
                         << " (Purse: Rs. " << teams[i].getRemainingBudget() << " Cr)\n";
                }
                cout << "  0. Sell to current highest bidder / Pass\n";
                cout << "Enter choice: ";
                cin >> choice;

                if (choice >= 1 && choice <= (int)teams.size()) {
                    int teamIdx = choice - 1;
                    double nextBid = (highestBidderIndex == -1) ? currentBid : (currentBid + 0.25);

                    if (teams[teamIdx].canAfford(nextBid)) {
                        currentBid = nextBid;
                        highestBidderIndex = teamIdx;
                        cout << ">> " << teams[teamIdx].getName() << " placed bid of Rs. " << currentBid << " Cr!\n";
                    } else {
                        cout << "[X] Insufficient funds in " << teams[teamIdx].getName() << "'s purse!\n";
                    }
                } else if (choice != 0) {
                    cout << "[!] Invalid choice!\n";
                }
            }
        } else {
            // Fast Auto-Bidding
            vector<int> interestedTeams;
            for (size_t i = 0; i < teams.size(); i++) {
                if (teams[i].canAfford(p->getBasePrice() + 1.0) && teams[i].getSquadSize() < 5) {
                    interestedTeams.push_back(i);
                }
            }

            if (!interestedTeams.empty()) {
                int winner = interestedTeams[rand() % interestedTeams.size()];
                double randomHike = (rand() % 5) * 0.50; // 0.0 to 2.0 Cr hike
                double finalBid = p->getBasePrice() + randomHike;

                if (teams[winner].canAfford(finalBid)) {
                    currentBid = finalBid;
                    highestBidderIndex = winner;
                }
            }
        }

        // Finalize Outcome
        if (highestBidderIndex != -1) {
            Team& winner = teams[highestBidderIndex];
            winner.deductBudget(currentBid);
            p->assignToTeam(winner.getName(), currentBid);
            winner.addPlayer(p);
            cout << "\nSOLD! " << p->getName() << " is SOLD to "
                 << winner.getName() << " for Rs. " << fixed << setprecision(2) << currentBid << " Cr!\n";
        } else {
            cout << "\nUNSOLD! No valid bids received for " << p->getName() << ".\n";
        }
    }

    void runFullAuction(bool autoMode) {
        cout << "\n==============================================================================\n";
        cout << "                  IPL MEGA-AUCTION SESSION COMMENCING                         \n";
        cout << "==============================================================================\n";

        for (size_t i = 0; i < playerPool.size(); i++) {
            if (!playerPool[i]->getIsSold()) {
                auctionSinglePlayer(playerPool[i], autoMode);
            }
        }

        cout << "\n[✓] Auction session completed for all registered players!\n";
    }
};

// =====================================================================================
// 6. TOURNAMENT & MATCH SIMULATION ENGINE
// =====================================================================================
struct TeamStanding {
    string teamName;
    int matchesPlayed;
    int won;
    int lost;
    int points;
    double netRunRate;

    TeamStanding() {
        teamName = "";
        matchesPlayed = 0;
        won = 0;
        lost = 0;
        points = 0;
        netRunRate = 0.0;
    }
};

class Tournament {
private:
    vector<Team>& teams;
    vector<TeamStanding> standings;

    void simulateMatch(Team& teamA, Team& teamB, int matchNo) {
        cout << "\n------------------------------------------------------------------------------\n";
        cout << " Match #" << matchNo << ": " << teamA.getName() << " vs " << teamB.getName() << "\n";
        cout << " Format: 20 Overs T20\n";
        cout << "------------------------------------------------------------------------------\n";

        double strengthA = teamA.getOverallTeamStrength();
        double strengthB = teamB.getOverallTeamStrength();

        // Innings 1
        int scoreA = 120 + (int)(strengthA * 0.8) + (rand() % 35);
        int wicketsA = 3 + (rand() % 6);

        // Innings 2
        int scoreB = 115 + (int)(strengthB * 0.8) + (rand() % 35);
        int wicketsB = 4 + (rand() % 6);

        cout << " 1st Innings: " << teamA.getName() << " scored " << scoreA << "/" << wicketsA << " (20.0 ov)\n";
        cout << " 2nd Innings: " << teamB.getName() << " scored " << scoreB << "/" << wicketsB << " (20.0 ov)\n";

        int idxA = getStandingIndex(teamA.getName());
        int idxB = getStandingIndex(teamB.getName());

        standings[idxA].matchesPlayed++;
        standings[idxB].matchesPlayed++;

        double nrrDelta = (scoreA - scoreB) / 20.0;

        if (scoreA > scoreB) {
            cout << " Result: " << teamA.getName() << " won by " << (scoreA - scoreB) << " runs!\n";
            standings[idxA].won++;
            standings[idxA].points += 2;
            standings[idxB].lost++;
            standings[idxA].netRunRate += nrrDelta;
            standings[idxB].netRunRate -= nrrDelta;
        } else if (scoreB > scoreA) {
            cout << " Result: " << teamB.getName() << " won by " << (10 - wicketsB) << " wickets!\n";
            standings[idxB].won++;
            standings[idxB].points += 2;
            standings[idxA].lost++;
            standings[idxB].netRunRate += (-nrrDelta);
            standings[idxA].netRunRate -= (-nrrDelta);
        } else {
            cout << " Result: Match TIED! Points shared.\n";
            standings[idxA].points += 1;
            standings[idxB].points += 1;
        }
    }

    int getStandingIndex(string tName) {
        for (size_t i = 0; i < standings.size(); i++) {
            if (standings[i].teamName == tName) return i;
        }
        return 0;
    }

public:
    Tournament(vector<Team>& tList) : teams(tList) {
        for (size_t i = 0; i < teams.size(); i++) {
            TeamStanding ts;
            ts.teamName = teams[i].getName();
            standings.push_back(ts);
        }
    }

    void runTournament() {
        cout << "\n==============================================================================\n";
        cout << "                   ANIMAL FRANCHISE PREMIER LEAGUE BEGINS                     \n";
        cout << "==============================================================================\n";

        for (size_t i = 0; i < standings.size(); i++) {
            standings[i].matchesPlayed = 0;
            standings[i].won = 0;
            standings[i].lost = 0;
            standings[i].points = 0;
            standings[i].netRunRate = 0.0;
        }

        int matchCount = 1;
        for (size_t i = 0; i < teams.size(); i++) {
            for (size_t j = i + 1; j < teams.size(); j++) {
                simulateMatch(teams[i], teams[j], matchCount++);
            }
        }

        // Sort by Points
        sort(standings.begin(), standings.end(), [](const TeamStanding& a, const TeamStanding& b) {
            if (a.points != b.points) return a.points > b.points;
            return a.netRunRate > b.netRunRate;
        });

        displayPointsTable();
        playGrandFinal();
    }

    void displayPointsTable() const {
        cout << "\n==============================================================================\n";
        cout << "                         OFFICIAL POINTS TABLE                                \n";
        cout << "==============================================================================\n";
        cout << left << setw(4) << "Pos"
             << setw(20) << "Team"
             << setw(6) << "P"
             << setw(6) << "W"
             << setw(6) << "L"
             << setw(8) << "Pts"
             << setw(10) << "NRR" << "\n";
        cout << "------------------------------------------------------------------------------\n";

        for (size_t i = 0; i < standings.size(); i++) {
            cout << left << setw(4) << (i + 1)
                 << setw(20) << standings[i].teamName
                 << setw(6) << standings[i].matchesPlayed
                 << setw(6) << standings[i].won
                 << setw(6) << standings[i].lost
                 << setw(8) << standings[i].points
                 << fixed << setprecision(3) << setw(10) << standings[i].netRunRate << "\n";
        }
        cout << "==============================================================================\n";
    }

    void playGrandFinal() {
        if (standings.size() < 2) return;

        cout << "\n******************************************************************************\n";
        cout << "                      THE GRAND FINALE OF THE TOURNAMENT                      \n";
        cout << "******************************************************************************\n";

        string team1Name = standings[0].teamName;
        string team2Name = standings[1].teamName;

        Team* finalTeamA = nullptr;
        Team* finalTeamB = nullptr;

        for (size_t i = 0; i < teams.size(); i++) {
            if (teams[i].getName() == team1Name) finalTeamA = &teams[i];
            if (teams[i].getName() == team2Name) finalTeamB = &teams[i];
        }

        if (finalTeamA && finalTeamB) {
            cout << " Finalists: " << finalTeamA->getName() << " vs " << finalTeamB->getName() << "\n\n";
            int finalScoreA = 160 + (rand() % 40);
            int finalScoreB = 158 + (rand() % 40);

            cout << " " << finalTeamA->getName() << ": " << finalScoreA << "/6 (20.0 ov)\n";
            cout << " " << finalTeamB->getName() << ": " << finalScoreB << "/7 (20.0 ov)\n\n";

            if (finalScoreA >= finalScoreB) {
                cout << " CHAMPIONS: " << finalTeamA->getName() << " lifts the Trophy! 🏆\n";
            } else {
                cout << " CHAMPIONS: " << finalTeamB->getName() << " lifts the Trophy! 🏆\n";
            }
        }
        cout << "******************************************************************************\n";
    }
};

// =====================================================================================
// 7. DATA INITIALIZATION & CLEANUP
// =====================================================================================
void seedInitialData(vector<Player*>& pool, vector<Team>& teams) {
    // 4 Animal Franchises with Rs. 100.0 Cr budget each
    teams.push_back(Team(1, "Roaring Lions", "Lion", 100.0));
    teams.push_back(Team(2, "Fierce Tigers", "Tiger", 100.0));
    teams.push_back(Team(3, "Mighty Elephants", "Elephant", 100.0));
    teams.push_back(Team(4, "Swift Falcons", "Falcon", 100.0));

    // Batsmen (id, name, basePrice, runs, strikeRate, 100s)
    pool.push_back(new Batsman(101, "Aarav Stone", 2.0, 3850, 142.5, 4));
    pool.push_back(new Batsman(102, "Leo Vance", 1.5, 2900, 135.2, 2));
    pool.push_back(new Batsman(103, "Maya Sterling", 2.0, 3400, 139.8, 3));
    pool.push_back(new Batsman(104, "Rohan Ray", 1.0, 2150, 128.6, 1));

    // Bowlers (id, name, basePrice, wickets, economy)
    pool.push_back(new Bowler(201, "Shadow Bolt", 2.0, 95, 7.15));
    pool.push_back(new Bowler(202, "Spike Archer", 1.5, 82, 7.60));
    pool.push_back(new Bowler(203, "Tornado Thorne", 1.5, 88, 7.40));
    pool.push_back(new Bowler(204, "Axel Swift", 1.0, 64, 8.10));

    // All-Rounders (id, name, basePrice, runs, wickets, strikeRate, economy)
    pool.push_back(new AllRounder(301, "Blaze Rivera", 2.0, 2400, 68, 141.0, 7.80));
    pool.push_back(new AllRounder(302, "Hunter Cross", 1.5, 1850, 52, 133.5, 8.20));
    pool.push_back(new AllRounder(303, "Phoenix Drake", 2.0, 2600, 74, 144.2, 7.50));
    pool.push_back(new AllRounder(304, "Zenith Cole", 1.0, 1400, 40, 126.0, 8.50));

    // Wicket-Keepers (id, name, basePrice, runs, dismissals)
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

// =====================================================================================
// 8. MAIN INTERACTIVE CONSOLE INTERFACE
// =====================================================================================
int main() {
    srand(time(0));

    vector<Player*> playerPool;
    vector<Team> teams;

    // Seed sample players and animal franchises
    seedInitialData(playerPool, teams);

    AuctionSystem auction(playerPool, teams);
    Tournament tournament(teams);

    int choice = -1;
    while (choice != 0) {
        cout << "\n==============================================================================\n";
        cout << "       SPORTS TOURNAMENT & PLAYER AUCTION MANAGEMENT SYSTEM                   \n";
        cout << "==============================================================================\n";
        cout << " 1. View Registered Player Pool\n";
        cout << " 2. View Franchise Details & Remaining Budgets\n";
        cout << " 3. Search Player by ID or Name\n";
        cout << " 4. Auction Next Unsold Player (Interactive Bidding)\n";
        cout << " 5. Run Fast Automated Auction for All Remaining Players\n";
        cout << " 6. View Team Squads & Role Composition\n";
        cout << " 7. Simulate Full Tournament (Matches, Points Table & Grand Final)\n";
        cout << " 8. View Tournament Points Table\n";
        cout << " 0. Exit System\n";
        cout << "==============================================================================\n";
        cout << " Enter your choice (0-8): ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "\n--- REGISTERED PLAYER AUCTION POOL ---\n";
                cout << left << setw(4) << "ID"
                     << setw(18) << "Name"
                     << setw(15) << "Role"
                     << setw(14) << "Base Price"
                     << "Stats & Details\n";
                cout << "------------------------------------------------------------------------------\n";
                for (size_t i = 0; i < playerPool.size(); i++) {
                    playerPool[i]->displayDetails();
                    if (playerPool[i]->getIsSold()) {
                        cout << " | [SOLD: " << playerPool[i]->getTeamName() << " @ Rs. " << playerPool[i]->getSoldPrice() << " Cr]";
                    } else {
                        cout << " | [AVAILABLE]";
                    }
                    cout << "\n";
                }
                break;
            }

            case 2: {
                cout << "\n--- FRANCHISE STATUS & PURSES ---\n";
                for (size_t i = 0; i < teams.size(); i++) {
                    cout << "* " << left << setw(18) << teams[i].getName()
                         << " (" << teams[i].getMascot() << ")"
                         << " | Purse: Rs. " << fixed << setprecision(2) << setw(6) << teams[i].getRemainingBudget() << " Cr"
                         << " | Squad Size: " << teams[i].getSquadSize() << " players\n";
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
                        cout << "\n[✓] Found Player:\n";
                        p->displayDetails();
                        cout << "\nStatus: " << (p->getIsSold() ? ("Sold to " + p->getTeamName()) : "Available") << "\n";
                    } else {
                        cout << "[X] No player found with ID " << pId << "!\n";
                    }
                } else if (subChoice == 2) {
                    cout << "Enter Player First Name (e.g. Aarav, Shadow, Blaze, Dexter): ";
                    string pName;
                    cin >> pName;
                    // Find player whose name starts with or equals pName
                    Player* found = nullptr;
                    for (size_t i = 0; i < playerPool.size(); i++) {
                        if (playerPool[i]->getName().find(pName) != string::npos) {
                            found = playerPool[i];
                            break;
                        }
                    }
                    if (found != nullptr) {
                        cout << "\n[✓] Found Player:\n";
                        found->displayDetails();
                        cout << "\nStatus: " << (found->getIsSold() ? ("Sold to " + found->getTeamName()) : "Available") << "\n";
                    } else {
                        cout << "[X] No player found with Name '" << pName << "'!\n";
                    }
                } else {
                    cout << "[!] Invalid search choice.\n";
                }
                break;
            }

            case 4: {
                Player* nextPlayer = auction.getNextUnsoldPlayer();
                if (nextPlayer != nullptr) {
                    auction.auctionSinglePlayer(nextPlayer, false);
                } else {
                    cout << "\n[✓] All players in the auction pool are already SOLD!\n";
                }
                break;
            }

            case 5: {
                auction.runFullAuction(true);
                break;
            }

            case 6: {
                for (size_t i = 0; i < teams.size(); i++) {
                    teams[i].displaySquad();
                }
                break;
            }

            case 7: {
                tournament.runTournament();
                break;
            }

            case 8: {
                tournament.displayPointsTable();
                break;
            }

            case 0: {
                cout << "\nExiting IPL Auction & Tournament Management System. Goodbye!\n";
                break;
            }

            default: {
                cout << "\n[!] Invalid choice! Please select an option between 0 and 8.\n";
                break;
            }
        }
    }

    cleanUpMemory(playerPool);

    return 0;
}
