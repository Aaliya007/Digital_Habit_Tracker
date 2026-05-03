#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <cctype>

using namespace std;

// ======================= COLOR CONSTANTS =======================
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";

const string BLACK   = "\033[30m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string WHITE   = "\033[37m";

const string BRIGHT_RED     = "\033[91m";
const string BRIGHT_GREEN   = "\033[92m";
const string BRIGHT_YELLOW  = "\033[93m";
const string BRIGHT_BLUE    = "\033[94m";
const string BRIGHT_MAGENTA = "\033[95m";
const string BRIGHT_CYAN    = "\033[96m";

const string BG_RED     = "\033[41m";
const string BG_GREEN   = "\033[42m";
const string BG_YELLOW  = "\033[43m";
const string BG_BLUE    = "\033[44m";
const string BG_MAGENTA = "\033[45m";
const string BG_CYAN    = "\033[46m";

// ======================= UI HELPERS =======================
void printLine(int width = 100, char ch = '=') {
    cout << BRIGHT_CYAN;
    for (int i = 0; i < width; i++) cout << ch;
    cout << RESET << "\n";
}

string centerText(const string& text, int width) {
    if ((int)text.size() >= width) return text;
    int left = (width - text.size()) / 2;
    return string(left, ' ') + text;
}

void printHeader(const string& title, const string& subtitle = "") {
    cout << "\n\n";
    cout << BRIGHT_BLUE;
    for (int i = 0; i < 100; i++) cout << "=";
    cout << RESET << "\n";

    cout << BOLD << BRIGHT_MAGENTA << centerText("DIGITAL HABIT TRACKER", 100) << RESET << "\n";

    if (!subtitle.empty())
        cout << BRIGHT_YELLOW << centerText(subtitle, 100) << RESET << "\n";

    cout << BRIGHT_BLUE;
    for (int i = 0; i < 100; i++) cout << "=";
    cout << RESET << "\n";

    cout << BOLD << BRIGHT_GREEN << title << RESET << "\n";

    cout << BRIGHT_CYAN;
    for (int i = 0; i < 100; i++) cout << "-";
    cout << RESET << "\n";
}

string badgeFromStreak(int streak) {
    if (streak >= 30) return "Legend";
    if (streak >= 14) return "Gold";
    if (streak >= 7)  return "Silver";
    if (streak >= 3)  return "Bronze";
    return "Starter";
}

string coloredBadge(string badge) {
    if (badge == "Legend") return BRIGHT_MAGENTA + badge + RESET;
    if (badge == "Gold")   return BRIGHT_YELLOW + badge + RESET;
    if (badge == "Silver") return BRIGHT_CYAN + badge + RESET;
    if (badge == "Bronze") return YELLOW + badge + RESET;
    return WHITE + badge + RESET;
}

string progressBar(int value, int maxValue = 10) {
    int filled = min(value, maxValue);
    string result = "[";

    for (int i = 0; i < maxValue; i++) {
        if (i < filled) result += BRIGHT_GREEN + string("#") + RESET;
        else result += BRIGHT_RED + string(".") + RESET;
    }

    result += "]";
    return result;
}

void printCard(const string& title, const string& value, const string& color) {
    cout << color << "+------------------------------+" << RESET << "\n";
    cout << color << "| " << left << setw(28) << title << " |" << RESET << "\n";
    cout << color << "| " << left << setw(28) << value << " |" << RESET << "\n";
    cout << color << "+------------------------------+" << RESET << "\n";
}

void waitForUser() {
    cout << BRIGHT_YELLOW << "\nEnter 1 to continue: " << RESET;
    int dummy;
    cin >> dummy;
}

void inputLine(string& s) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, s);
}

string toLowerCase(string s) {
    for (char &c : s) {
        c = tolower(static_cast<unsigned char>(c));
    }
    return s;
}

// helper only for alignment
string padRight(const string& text, int width) {
    if ((int)text.length() >= width) return text.substr(0, width - 1) + " ";
    return text + string(width - text.length(), ' ');
}

// ======================= BASE CLASS =======================
class Habit {
protected:
    int id;
    string name;
    string frequency;
    int streak;
    int points;
    bool completed;

public:
    Habit(int i = 0, string n = "", string f = "", int s = 0, int p = 0, bool c = false)
        : id(i), name(n), frequency(f), streak(s), points(p), completed(c) {}

    virtual ~Habit() {}

    virtual void markComplete() = 0;
    virtual void resetCycle() = 0;
    virtual string getType() const = 0;

    int getId() const { return id; }
    string getName() const { return name; }
    string getFrequency() const { return frequency; }
    int getStreak() const { return streak; }
    int getPoints() const { return points; }
    bool isCompleted() const { return completed; }

    string getBadge() const {
        return badgeFromStreak(streak);
    }

    virtual void save(ofstream& out) const {
        out << getType() << "\n";
        out << id << "\n";
        out << name << "\n";
        out << frequency << "\n";
        out << streak << "\n";
        out << points << "\n";
        out << completed << "\n";
    }

    virtual void loadCommon(ifstream& in) {
        in >> id;
        in.ignore();
        getline(in, name);
        getline(in, frequency);
        in >> streak >> points >> completed;
        in.ignore();
    }
};

// ======================= DAILY HABIT =======================
class DailyHabit : public Habit {
public:
    DailyHabit(int i = 0, string n = "", int s = 0, int p = 0, bool c = false)
        : Habit(i, n, "Daily", s, p, c) {}

    void markComplete() override {
        if (!completed) {
            completed = true;
            streak++;
            points += 10;
        }
    }

    void resetCycle() override {
        completed = false;
    }

    string getType() const override {
        return "Daily";
    }
};

// ======================= WEEKLY HABIT =======================
class WeeklyHabit : public Habit {
public:
    WeeklyHabit(int i = 0, string n = "", int s = 0, int p = 0, bool c = false)
        : Habit(i, n, "Weekly", s, p, c) {}

    void markComplete() override {
        if (!completed) {
            completed = true;
            streak++;
            points += 20;
        }
    }

    void resetCycle() override {
        completed = false;
    }

    string getType() const override {
        return "Weekly";
    }
};

// ======================= TRACKER CLASS =======================
class HabitTracker {
private:
    vector<Habit*> habits;
    string fileName;
    int nextId;

public:
    HabitTracker(string file = "habit_data.txt") {
        fileName = file;
        nextId = 1;
    }

    ~HabitTracker() {
        for (auto h : habits) delete h;
        habits.clear();
    }

    // Operator Overloading
    HabitTracker& operator+(Habit* h) {
        habits.push_back(h);
        return *this;
    }

    void loadFromFile() {
        ifstream in(fileName);
        if (!in) return;

        int count;
        in >> count;
        in.ignore();

        for (int i = 0; i < count; i++) {
            string type;
            getline(in, type);

            Habit* h = nullptr;

            if (type == "Daily") h = new DailyHabit();
            else if (type == "Weekly") h = new WeeklyHabit();

            if (h != nullptr) {
                h->loadCommon(in);
                habits.push_back(h);
                nextId = max(nextId, h->getId() + 1);
            }
        }

        in.close();
    }

    void saveToFile() const {
        ofstream out(fileName);
        out << habits.size() << "\n";
        for (auto h : habits) {
            h->save(out);
        }
        out.close();
    }

    int totalHabits() const {
        return habits.size();
    }

    int totalCompleted() const {
        int count = 0;
        for (auto h : habits) {
            if (h->isCompleted()) count++;
        }
        return count;
    }

    int totalPoints() const {
        int sum = 0;
        for (auto h : habits) {
            sum += h->getPoints();
        }
        return sum;
    }

    Habit* topHabit() const {
        if (habits.empty()) return nullptr;
        Habit* best = habits[0];
        for (auto h : habits) {
            if (h->getStreak() > best->getStreak()) {
                best = h;
            }
        }
        return best;
    }

    void dashboard() const {
        printHeader("Dashboard", "Check your habit progress");

        printCard("Total Habits", to_string(totalHabits()), BRIGHT_CYAN);
        printCard("Completed This Cycle", to_string(totalCompleted()), BRIGHT_GREEN);
        printCard("Total Points", to_string(totalPoints()), BRIGHT_YELLOW);

        Habit* best = topHabit();
        if (best) {
            printCard("Top Habit", best->getName() + " (" + to_string(best->getStreak()) + " streak)", BRIGHT_MAGENTA);
        } else {
            printCard("Top Habit", "No data yet", BRIGHT_MAGENTA);
        }

        cout << "\n" << BOLD << BRIGHT_YELLOW << "Quick Navigation\n" << RESET;
        printLine(100, '-');

        cout << BRIGHT_GREEN   << "[1] Add Habit        "
             << BRIGHT_CYAN    << "[2] View Habits       "
             << BRIGHT_YELLOW  << "[3] Search Habit      "
             << BRIGHT_MAGENTA << "[4] Mark Habit as Complete\n" << RESET;

        cout << BRIGHT_RED     << "[5] Delete Habit     "
             << BRIGHT_BLUE    << "[6] Reports           "
             << BRIGHT_GREEN   << "[7] Leaderboard       "
             << BRIGHT_CYAN    << "[8] New Cycle\n" << RESET;

        cout << BRIGHT_YELLOW  << "[9] Save             "
             << BRIGHT_RED     << "[0] Exit\n" << RESET;

        printLine(100, '-');
    }

    void addHabit() {
        printHeader("Add New Habit");

        string habitName;
        int typeChoice;

        cout << BRIGHT_CYAN << "Enter habit name: " << RESET;
        inputLine(habitName);

        cout << "\n" << BRIGHT_YELLOW << "Select habit type:\n" << RESET;
        cout << BRIGHT_GREEN << "1. Daily Habit\n" << RESET;
        cout << BRIGHT_MAGENTA << "2. Weekly Habit\n" << RESET;
        cout << BRIGHT_CYAN << "Enter choice: " << RESET;
        cin >> typeChoice;

        if (typeChoice == 1) {
            *this + new DailyHabit(nextId++, habitName);
            cout << BRIGHT_GREEN << "\nDaily habit added successfully.\n" << RESET;
        } else if (typeChoice == 2) {
            *this + new WeeklyHabit(nextId++, habitName);
            cout << BRIGHT_GREEN << "\nWeekly habit added successfully.\n" << RESET;
        } else {
            cout << BRIGHT_RED << "\nInvalid choice.\n" << RESET;
        }
    }

    void viewHabits() const {
        printHeader("All Habits");

        if (habits.empty()) {
            cout << BRIGHT_RED << "No habits found.\n" << RESET;
            return;
        }

        cout << padRight("ID", 6)
             << padRight("Habit", 22)
             << padRight("Type", 12)
             << padRight("Streak", 10)
             << padRight("Points", 10)
             << padRight("Badge", 12)
             << padRight("Status", 12)
             << "Progress\n";

        printLine(100, '-');

        for (auto h : habits) {
            string idStr = to_string(h->getId());
            string streakStr = to_string(h->getStreak());
            string pointsStr = to_string(h->getPoints());
            string statusPlain = h->isCompleted() ? "Done" : "Pending";
            string statusColor = h->isCompleted() ? BRIGHT_GREEN + statusPlain + RESET
                                                  : BRIGHT_RED + statusPlain + RESET;

            cout << padRight(idStr, 6)
                 << BRIGHT_CYAN << padRight(h->getName(), 22) << RESET
                 << BRIGHT_YELLOW << padRight(h->getFrequency(), 12) << RESET
                 << padRight(streakStr, 10)
                 << padRight(pointsStr, 10)
                 << padRight("", 0);

            cout << coloredBadge(h->getBadge());
            int badgeSpace = 12 - (int)h->getBadge().length();
            if (badgeSpace > 0) cout << string(badgeSpace, ' ');

            cout << statusColor;
            int statusSpace = 12 - (int)statusPlain.length();
            if (statusSpace > 0) cout << string(statusSpace, ' ');

            cout << progressBar(h->getStreak()) << "\n";
        }
    }

    void searchHabit() const {
        printHeader("Search Habit");

        if (habits.empty()) {
            cout << BRIGHT_RED << "No habits available.\n" << RESET;
            return;
        }

        string query;
        cout << BRIGHT_CYAN << "Enter habit name: " << RESET;
        inputLine(query);

        bool found = false;
        string searchName = toLowerCase(query);

        for (auto h : habits) {
            if (toLowerCase(h->getName()) == searchName) {
                found = true;
                cout << BRIGHT_GREEN << "\nHabit Found\n" << RESET;
                printLine(100, '-');
                cout << BRIGHT_YELLOW << "ID       : " << RESET << h->getId() << "\n";
                cout << BRIGHT_YELLOW << "Name     : " << RESET << h->getName() << "\n";
                cout << BRIGHT_YELLOW << "Type     : " << RESET << h->getFrequency() << "\n";
                cout << BRIGHT_YELLOW << "Streak   : " << RESET << h->getStreak() << "\n";
                cout << BRIGHT_YELLOW << "Points   : " << RESET << h->getPoints() << "\n";
                cout << BRIGHT_YELLOW << "Badge    : " << RESET << coloredBadge(h->getBadge()) << "\n";
                cout << BRIGHT_YELLOW << "Status   : " << RESET
                     << (h->isCompleted() ? BRIGHT_GREEN + string("Done") : BRIGHT_RED + string("Pending"))
                     << RESET << "\n";
                cout << BRIGHT_YELLOW << "Progress : " << RESET << progressBar(h->getStreak()) << "\n";
                break;
            }
        }

        if (!found) {
            cout << BRIGHT_RED << "\nHabit not found.\n" << RESET;
        }
    }

    void completeHabit() {
        printHeader("Complete Habit by Name");

        if (habits.empty()) {
            cout << BRIGHT_RED << "No habits available.\n" << RESET;
            return;
        }

        cout << padRight("Habit", 22)
             << padRight("Type", 12)
             << padRight("Streak", 10)
             << padRight("Points", 10)
             << padRight("Status", 12)
             << "\n";

        printLine(100, '-');

        for (auto h : habits) {
            string streakStr = to_string(h->getStreak());
            string pointsStr = to_string(h->getPoints());
            string statusPlain = h->isCompleted() ? "Done" : "Pending";
            string statusColor = h->isCompleted() ? BRIGHT_GREEN + statusPlain + RESET
                                                  : BRIGHT_RED + statusPlain + RESET;

            cout << BRIGHT_CYAN << padRight(h->getName(), 22) << RESET
                 << BRIGHT_YELLOW << padRight(h->getFrequency(), 12) << RESET
                 << padRight(streakStr, 10)
                 << padRight(pointsStr, 10)
                 << statusColor;

            int statusSpace = 12 - (int)statusPlain.length();
            if (statusSpace > 0) cout << string(statusSpace, ' ');

            cout << "\n";
        }

        string habitName;
        cout << BRIGHT_CYAN << "\nEnter habit name to mark as complete: " << RESET;
        inputLine(habitName);

        string targetName = toLowerCase(habitName);

        for (auto h : habits) {
            if (toLowerCase(h->getName()) == targetName) {
                if (h->isCompleted()) {
                    cout << BRIGHT_RED << "\nThis habit is already completed in the current cycle.\n" << RESET;
                    return;
                }

                h->markComplete();

                cout << BRIGHT_GREEN << "\nHabit marked as completed successfully.\n" << RESET;
                cout << BRIGHT_YELLOW << "Habit Name : " << RESET << h->getName() << "\n";
                cout << BRIGHT_YELLOW << "Type       : " << RESET << h->getFrequency() << "\n";
                cout << BRIGHT_YELLOW << "Streak     : " << RESET << h->getStreak() << "\n";
                cout << BRIGHT_YELLOW << "Points     : " << RESET << h->getPoints() << "\n";
                cout << BRIGHT_YELLOW << "Badge      : " << RESET << coloredBadge(h->getBadge()) << "\n";
                return;
            }
        }

        cout << BRIGHT_RED << "\nHabit name not found.\n" << RESET;
    }

    void deleteHabit() {
        printHeader("Delete Habit");

        if (habits.empty()) {
            cout << BRIGHT_RED << "No habits available.\n" << RESET;
            return;
        }

        string habitName;
        cout << BRIGHT_CYAN << "Enter habit name to delete: " << RESET;
        inputLine(habitName);

        string targetName = toLowerCase(habitName);

        for (auto it = habits.begin(); it != habits.end(); ++it) {
            if (toLowerCase((*it)->getName()) == targetName) {
                delete *it;
                habits.erase(it);
                cout << BRIGHT_GREEN << "\nHabit deleted successfully.\n" << RESET;
                return;
            }
        }

        cout << BRIGHT_RED << "\nHabit name not found.\n" << RESET;
    }

    void resetCycle() {
        printHeader("Start New Day / New Cycle");

        for (auto h : habits) {
            h->resetCycle();
        }

        cout << BRIGHT_GREEN << "All habits reset for the next cycle.\n" << RESET;
    }

    void reports() const {
        printHeader("Progress Reports");

        if (habits.empty()) {
            cout << BRIGHT_RED << "No habits available for report.\n" << RESET;
            return;
        }

        int completed = totalCompleted();
        int pending = totalHabits() - completed;
        Habit* best = topHabit();

        cout << BRIGHT_CYAN << "Total Habits    : " << RESET << totalHabits() << "\n";
        cout << BRIGHT_GREEN << "Completed       : " << RESET << completed << "\n";
        cout << BRIGHT_RED << "Pending         : " << RESET << pending << "\n";
        cout << BRIGHT_YELLOW << "Total Points    : " << RESET << totalPoints() << "\n";
        cout << BRIGHT_MAGENTA << "Best Habit      : " << RESET << (best ? best->getName() : "None") << "\n";
        cout << BRIGHT_BLUE << "Highest Streak  : " << RESET << (best ? best->getStreak() : 0) << "\n";

        cout << "\n" << BRIGHT_YELLOW << "Completion Ratio : " << RESET;
        int ratio = totalHabits() == 0 ? 0 : (completed * 10 / totalHabits());
        cout << progressBar(ratio) << " " << completed << "/" << totalHabits() << "\n";
    }

    void leaderboard() const {
        printHeader("Leaderboard");

        if (habits.empty()) {
            cout << BRIGHT_RED << "No habits available.\n" << RESET;
            return;
        }

        vector<Habit*> temp = habits;

        sort(temp.begin(), temp.end(), [](Habit* a, Habit* b) {
            if (a->getPoints() == b->getPoints())
                return a->getStreak() > b->getStreak();
            return a->getPoints() > b->getPoints();
        });

        cout << padRight("Rank", 8)
             << padRight("Habit", 22)
             << padRight("Type", 12)
             << padRight("Points", 10)
             << padRight("Streak", 10)
             << "Badge\n";

        printLine(100, '-');

        for (size_t i = 0; i < temp.size(); i++) {
            string rankStr = to_string(i + 1);
            string pointsStr = to_string(temp[i]->getPoints());
            string streakStr = to_string(temp[i]->getStreak());

            cout << BRIGHT_YELLOW << padRight(rankStr, 8) << RESET
                 << BRIGHT_CYAN << padRight(temp[i]->getName(), 22) << RESET
                 << BRIGHT_MAGENTA << padRight(temp[i]->getFrequency(), 12) << RESET
                 << padRight(pointsStr, 10)
                 << padRight(streakStr, 10)
                 << coloredBadge(temp[i]->getBadge()) << "\n";
        }
    }
};

// ======================= MAIN FUNCTION =======================
int main() {
    HabitTracker tracker;
    tracker.loadFromFile();

    int choice;

    do {
        tracker.dashboard();
        cout << BRIGHT_CYAN << "Enter your choice: " << RESET;
        cin >> choice;

        switch (choice) {
            case 1:
                tracker.addHabit();
                waitForUser();
                break;
            case 2:
                tracker.viewHabits();
                waitForUser();
                break;
            case 3:
                tracker.searchHabit();
                waitForUser();
                break;
            case 4:
                tracker.completeHabit();
                waitForUser();
                break;
            case 5:
                tracker.deleteHabit();
                waitForUser();
                break;
            case 6:
                tracker.reports();
                waitForUser();
                break;
            case 7:
                tracker.leaderboard();
                waitForUser();
                break;
            case 8:
                tracker.resetCycle();
                waitForUser();
                break;
            case 9:
                tracker.saveToFile();
                printHeader("Save Data");
                cout << BRIGHT_GREEN << "Data saved successfully in habit_data.txt\n" << RESET;
                waitForUser();
                break;
            case 0:
                tracker.saveToFile();
                printHeader("Exit");
                cout << BRIGHT_GREEN << "Thank you for using Digital Habit Tracker.\n" << RESET;
                break;
            default:
                printHeader("Invalid Choice");
                cout << BRIGHT_RED << "Please enter a valid option.\n" << RESET;
                waitForUser();
        }

    } while (choice != 0);

    return 0;
}