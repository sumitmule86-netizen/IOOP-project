# 🏏 Sports Player Auction Management System (IPL-Style)
> **An Object-Oriented Console Application in C++**

---

## 📌 Project Overview

The **Sports Player Auction Management System** is an object-oriented software application inspired by the Indian Premier League (IPL) mega-auction mechanism. It models four animal-themed franchises, each initialized with a fixed purse of **Rs. 100 Crore**, competing to acquire specialized cricket players (Batsmen, Bowlers, All-Rounders, and Wicket-Keepers) through interactive bidding wars.

---

## 🦁 Animal-Themed Franchises & Budget Setup

| Franchise Name | Mascot / Animal | Initial Purse / Budget | Max Squad Target |
| :--- | :---: | :---: | :---: |
| **Roaring Lions** | 🦁 Lion | Rs. 100.00 Crore | 5-6 Players |
| **Fierce Tigers** | 🐯 Tiger | Rs. 100.00 Crore | 5-6 Players |
| **Mighty Elephants** | 🐘 Elephant | Rs. 100.00 Crore | 5-6 Players |
| **Swift Falcons** | 🦅 Falcon | Rs. 100.00 Crore | 5-6 Players |

---

## 🎭 Hypothetical Player Pool Preview

Players are categorized into 4 core cricketing specializations, each with distinct attributes and base prices:

```
[Batsman]        Aarav Stone     | Base: Rs. 2.00 Cr | Runs: 3850, Strike Rate: 142.5, 100s: 4
[Bowler]         Shadow Bolt     | Base: Rs. 2.00 Cr | Wickets: 95, Economy: 7.15
[All-Rounder]    Blaze Rivera    | Base: Rs. 2.00 Cr | Runs: 2400, Wickets: 68, Bat SR: 141.0
[Wicket-Keeper]  Dexter Fox      | Base: Rs. 1.50 Cr | Runs: 2750, Dismissals: 56
```

---

## 🏛️ Comprehensive OOPS Mapping (Viva Ready)

| OOP Concept | Project Implementation Detail | Practical Code Example |
| :--- | :--- | :--- |
| **1. Abstraction** | An abstract base class `Player` enforces a uniform contract via pure virtual functions (`= 0`). | `virtual void displayDetails() const = 0;`<br>`virtual double calculateRating() const = 0;` |
| **2. Encapsulation** | Sensitive data (`remainingBudget`, `soldPrice`, `squad`) is `private`/`protected`. Mutated strictly via validated methods. | `bool Team::deductBudget(double amount)` prevents overspending. |
| **3. Inheritance** | `Player` is the generalized base class. Specialized subclasses (`Batsman`, `Bowler`, `AllRounder`, `WicketKeeper`) inherit common properties and introduce role-specific attributes. | `class Batsman : public Player`<br>`class Bowler : public Player` |
| **4. Polymorphism (Runtime)** | Overriding base class methods in derived classes. Storing derived objects in base pointers `vector<Player*> squad`. | `player->displayDetails();`<br>`player->calculateRating();` (Dynamic dispatch via V-Table) |
| **5. Polymorphism (Compile-time)** | Function overloading for player searches by ID or Name. | `findPlayer(pool, int id)` vs `findPlayer(pool, string name)` |
| **6. Composition & Aggregation** | `Team` aggregates a collection of `Player*` pointers. | `std::vector<Player*> squad;` |
| **7. Resource Management** | Virtual destructor in `Player` guarantees clean polymorphic object deletion. | `virtual ~Player() {}` |

---

## 📊 System Architecture & Data Flow

```mermaid
flowchart TD
    subgraph Data_Layer ["💾 In-Memory Data Store"]
        P1["Categorized Player Pool<br>(Batsmen, Bowlers, AR, WK)"]
        T1["Animal Franchise Purses<br>(Rs. 100 Cr each)"]
    end

    subgraph Core_Engine ["⚙️ Core Business Logic"]
        PM["Player Management<br>(Inheritance Hierarchy)"]
        TM["Team Management<br>(Budget & Squad Composition)"]
        AE["Auction Engine<br>(Interactive & Fast Auto-Bidding)"]
    end

    subgraph Presentation_Layer ["🖥️ Terminal User Interface (CLI)"]
        UI["Interactive Console Dashboard"]
        Menu1["1. View Player Pool"]
        Menu2["2. View Franchises & Purse"]
        Menu3["3. Search Player (ID/Name)"]
        Menu4["4. Auction Next Unsold Player"]
        Menu5["5. Fast Automated Auction"]
        Menu6["6. View Team Squads"]
        Menu0["0. Exit System"]
    end

    P1 --> PM
    T1 --> TM
    PM --> AE
    TM --> AE
    AE --> TM

    UI --> Menu1
    UI --> Menu2
    UI --> Menu3
    UI --> Menu4
    UI --> Menu5
    UI --> Menu6
    UI --> Menu0
```

---

## 🧩 Class Hierarchy & Relationship Diagram

```mermaid
classDiagram
    direction TB

    class Player {
        <<Abstract>>
        #int id
        #string name
        #string role
        #double basePrice
        #double soldPrice
        #bool isSold
        #string teamName
        +Player(int, string, string, double)
        +virtual ~Player()
        +virtual void displayDetails() const*
        +virtual double calculateRating() const*
        +int getId() const
        +string getName() const
        +string getRole() const
        +double getBasePrice() const
        +double getSoldPrice() const
        +bool getIsSold() const
        +string getTeamName() const
        +void assignToTeam(string, double)
    }

    class Batsman {
        -int runs
        -double strikeRate
        -int centuries
        +displayDetails() const
        +calculateRating() const
    }

    class Bowler {
        -int wickets
        -double economy
        +displayDetails() const
        +calculateRating() const
    }

    class AllRounder {
        -int runs
        -int wickets
        -double strikeRate
        -double economy
        +displayDetails() const
        +calculateRating() const
    }

    class WicketKeeper {
        -int runs
        -int dismissals
        +displayDetails() const
        +calculateRating() const
    }

    Player <|-- Batsman : Inheritance
    Player <|-- Bowler : Inheritance
    Player <|-- AllRounder : Inheritance
    Player <|-- WicketKeeper : Inheritance

    class Team {
        -int id
        -string name
        -string mascot
        -double totalBudget
        -double remainingBudget
        -vector~Player*~ squad
        +Team(int, string, string, double)
        +bool canAfford(double) const
        +bool deductBudget(double)
        +void addPlayer(Player*)
        +void displaySquad() const
        +double getRemainingBudget() const
        +string getName() const
        +vector~Player*~ getSquad() const
    }

    Team o-- Player : Aggregates (Squad)

    class AuctionSystem {
        -vector~Player*~ playerPool
        -vector~Team*~ teams
        +AuctionSystem(vector~Player*~, vector~Team*~)
        +Player* getNextUnsoldPlayer()
        +void auctionSinglePlayer(Player*, bool)
        +void runFullAuction(bool)
    }

    AuctionSystem o-- Player : Manages Pool
    AuctionSystem o-- Team : Manages Franchises
```

---

## ⚡ Auction Engine Logic & State Machine

```mermaid
stateDiagram-v2
    [*] --> SelectPlayer : Next Player from Pool
    SelectPlayer --> BiddingRound : Announce Player & Base Price
    
    state BiddingRound {
        [*] --> AwaitBid
        AwaitBid --> ValidateBid : Franchise places increment bid
        ValidateBid --> CheckPurse : Sufficient purse?
        CheckPurse --> AcceptBid : Yes
        CheckPurse --> RejectBid : No (Budget exceeded)
        RejectBid --> AwaitBid
        AcceptBid --> NewHighestBidder : Update Top Bidder
        NewHighestBidder --> AwaitBid : Counter-bids?
    }

    BiddingRound --> EvaluateOutcome : Hammer falls (Choice = 0)
    EvaluateOutcome --> MarkSold : Bid >= Base Price
    EvaluateOutcome --> MarkUnsold : No bids placed

    MarkSold --> DeductBudget : Deduct winning amount from purse
    DeductBudget --> AddToSquad : Assign player to franchise
    AddToSquad --> NextPlayerCheck

    MarkUnsold --> NextPlayerCheck : Retain in unsold pool

    state NextPlayerCheck {
        [*] --> MorePlayers : Pool active
        [*] --> AuctionFinished : Pool completed
    }

    MorePlayers --> SelectPlayer
    AuctionFinished --> [*]
```

---

## 📂 Project Directory Structure

```
IOOP project/
│
├── README.md                  # Complete architecture, diagrams & viva documentation
├── .gitignore                 # Ignores compiled binaries (*.exe, *.o)
└── main.cpp                   # Clean, self-contained C++ implementation
```

---

## 🚀 How to Compile & Run

```bash
# Compile using any standard C++ compiler
g++ main.cpp -o main.exe

# Run the executable
.\main.exe
```

---

## 🎓 Viva Questions & Answers Cheat-Sheet

1. **Q: Why is `Player` an abstract class?**
   - **A**: A player in cricket always plays a specialized role (Batsman, Bowler, etc.). Declaring pure virtual functions (`displayDetails() = 0` and `calculateRating() = 0`) turns `Player` into an abstract class, preventing accidental instantiation and ensuring each role provides its specific implementation.

2. **Q: Where is runtime polymorphism demonstrated?**
   - **A**: `Team` stores players as a collection of base pointers: `std::vector<Player*> squad`. When we loop over `squad` and call `squad[i]->displayDetails()`, C++ uses the V-Table pointer (`vptr`) to dynamically invoke the derived class method at runtime.

3. **Q: Why does `Player` have a virtual destructor?**
   - **A**: When deleting derived objects via base class pointers (`delete player;`), a virtual destructor guarantees the complete destructor chain is called, avoiding memory leaks.

4. **Q: How does the system demonstrate Encapsulation?**
   - **A**: Critical team data (`remainingBudget`, `squad`) is `private`. The purse can only be reduced through `deductBudget()`, which checks `canAfford()` to prevent overspending.

5. **Q: Where is function overloading used?**
   - **A**: We overloaded `findPlayer`:
     - `Player* findPlayer(const vector<Player*>& pool, int id);`
     - `Player* findPlayer(const vector<Player*>& pool, string name);`
     Both share the same function name but take different argument types.
