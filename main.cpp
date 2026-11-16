/*
 * =====================================================================================
 *  PROJECT: Sports Tournament & Player Auction Management System (IPL-Style)
 *  LANGUAGE: C++ (Standard C++11 / C++17)
 *  
 *  QUICK COMPILATION & EXECUTION COMMAND:
 *      g++ -std=c++17 main.cpp -o main.exe
 *      .\main.exe
 *
 *  -----------------------------------------------------------------------------------
 *  QUICK REFERENCE GUIDE FOR VIVA / LAB EVALUATION:
 *  -----------------------------------------------------------------------------------
 *  1. ABSTRACTION:
 *     - Line ~60: Abstract base class 'Player' defines pure virtual functions:
 *       virtual void displayDetails() const = 0;
 *       virtual double calculateRating() const = 0;
 *
 *  2. INHERITANCE:
 *     - Line ~110-180: Subclasses inherit from Player:
 *       Batsman, Bowler, AllRounder, WicketKeeper.
 *
 *  3. ENCAPSULATION:
 *     - Line ~190: Data members in 'Team' (remainingBudget, squad) are private.
 *       Mutated safely via bool deductBudget(double amount).
 *
 *  4. RUNTIME POLYMORPHISM (OVERRIDING):
 *     - Storing derived objects in 'vector<Player*> squad'.
 *     - Calling player->displayDetails() and player->calculateRating() invokes the
 *       correct derived class implementation dynamically via the V-Table.
 *
 *  5. COMPILE-TIME POLYMORPHISM (FUNCTION OVERLOADING):
 *     - Line ~260: Overloaded functions findPlayer(vector, int id) and
 *       findPlayer(vector, string name).
 *
 *  6. COMPOSITION & AGGREGATION:
 *     - 'Team' aggregates a squad of Player pointers (vector<Player*>).
 *     - 'Tournament' composes Team references and Match simulations.
 *
 *  7. FILE HANDLING:
 *     - Line ~400: 'FileManager' writes auction reports (auction_summary.txt)
 *       and standings (tournament_results.txt) using <fstream>.
 * =====================================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// =====================================================================================
// 1. OOPS CONCEPT: ABSTRACTION & BASE CLASS
// =====================================================================================
// 'Player' is an abstract base class. It defines common attributes and pure virtual
// functions that every derived player role must implement.
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

    // Virtual Destructor (Crucial for safe polymorphic deletion)
    virtual ~Player() {}

    // Getters (Encapsulation)
    int getId() const { return id; }
    string getName() const { return name; }
    string getRole() const { return role; }
    double getBasePrice() const { return basePrice; }
    double getSoldPrice() const { return soldPrice; }
    bool getIsSold() const { return isSold; }
    string getTeamName() const { return teamName; }

    // Method to assign player to a team upon successful auction bid
    void assignToTeam(string tName, double price) {
        teamName = tName;
        soldPrice = price;
        isSold = true;
    }

    // Pure Virtual Functions (Enforcing Abstraction & Runtime Polymorphism)
    virtual void displayDetails() const = 0;
    virtual double calculateRating() const = 0; // Performance metric used in match simulation
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
        : Player(pId, pName, "Batsman", bPrice), runs(r), strikeRate(sr), centuries(cent) {}

    // Overriding pure virtual function
    void displayDetails() const override {
        cout << left << setw(4) << id
             << setw(20) << name
             << setw(15) << role
             << "Rs. " << fixed << setprecision(2) << setw(5) << basePrice << " Cr  "
             << "Runs: " << setw(5) << runs
             << "  SR: " << setw(6) << strikeRate
             << "  100s: " << centuries;
    }

    // Specialized Rating Calculation for Batsman
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
        : Player(pId, pName, "Bowler", bPrice), wickets(w), economy(eco) {}

    void displayDetails() const override {
        cout << left << setw(4) << id
             << setw(20) << name
             << setw(15) << role
             << "Rs. " << fixed << setprecision(2) << setw(5) << basePrice << " Cr  "
             << "Wkts: " << setw(5) << wickets
             << "  Eco: " << setw(6) << economy;
    }

    // Specialized Rating Calculation for Bowler (Lower economy gives higher rating)
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
        : Player(pId, pName, "All-Rounder", bPrice), runs(r), wickets(w), strikeRate(sr), economy(eco) {}

    void displayDetails() const override {
        cout << left << setw(4) << id
             << setw(20) << name
             << setw(15) << role
             << "Rs. " << fixed << setprecision(2) << setw(5) << basePrice << " Cr  "
             << "Runs: " << setw(5) << runs
             << "  Wkts: " << setw(3) << wickets
             << "  SR: " << strikeRate;
    }

    // Specialized Rating combining both Batting and Bowling
    double calculateRating() const override {
        return (runs / 60.0) + (wickets * 1.2) + (strikeRate * 0.25) + ((10.0 - economy) * 4.0);
    }
};

// Derived Class 4: Wicket-Keeper
class WicketKeeper : public Player {
private:
    int runs;
    int dismissals; // Catches + Stumpings

public:
    WicketKeeper(int pId, string pName, double bPrice, int r, int dis)
        : Player(pId, pName, "Wicket-Keeper", bPrice), runs(r), dismissals(dis) {}

    void displayDetails() const override {
        cout << left << setw(4) << id
             << setw(20) << name
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
// Team aggregates a squad of Player pointers. It protects its budget and enforces
// safe budget deduction.
class Team {
private:
    int id;
    string name;
    string mascot;
    double totalBudget;     // Rs. 100.0 Cr
    double remainingBudget; // Purse left
    vector<Player*> squad;  // Aggregation of Players

public:
    // Constructor
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
    int getSquadSize() const { return (int)squad.size(); }
    const vector<Player*>& getSquad() const { return squad; }

    // Budget checks & deduction
    bool canAfford(double amount) const {
        return remainingBudget >= amount;
    }

    bool deductBudget(double amount) {
        if (canAfford(amount)) {
            remainingBudget -= amount;
            return true;
        }
        return false;
    }

    // Add Player to Squad
    void addPlayer(Player* p) {
        squad.push_back(p);
    }

    // Display Squad Details
    void displaySquad() const {
        cout << "\n==============================================================================\n";
        cout << " [Franchise] " << name << " (" << mascot << ")\n";
        cout << " Remaining Purse: Rs. " << fixed << setprecision(2) << remainingBudget << " Cr / Rs. " << totalBudget << " Cr\n";
        cout << " Total Players: " << squad.size() << "\n";
        cout << "------------------------------------------------------------------------------\n";

        if (squad.empty()) {
            cout << " (Squad is currently empty. Participate in the auction to buy players!)\n";
            return;
        }

        int bats = 0, bowls = 0, ar = 0, wk = 0;
        for (const auto* p : squad) {
            cout << " - ";
            p->displayDetails();
            cout << " | Sold: Rs. " << fixed << setprecision(2) << p->getSoldPrice() << " Cr\n";

            if (p->getRole() == "Batsman") bats++;
            else if (p->getRole() == "Bowler") bowls++;
            else if (p->getRole() == "All-Rounder") ar++;
            else if (p->getRole() == "Wicket-Keeper") wk++;
        }
        cout << "------------------------------------------------------------------------------\n";
        cout << " Squad Composition: [Batsmen: " << bats << " | Bowlers: " << bowls
             << " | All-Rounders: " << ar << " | Wicket-Keepers: " << wk << "]\n";
        cout << "==============================================================================\n";
    }

    // Calculate overall team strength based on player ratings
    double getOverallTeamStrength() const {
        if (squad.empty()) return 50.0; // Baseline
        double totalRating = 0.0;
        for (const auto* p : squad) {
            totalRating += p->calculateRating();
        }
        return totalRating / squad.size();
    }
};

// =====================================================================================
// 4. OOPS CONCEPT: FUNCTION OVERLOADING (Helper Functions)
// =====================================================================================
// Search a player by ID (Overload 1)
Player* findPlayer(const vector<Player*>& pool, int id) {
    for (auto* p : pool) {
        if (p->getId() == id) return p;
    }
    return nullptr;
}

// Search a player by Name (Overload 2)
Player* findPlayer(const vector<Player*>& pool, const string& name) {
    for (auto* p : pool) {
        if (p->getName() == name) return p;
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

    // Find the next unsold player in the pool
    Player* getNextUnsoldPlayer() {
        for (auto* p : playerPool) {
            if (!p->getIsSold()) return p;
        }
        return nullptr;
    }

    // Interactive or Auto Auction for a single player
    bool auctionSinglePlayer(Player* p, bool autoMode = false) {
        if (p == nullptr) {
            cout << "\n[!] All players in the auction pool are already sold!\n";
            return false;
        }

        if (p->getIsSold()) {
            cout << "\n[!] Player " << p->getName() << " is already SOLD to " << p->getTeamName() << "!\n";
            return false;
        }

        cout << "\n------------------------------------------------------------------------------\n";
        cout << " >>> UP FOR BIDDING: ";
        p->displayDetails();
        cout << "\n------------------------------------------------------------------------------\n";

        double currentBid = p->getBasePrice();
        int highestBidderIndex = -1; // -1 means unsold

        if (!autoMode) {
            // Interactive Bidding
            bool biddingActive = true;
            while (biddingActive) {
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
                int choice;
                if (!(cin >> choice)) {
                    if (cin.eof()) return false;
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }

                if (choice == 0) {
                    biddingActive = false;
                } else if (choice >= 1 && choice <= (int)teams.size()) {
                    int teamIdx = choice - 1;
                    double nextBid = (highestBidderIndex == -1) ? currentBid : (currentBid + 0.25);

                    if (teams[teamIdx].canAfford(nextBid)) {
                        currentBid = nextBid;
                        highestBidderIndex = teamIdx;
                        cout << ">> " << teams[teamIdx].getName() << " placed bid of Rs. " << currentBid << " Cr!\n";
                    } else {
                        cout << "[X] Insufficient funds in " << teams[teamIdx].getName() << "'s purse!\n";
                    }
                } else {
                    cout << "[!] Invalid option!\n";
                }
            }
        } else {
            // Intelligent Auto-Bidding (Simulated)
            vector<int> interestedTeams;
            for (size_t i = 0; i < teams.size(); i++) {
                if (teams[i].canAfford(p->getBasePrice() + 1.0) && teams[i].getSquadSize() < 5) {
                    interestedTeams.push_back((int)i);
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
            cout << "\n🔨 SOLD! " << p->getName() << " is SOLD to "
                 << winner.getName() << " for Rs. " << fixed << setprecision(2) << currentBid << " Cr!\n";
        } else {
            cout << "\n❌ UNSOLD! No valid bids received for " << p->getName() << ".\n";
        }
        return true;
    }

    // Run Full Auction for all unsold players
    void runFullAuction(bool autoMode) {
        cout << "\n==============================================================================\n";
        cout << "                  🏏 IPL MEGA-AUCTION SESSION COMMENCING 🏏                   \n";
        cout << "==============================================================================\n";

        for (auto* p : playerPool) {
            if (!p->getIsSold()) {
                if (!auctionSinglePlayer(p, autoMode)) break;
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
    int matchesPlayed = 0;
    int won = 0;
    int lost = 0;
    int points = 0;
    double netRunRate = 0.0;
};

class Tournament {
private:
    vector<Team>& teams;
    vector<TeamStanding> standings;

    // Simulate an exciting T20 match between two franchises
    void simulateMatch(Team& teamA, Team& teamB, int matchNo) {
        cout << "\n------------------------------------------------------------------------------\n";
        cout << " Match #" << matchNo << ": " << teamA.getName() << " vs " << teamB.getName() << "\n";
        cout << " Format: 20 Overs T20\n";
        cout << "------------------------------------------------------------------------------\n";

        double strengthA = teamA.getOverallTeamStrength();
        double strengthB = teamB.getOverallTeamStrength();

        // Innings 1: Team A bats (120-210 runs based on rating + random factor)
        int scoreA = 120 + (int)(strengthA * 0.8) + (rand() % 35);
        int wicketsA = 3 + (rand() % 6);

        // Innings 2: Team B chases
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
            cout << " Result: Match TIED! Points shared equally.\n";
            standings[idxA].points += 1;
            standings[idxB].points += 1;
        }
    }

    int getStandingIndex(const string& tName) {
        for (size_t i = 0; i < standings.size(); i++) {
            if (standings[i].teamName == tName) return (int)i;
        }
        return 0;
    }

public:
    Tournament(vector<Team>& tList) : teams(tList) {
        for (const auto& t : teams) {
            TeamStanding ts;
            ts.teamName = t.getName();
            standings.push_back(ts);
        }
    }

    // Run Round-Robin Tournament (Every team plays each other once = 6 matches)
    void runTournament() {
        cout << "\n==============================================================================\n";
        cout << "                🏆 ANIMAL FRANCHISE PREMIER LEAGUE BEGINS 🏆                  \n";
        cout << "==============================================================================\n";

        // Reset standings
        for (auto& s : standings) {
            s.matchesPlayed = 0;
            s.won = 0;
            s.lost = 0;
            s.points = 0;
            s.netRunRate = 0.0;
        }

        int matchCount = 1;
        for (size_t i = 0; i < teams.size(); i++) {
            for (size_t j = i + 1; j < teams.size(); j++) {
                simulateMatch(teams[i], teams[j], matchCount++);
            }
        }

        // Sort Standings by Points (Descending), then NRR
        sort(standings.begin(), standings.end(), [](const TeamStanding& a, const TeamStanding& b) {
            if (a.points != b.points) return a.points > b.points;
            return a.netRunRate > b.netRunRate;
        });

        displayPointsTable();
        playGrandFinal();
    }

    void displayPointsTable() const {
        cout << "\n==============================================================================\n";
        cout << "                         📊 OFFICIAL POINTS TABLE                             \n";
        cout << "==============================================================================\n";
        cout << left << setw(4) << "Pos"
             << setw(22) << "Team"
             << setw(6) << "P"
             << setw(6) << "W"
             << setw(6) << "L"
             << setw(8) << "Pts"
             << setw(10) << "NRR" << "\n";
        cout << "------------------------------------------------------------------------------\n";

        for (size_t i = 0; i < standings.size(); i++) {
            cout << left << setw(4) << (i + 1)
                 << setw(22) << standings[i].teamName
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
        cout << "                  🔥 THE GRAND FINALE OF THE TOURNAMENT 🔥                    \n";
        cout << "                  Top 2 Teams Clash for the Championship!                     \n";
        cout << "******************************************************************************\n";

        string team1Name = standings[0].teamName;
        string team2Name = standings[1].teamName;

        Team* finalTeamA = nullptr;
        Team* finalTeamB = nullptr;

        for (auto& t : teams) {
            if (t.getName() == team1Name) finalTeamA = &t;
            if (t.getName() == team2Name) finalTeamB = &t;
        }

        if (finalTeamA && finalTeamB) {
            cout << " Finalists: " << finalTeamA->getName() << " vs " << finalTeamB->getName() << "\n\n";
            int finalScoreA = 160 + (rand() % 40);
            int finalScoreB = 158 + (rand() % 40);

            cout << " " << finalTeamA->getName() << ": " << finalScoreA << "/6 (20.0 ov)\n";
            cout << " " << finalTeamB->getName() << ": " << finalScoreB << "/7 (20.0 ov)\n\n";

            if (finalScoreA >= finalScoreB) {
                cout << " 🎉 CHAMPIONS: " << finalTeamA->getName() << " lifts the Trophy! 🏆\n";
            } else {
                cout << " 🎉 CHAMPIONS: " << finalTeamB->getName() << " lifts the Trophy! 🏆\n";
            }
        }
        cout << "******************************************************************************\n";
    }

    const vector<TeamStanding>& getStandings() const { return standings; }
};

// =====================================================================================
// 7. OOPS CONCEPT: FILE HANDLING MODULE
// =====================================================================================
class FileManager {
public:
    // Export Auction Summary to a clean text report
    static void saveAuctionSummary(const vector<Player*>& pool, const vector<Team>& teams, const string& filename = "auction_summary.txt") {
        ofstream outFile(filename);
        if (!outFile) {
            cout << "[!] Error opening file for writing auction summary!\n";
            return;
        }

        outFile << "==============================================================================\n";
        outFile << "              IPL-STYLE CRICKET AUCTION SUMMARY REPORT                        \n";
        outFile << "==============================================================================\n\n";

        for (const auto& t : teams) {
            outFile << "Franchise: " << t.getName() << " (" << t.getMascot() << ")\n";
            outFile << "Remaining Purse: Rs. " << fixed << setprecision(2) << t.getRemainingBudget() << " Cr\n";
            outFile << "Players Bought (" << t.getSquadSize() << "):\n";
            for (const auto* p : t.getSquad()) {
                outFile << "  - " << left << setw(20) << p->getName()
                        << " | " << setw(15) << p->getRole()
                        << " | Rs. " << p->getSoldPrice() << " Cr\n";
            }
            outFile << "------------------------------------------------------------------------------\n\n";
        }

        outFile << "UNSOLD PLAYERS:\n";
        bool hasUnsold = false;
        for (const auto* p : pool) {
            if (!p->getIsSold()) {
                outFile << "  - " << left << setw(20) << p->getName()
                        << " | " << setw(15) << p->getRole()
                        << " | Base: Rs. " << p->getBasePrice() << " Cr\n";
                hasUnsold = true;
            }
        }
        if (!hasUnsold) outFile << "  (None. All players were sold!)\n";

        outFile.close();
        cout << "[✓] Auction summary successfully exported to: " << filename << "\n";
    }

    // Export Tournament Results to a text report
    static void saveTournamentResults(const vector<TeamStanding>& standings, const string& filename = "tournament_results.txt") {
        ofstream outFile(filename);
        if (!outFile) {
            cout << "[!] Error opening file for writing tournament results!\n";
            return;
        }

        outFile << "==============================================================================\n";
        outFile << "              ANIMAL PREMIER LEAGUE - TOURNAMENT STANDINGS                    \n";
        outFile << "==============================================================================\n\n";
        outFile << left << setw(4) << "Pos"
                << setw(22) << "Team"
                << setw(6) << "P"
                << setw(6) << "W"
                << setw(6) << "L"
                << setw(8) << "Pts"
                << setw(10) << "NRR" << "\n";
        outFile << "------------------------------------------------------------------------------\n";

        for (size_t i = 0; i < standings.size(); i++) {
            outFile << left << setw(4) << (i + 1)
                    << setw(22) << standings[i].teamName
                    << setw(6) << standings[i].matchesPlayed
                    << setw(6) << standings[i].won
                    << setw(6) << standings[i].lost
                    << setw(8) << standings[i].points
                    << fixed << setprecision(3) << setw(10) << standings[i].netRunRate << "\n";
        }

        outFile.close();
        cout << "[✓] Tournament standings exported to: " << filename << "\n";
    }
};

// =====================================================================================
// 8. DATA INITIALIZATION & CLEANUP
// =====================================================================================
void seedInitialData(vector<Player*>& pool, vector<Team>& teams) {
    // 1. Initialize 4 Animal-themed Franchises with Rs. 100.0 Cr budget each
    teams.push_back(Team(1, "Roaring Lions", "Lion", 100.0));
    teams.push_back(Team(2, "Fierce Tigers", "Tiger", 100.0));
    teams.push_back(Team(3, "Mighty Elephants", "Elephant", 100.0));
    teams.push_back(Team(4, "Swift Falcons", "Falcon", 100.0));

    // 2. Initialize Categorized Hypothetical Players
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
    for (auto* p : pool) {
        delete p;
    }
    pool.clear();
}

// =====================================================================================
// 9. MAIN INTERACTIVE CONSOLE INTERFACE
// =====================================================================================
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    vector<Player*> playerPool;
    vector<Team> teams;

    // Seed sample players and franchises
    seedInitialData(playerPool, teams);

    AuctionSystem auction(playerPool, teams);
    Tournament tournament(teams);

    int choice = -1;
    while (choice != 0) {
        cout << "\n==============================================================================\n";
        cout << "       🏏 SPORTS TOURNAMENT & PLAYER AUCTION MANAGEMENT SYSTEM 🏏             \n";
        cout << "==============================================================================\n";
        cout << " 1. View Registered Player Pool (Batsmen, Bowlers, All-Rounders, WKs)\n";
        cout << " 2. View Franchise Details & Remaining Budgets\n";
        cout << " 3. Search Player by ID or Name (Function Overloading Demo)\n";
        cout << " 4. Auction Next Unsold Player (Interactive Bidding)\n";
        cout << " 5. Run Fast Automated Auction for All Remaining Players\n";
        cout << " 6. View Team Squads & Role Composition\n";
        cout << " 7. Simulate Full Tournament (Matches, Points Table & Grand Final)\n";
        cout << " 8. View Tournament Points Table\n";
        cout << " 9. Export Reports to Files (auction_summary.txt & tournament_results.txt)\n";
        cout << " 0. Exit System\n";
        cout << "==============================================================================\n";
        cout << " Enter your choice (0-9): ";

        if (!(cin >> choice)) {
            if (cin.eof()) break;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[!] Please enter a valid number!\n";
            continue;
        }

        switch (choice) {
            case 1: {
                cout << "\n--- REGISTERED PLAYER AUCTION POOL ---\n";
                cout << left << setw(4) << "ID"
                     << setw(20) << "Name"
                     << setw(15) << "Role"
                     << setw(14) << "Base Price"
                     << "Stats & Details\n";
                cout << "------------------------------------------------------------------------------\n";
                for (const auto* p : playerPool) {
                    p->displayDetails();
                    if (p->getIsSold()) {
                        cout << " | [SOLD: " << p->getTeamName() << " @ Rs. " << p->getSoldPrice() << " Cr]";
                    } else {
                        cout << " | [AVAILABLE]";
                    }
                    cout << "\n";
                }
                break;
            }

            case 2: {
                cout << "\n--- FRANCHISE STATUS & PURSES ---\n";
                for (const auto& t : teams) {
                    cout << "• " << left << setw(20) << t.getName()
                         << " (" << t.getMascot() << ")"
                         << " | Purse: Rs. " << fixed << setprecision(2) << setw(6) << t.getRemainingBudget() << " Cr"
                         << " | Squad Size: " << t.getSquadSize() << " players\n";
                }
                break;
            }

            case 3: {
                cout << "\nSearch by: 1. ID  2. Name\nEnter choice: ";
                int subChoice;
                if (!(cin >> subChoice)) {
                    if (cin.eof()) break;
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }
                if (subChoice == 1) {
                    cout << "Enter Player ID: ";
                    int pId;
                    cin >> pId;
                    Player* p = findPlayer(playerPool, pId); // Calls overloaded function 1
                    if (p) {
                        cout << "\n[✓] Found Player:\n";
                        p->displayDetails();
                        cout << "\nStatus: " << (p->getIsSold() ? ("Sold to " + p->getTeamName()) : "Available") << "\n";
                    } else {
                        cout << "[X] No player found with ID " << pId << "!\n";
                    }
                } else if (subChoice == 2) {
                    cout << "Enter Player Name (e.g., 'Aarav Stone'): ";
                    cin.ignore();
                    string pName;
                    getline(cin, pName);
                    Player* p = findPlayer(playerPool, pName); // Calls overloaded function 2
                    if (p) {
                        cout << "\n[✓] Found Player:\n";
                        p->displayDetails();
                        cout << "\nStatus: " << (p->getIsSold() ? ("Sold to " + p->getTeamName()) : "Available") << "\n";
                    } else {
                        cout << "[X] No player found with Name '" << pName << "'!\n";
                    }
                } else {
                    cout << "[!] Invalid search choice.\n";
                }
                break;
            }

            case 4: {
                // Interactive auction for next unsold player
                Player* nextPlayer = auction.getNextUnsoldPlayer();
                if (nextPlayer) {
                    auction.auctionSinglePlayer(nextPlayer, false);
                } else {
                    cout << "\n[✓] All players in the auction pool are already SOLD!\n";
                }
                break;
            }

            case 5: {
                // Fast auto-auction for remaining players
                auction.runFullAuction(true);
                break;
            }

            case 6: {
                for (const auto& t : teams) {
                    t.displaySquad();
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

            case 9: {
                FileManager::saveAuctionSummary(playerPool, teams);
                FileManager::saveTournamentResults(tournament.getStandings());
                break;
            }

            case 0: {
                cout << "\nExiting IPL Auction & Tournament Management System. Goodbye!\n";
                break;
            }

            default: {
                cout << "\n[!] Invalid choice! Please select an option between 0 and 9.\n";
                break;
            }
        }
    }

    // Clean up allocated memory for players
    cleanUpMemory(playerPool);

    return 0;
}
